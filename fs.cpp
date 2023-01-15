#include "fs.h"

using namespace std;
using boost::unique_lock;
using boost::shared_lock;
using boost::shared_mutex;
using boost::mutex;


// Implementation of function in fs.h
using boost::mutex;

void traverse(int block) {
    // Push the current "Root" block id to used
    used_block.insert(block);
    fs_inode root_inode;

    disk_readblock(block, &root_inode);

    if(root_inode.type == 'f') {
        // File - Push everything in inode block into used
        for(unsigned int i = 0; i < root_inode.size; ++i) {
            used_block.insert(root_inode.blocks[i]);      
        }

    } else {
        // directory
        fs_direntry direntries[FS_MAX_DIR];
        for(unsigned int i = 0; i < root_inode.size; ++i) {
            // Push the current Direntry's inode disk block into used
            used_block.insert(root_inode.blocks[i]);

            // Go into each direntry and recursive call on them
            // Make sure that we have lock for the direntry's previous inode
            disk_readblock(root_inode.blocks[i], direntries);

            for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {
                if(direntries[j].inode_block != 0) {
                    traverse(direntries[j].inode_block);
                }
            }
        }
    }
}

void handle_request(int connectionfd) {
    // {
    //     unique_lock<mutex> cout_l(cout_lock);
    //     cout << "Handle Request: Called for connectionfd :" << connectionfd << "\n";
    // }

    // Recieve cleartext from connectionfd, end by null terminator
    string cleartext = "";
    if(receive_cleartext(connectionfd, cleartext) < 0) {
        close(connectionfd);
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Close in Recieve_Cleartext. \n";
        // }
        return;
    };

    // Parse the cleartext
    string username = "";
    int size = -1;
    if(parser::parse_cleartext(cleartext, username, size, users) < 0) {
        close(connectionfd);
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Close in Parse_Cleartext. \n";
        // }
        return;
    };

    // Redundant - already checked in parsed_cleartext, still keep it
    if(users.count(username) == 0) {
        close(connectionfd);
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Username does not exist. \n";
        // }
        return;
    }

    // Recieve request from connectionfd, end by null terminator
    // Already know size of buffer, don't need to check null terminator
    char msg[size];
	bzero(msg, size);
    if(recv(connectionfd, &msg, sizeof(msg), MSG_WAITALL) < 0) {
        close(connectionfd);
        return;
    };
    
    // Decrypt the request by user password
    void* decrypted_msg[size];
    int decrypted_size = fs_decrypt(users[username].c_str(), (void*) msg, size, decrypted_msg);
    if(decrypted_size < 0) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Decyption failure - The message to be decrypted is of size" 
        //         << sizeof(msg) << " " << size << " and the password is " << users[username] << ". \n";
        // }
        close(connectionfd);
        return;
    }
    string decrypted_string((char *) decrypted_msg, decrypted_size);
    // {
    //     unique_lock<mutex> cout_l(cout_lock);
    //     cout << "Handle Request: The decrypted string is " << decrypted_string << "decrypt size is " << decrypted_size << ". \n";
    // }

    // Parse the Decrypted message
    parser::Request parsed_msg = parser::parse_request(decrypted_string);
    bool process_signal = false;
    string response = "";
    switch (parsed_msg.request) {
        case 0:
            // parsing error
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: ***** Encountering parsing error. \n";
            // }
            break;
        case 1:
            // Create Session
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: A session request. \n";
            // }
            process_signal = process_session(parsed_msg, response, username);
            break;
        case 2:
            // Readblock
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: A readblock request. \n";
            // }
            process_signal = process_read(parsed_msg, response, username);
            break;
        case 3:
            // Writeblock
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: A writeblock request. \n";
            // }
            process_signal = process_write(parsed_msg, response, username);
            break;
        case 4:
            // Create File/Dir
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: A create request. \n";
            // }
            process_signal = process_create(parsed_msg, response, username);
            break;
        case 5:
            // Delete
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request: A delete request. \n";
            // }
            process_signal = process_delete(parsed_msg, response, username);
            break;
        default:
            // other undefined header
            // {
            //     unique_lock<mutex> cout_l(cout_lock);
            //     cout << "Handle Request Error: Header Undefined. \n";
            // }
            break;
    }

    // processing failed
    if(process_signal == false) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Processing falied with request NO." << parsed_msg.request << ". \n";
        // }
        close(connectionfd);
        return;
    }

    // encrypt and send back response msg
    if(!send_back(connectionfd, username, parsed_msg, response)) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Handle Request Error: Failure in responding. \n";
        // }
    };

    // close connection
    close(connectionfd);
    return;
} 

int receive_cleartext(int connectionfd, string& res) {
    char c;
    string temp = "";
    while(true) {
        if(recv(connectionfd, &c, sizeof(c), MSG_WAITALL) < 0) return -1;
        if(c == '\0') {
            res = temp;
            break;
        }
        temp += c;
    }
    return 1;
}

bool check_target(fs_inode& target, int& target_block, unsigned int level, parser::Request& request, string& username, bool use_writelock) {
    // Invariant - target is reader locked, parent is released
    //           - on success, return true and hold the dir lock
    //           - on failure, return false and relase all locks

    // Sanity check - Return false if owner does no match
    //              - if one of the type is not directory
    if((strlen(target.owner) > 0) && strcmp(target.owner, username.c_str()) != 0) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Check_Target Error: Owner doesn't match. \n";
        // }
        if(use_writelock && level >= (request.filenames.size() - 1)) {
            inode_locks[target_block].unlock();
        } else {
            inode_locks[target_block].unlock_shared();
        }
        return false;
    }
    if(target.type != 'd') {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Check_Target Error: One of the filename is not directory. \n";
        // }
        if(use_writelock && level >= (request.filenames.size() - 1)) {
            inode_locks[target_block].unlock();
        } else {
            inode_locks[target_block].unlock_shared();
        }
        return false;
    }

    // Got Second Last!
    // target lock returned locked
    if(level >= (request.filenames.size() - 1)) return true;
    
    
    // Loop through all directories on this level
    fs_direntry direntries[FS_MAX_DIR];
    for(unsigned int i = 0; i < target.size; ++i) {

        // Lock - read the content of target
        // mutex[target.blocks[i]].lock_shared();
        disk_readblock(target.blocks[i], direntries);

        for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {
            // Recursive call only if the dirname match our path
            if(direntries[j].inode_block != 0 && 
               strcmp(direntries[j].name, request.filenames[level].c_str()) == 0) {

                // Make sure that we have lock for the direntry's previous inode
                if(use_writelock && level >= (request.filenames.size() - 2)) {
                    inode_locks[direntries[j].inode_block].lock();
                } else {
                    inode_locks[direntries[j].inode_block].lock_shared();
                }
                inode_locks[target_block].unlock_shared();

                disk_readblock(direntries[j].inode_block, &target);
                target_block = direntries[j].inode_block;
                return check_target(target, target_block, level + 1, request, username, use_writelock);
            }
        }
        // mutex[target.blocks[i]].unlock_shared();
    }

    // {
    //     unique_lock<mutex> cout_l(cout_lock);
    //     cout << "Check_Target Error: Invalid Pass. \n";
    // }
    inode_locks[target_block].unlock_shared();
    return false;
}

bool process_session(parser::Request& vals, string& res, string& username) {

    // session = 0 or could not have # exceed max
    if(vals.session != 0) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Process_session failed: session request has nonzero session field. \n";
        }
        return false;
    }

    {
        unique_lock<mutex> session_l(session_lock);
        if(session >= UINT32_MAX) {
            {
                unique_lock<mutex> cout_l(cout_lock);
                cout << "Process_session failed: Session full or invalid. \n";
            }
            return false;
        }
        
        unique_lock<mutex> session_map_l(session_map_lock);
        // Assigned the next unused session and RESPONSE
        session_map[session] = {username, vals.sequence};
        res += to_string(session) + " " + to_string(vals.sequence) + '\0';
        ++session;
    }
    return true;
}


bool process_read(parser::Request& vals, string& res, string& username) {
    /// Check Sequence, Session, Username Valid
    if(!check_sequence(username, vals.session, vals.sequence)) return false;
    
    // block invalid, no need for traversal
    if(vals.block >= FS_MAXFILEBLOCKS || vals.block < 0) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Block cannot be valid\n";
        }
        return false;
    }
    /// Check Pass Valid and Traverse to the second last path
    fs_inode target;
    int target_block = 0;

    // Lock - lock the root inode
    inode_locks[0].lock_shared();
    disk_readblock(0, &target);
    if(!check_target(target, target_block, 0, vals, username, false)) return false;

    
    // Search for the directory of read
    fs_direntry direntries[FS_MAX_DIR];
    for(unsigned int i = 0; i < target.size; ++i) {
        disk_readblock(target.blocks[i], direntries);
        for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {

            // Read only if filename matches
            if(direntries[j].inode_block != 0 && 
               strcmp(direntries[j].name, vals.filenames.back().c_str()) == 0) {
                inode_locks[direntries[j].inode_block].lock_shared();
                inode_locks[target_block].unlock_shared();
                disk_readblock(direntries[j].inode_block, &target);
                // Sanity check - not file, invalid
                //              - block not in range
                //              - owner does not match

                if(target.type != 'f') {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_read Error: Not a File. \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock_shared();
                    return false;
                }
                if(vals.block < 0 || vals.block >= target.size) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_read Error: Block not in range \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock_shared();
                    return false;
                }
                if(strcmp(target.owner, username.c_str()) != 0) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_read Error: Owner does not match \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock_shared();
                    return false;
                }

                // Read block
                char data[FS_BLOCKSIZE];
                disk_readblock(target.blocks[vals.block], data);
                inode_locks[direntries[j].inode_block].unlock_shared();

                // RESPONSE
                string temp_data(data, FS_BLOCKSIZE);
                res = to_string(vals.session) + " " + to_string(vals.sequence) + '\0' + temp_data;
                return true;
            }
        }
    }
    // Not found

    {
        unique_lock<mutex> cout_l(cout_lock);
        cout << "Process_read Error: Pass not found \n";
    }

    inode_locks[target_block].unlock_shared();
    return false;
}


bool process_write(parser::Request& vals, string& res, string& username) {
    
    /// Check Sequence, Session, Username Valid
    if(!check_sequence(username, vals.session, vals.sequence)) return false;

    // block invalid, no need for traversal
    if(vals.block >= FS_MAXFILEBLOCKS || vals.block < 0) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Block cannot be valid\n";
        }
        return false;
    }


    /// Check Pass Valid and Traverse to the second last path
    fs_inode target;
    int target_block = 0;
    // Lock - lock the root inode
    inode_locks[0].lock_shared();
    disk_readblock(0, &target);
    if(!check_target(target, target_block , 0, vals, username, false)) return false;

    /// Traverse through all blocks(array of direntry) in the directory
    fs_direntry direntries[FS_MAX_DIR];
    for(unsigned int i = 0; i < target.size; ++i) {
        disk_readblock(target.blocks[i], direntries);

        //// For Each Direntry block, Loop through everything to check filename
        for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {
            if(direntries[j].inode_block != 0 && 
               strcmp(direntries[j].name,vals.filenames.back().c_str()) == 0) {
                // acquire write lock
                inode_locks[direntries[j].inode_block].lock();
                inode_locks[target_block].unlock_shared();
                disk_readblock(direntries[j].inode_block, &target);
                
                // Sanity Check - if it is not file, Invalid and return false
                //              - if the block is not in range, return false
                //              - if user is not the owner, return false
                if(target.type != 'f') {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_write Error: Not a File. \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock();
                    return false;
                }
                if(vals.block < 0 || vals.block > target.size) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_write Error: Block not in range \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock();
                    return false;
                }
                if(strcmp(target.owner, username.c_str()) != 0) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_write Error: Owner does not match \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock();
                    return false;
                }


                // IF we need to assign a new block, check there's enough space & Assign & Write back
                // IF not, Write back
                if(vals.block == target.size) {
                    // Storage Sanity Check
                    if (target.size == FS_MAXFILEBLOCKS) {
                        {
                            unique_lock<mutex> cout_l(cout_lock);
                            cout << "Process_write Error: No Space in Inode blocks. \n";
                        }
                        inode_locks[direntries[j].inode_block].unlock();
                        return false;
                    }

                    // Check available block existing for write
                    {
                        unique_lock<mutex> used_block_l(used_block_lock);
                        if (avail_block.empty()) {
                            {
                                unique_lock<mutex> cout_l(cout_lock);
                                cout << "Process_write Error: No Space on Disk. \n";
                            }
                            inode_locks[direntries[j].inode_block].unlock();
                            return false;
                        }
                    }

                    // Used_block and Avail_block Lock and Change
                    uint32_t block_assigned;
                    {  
                        unique_lock<mutex> used_block_l(used_block_lock);
                        block_assigned = avail_block.front();
                        used_block.insert(block_assigned);
                        avail_block.pop();
                    }
                    disk_writeblock(block_assigned, vals.body.c_str());
                    target.blocks[target.size] = block_assigned;
                    target.size ++;
                    disk_writeblock(direntries[j].inode_block, &target);
                }
                else disk_writeblock(target.blocks[vals.block], vals.body.c_str());
                
                inode_locks[direntries[j].inode_block].unlock();
                // RESPONSE
                res = to_string(vals.session) + " " + to_string(vals.sequence) + '\0';
                return true;
            }
        }
    }
    // not found
    {
        unique_lock<mutex> cout_l(cout_lock);
        cout << "Process_write Error: Pass not found \n";
    }
    inode_locks[target_block].unlock_shared();
    return false;
}


bool process_create(parser::Request& vals, string& res, string& username) {

    // Sanity Check - Check If there's enough space for allocating
    //              - Check Sequence, Session, Username Valid
    {  
        unique_lock<mutex> used_block_l(used_block_lock);
        if(avail_block.empty()) {
            {
                unique_lock<mutex> cout_l(cout_lock);
                cout << "Process_Create Error: No available blocks, allocation failure. \n";
            }
            return false;
        }
    }

    if(!check_sequence(username, vals.session, vals.sequence)) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Process_Create Error: Check_squence fail. \n";
        }
        return false;
    }

    /// Check Pass Valid and Traverse to the second last path
    int target_block = 0;
    fs_inode target;

    // Lock - lock the root inode
    if(vals.filenames.size() == 1) {
        inode_locks[target_block].lock();
        disk_readblock(target_block, &target);
    } else {
        inode_locks[target_block].lock_shared();
        disk_readblock(target_block, &target);
        if(!check_target(target, target_block, 0, vals, username, true)) return false;
    }
    // write lock on target_block

    // Find an available directory entry & Check if filename already exist
    int availi = -1;
    int availj = -1;
    fs_direntry temp[FS_MAX_DIR];
    fs_direntry direntries[FS_MAX_DIR];
    for(unsigned int i = 0; i < target.size; ++i) {
        disk_readblock(target.blocks[i], temp);
        for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {
            if(availi < 0 && temp[j].inode_block == 0) {
                availi = i;
                availj = j;
                copy(begin(temp), end(temp), begin(direntries));
            }

            // Check for duplicate filename
            if (temp[j].inode_block != 0 &&
                strcmp(temp[j].name, vals.filenames.back().c_str()) == 0) {
                {
                    unique_lock<mutex> cout_l(cout_lock);
                    cout << "Process_Create Error: Duplicate Filename. \n";
                }
                inode_locks[target_block].unlock();
                return false;
            }
        }
    }

    // If there is available direntry space for this new file
    if (availi != -1) {
        // Create a new inode & write
        fs_inode add_inode;
        if (vals.type == true) add_inode.type = 'f';
        else add_inode.type = 'd';
        strcpy(add_inode.owner, username.c_str());
        add_inode.size = 0;

        // Allocate disk space ***********LOCK*************
        int block_assigned;
        {  
            unique_lock<mutex> used_block_l(used_block_lock);
            block_assigned = avail_block.front();
            used_block.insert(avail_block.front());
            direntries[availj].inode_block = avail_block.front();
            avail_block.pop();
        }
        disk_writeblock(block_assigned, &add_inode);

        // Copy filename to direntry and write back direntry
        strcpy(direntries[availj].name, vals.filenames.back().c_str());
        disk_writeblock(target.blocks[availi], &direntries);
        inode_locks[target_block].unlock();

        
        // RESPONSE
        res = to_string(vals.session) + " " + to_string(vals.sequence) + '\0';
        return true;
    }


    // Cannot find an available directory entry, check if all allocated
    // Need at least 2 available block on Disk
    {  
        unique_lock<mutex> used_block_l(used_block_lock);
        if(avail_block.size() < 2) {
            {
                unique_lock<mutex> cout_l(cout_lock);
                cout << "Process_Create Error: Cannot allocate 2 blocks on disk. \n";
            }
            inode_locks[target_block].unlock();
            return false;
        }
    }
    if(target.size >= FS_MAXFILEBLOCKS) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Process_Create Error: Block in Inode used up. \n";
        }
        inode_locks[target_block].unlock();
        return false;
    }
    
    // Enough Space for Creating new block(array of direntries)
    // Create new inode
    fs_inode add_inode;
    if (vals.type == true) add_inode.type = 'f';
    else add_inode.type = 'd';
    strcpy(add_inode.owner, username.c_str());
    add_inode.size = 0;
    // Allocate Disk Space ***********LOCK*************
    uint32_t create_block;
    {  
        unique_lock<mutex> used_block_l(used_block_lock);
        create_block = avail_block.front(); 
        used_block.insert(create_block);
        avail_block.pop();
    }
    disk_writeblock(create_block, &add_inode);
    
    // Create new blocks(array of direntries)
    // ************ AJC modifies here, previously add_dir is a single direntry ******************
    fs_direntry add_dir[FS_MAX_DIR];
    strcpy(add_dir[0].name, vals.filenames.back().c_str());
    add_dir[0].inode_block = create_block;
    for (unsigned int k = 1; k < FS_MAX_DIR; k ++) {
        strcpy(add_dir[k].name, vals.filenames.back().c_str());
        add_dir[k].inode_block = 0;
    }
    // Allocate Disk Space ***********LOCK*************
    uint32_t add_inode_block;
    {  
        unique_lock<mutex> used_block_l(used_block_lock);
        add_inode_block = avail_block.front();
        used_block.insert(add_inode_block);
        avail_block.pop();
    }
    disk_writeblock(add_inode_block, &add_dir);

    // Assign new block disk and write back target
    target.blocks[target.size] = add_inode_block;
    target.size ++;
    disk_writeblock(target_block, &target);
    
    inode_locks[target_block].unlock();

    // RESPONSE
    res = to_string(vals.session) + " " + to_string(vals.sequence) + '\0';
    return true;
}

bool process_delete(parser::Request& vals, string& res, string& username) {

    /// Sanity Check - Check Sequence, Session, Username Valid
    ///              - Check if Deletring Root Node
    if(!check_sequence(username, vals.session, vals.sequence)) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Process_Delete Error: Check_squence fail \n";
        }
        return false;
    }
    if(vals.filenames.size() == 0) {
        {
            unique_lock<mutex> cout_l(cout_lock);
            cout << "Process_Delete Error: Cannot Delete Root \n";
        }
        return false;
    }


    /// Check Pass Valid and Traverse to the second last path
    fs_inode target;
    int target_block = 0;

    // Lock - lock the root inode
    if(vals.filenames.size() == 1) {
        inode_locks[target_block].lock();
        disk_readblock(target_block, &target);
    } else {
        inode_locks[target_block].lock_shared();
        disk_readblock(target_block, &target);
        if(!check_target(target, target_block, 0, vals, username, true)) return false;
    }
    // write lock on target_block

    /// Find the Specific Directory To Delete
    fs_direntry direntries[FS_MAX_DIR];
    for(unsigned int i = 0; i < target.size; ++i) {

        // Lock - reading locked dir
        disk_readblock(target.blocks[i], direntries);
        for(unsigned int j = 0; j < FS_MAX_DIR; ++j) {

            // direntries[j] satisfies only if name match
            if(direntries[j].inode_block != 0 &&
               strcmp(direntries[j].name, vals.filenames.back().c_str()) == 0) {
                fs_inode dir_to_delete;
                
                inode_locks[direntries[j].inode_block].lock();
                disk_readblock(direntries[j].inode_block, &dir_to_delete);

                // Sanity Check - Username Match
                if(strcmp(dir_to_delete.owner, username.c_str()) != 0) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_delete Error: Owner does not match \n";
                    }
                    inode_locks[direntries[j].inode_block].unlock();
                    inode_locks[target_block].unlock();
                    return false;
                }

                // If the filename is a directory
                if (dir_to_delete.type == 'd') {
                    /// Sanity check - Nothing in the directory
                    if (dir_to_delete.size != 0) {
                        inode_locks[direntries[j].inode_block].unlock();
                        inode_locks[target_block].unlock();
                        return false;
                    }
                }

                // If the filename is a file
                else {
                    /// Release all disk spaces for this file **************LOCK************
                    for (uint32_t l = 0; l < dir_to_delete.size; l++) {
                        {  
                            unique_lock<mutex> used_block_l(used_block_lock);
                            used_block.erase(dir_to_delete.blocks[l]);
                            avail_block.push(dir_to_delete.blocks[l]);
                        }
                        {
                            unique_lock<mutex> cout_l(cout_lock);
                            cout << "Process_Delete: Deleting block " << dir_to_delete.blocks[l] << " \n";
                        }
                    }
                }
                
                // Reset this direntry  **************LOCK*************
                {  
                    unique_lock<mutex> used_block_l(used_block_lock);
                    used_block.erase(direntries[j].inode_block);
                    avail_block.push(direntries[j].inode_block);
                }
                {
                    unique_lock<mutex> cout_l(cout_lock);
                    cout << "Process_Delete: Deleting block " << direntries[j].inode_block << " \n";
                }
                int to_release = direntries[j].inode_block;
                direntries[j].inode_block = 0;
                inode_locks[to_release].unlock();
                
                // Check if this block of direntries is empty
                bool empty = true;
                for (unsigned int l = 0; l < FS_MAX_DIR; ++l) {
                    if (direntries[l].inode_block != 0) {
                        empty = false;
                        break;
                    }
                }

                // SHRINK
                if (empty) {
                    {
                        unique_lock<mutex> cout_l(cout_lock);
                        cout << "Process_Delete: Deleting block " << target.blocks[i] << " \n";
                    }


                    /// Free the specific disk block for the array of directories **************LOCK*************
                    {  
                        unique_lock<mutex> used_block_l(used_block_lock);
                        used_block.erase(target.blocks[i]);
                        avail_block.push(target.blocks[i]);
                    }
                    
                    /// Move every block following one step forward
                    for (uint32_t l = i + 1; l < target.size; l++) {
                        target.blocks[l - 1] = target.blocks[l];
                    }
                    target.size--;
                    disk_writeblock(target_block, &target);


                } else {
                    disk_writeblock(target.blocks[i], direntries);
                }

                inode_locks[target_block].unlock();
                // RESPONSE
                res = to_string(vals.session) + " " + to_string(vals.sequence) + '\0';
                return true;
                
            }
        }
    }

    // Not Found
    {
        unique_lock<mutex> cout_l(cout_lock);
        cout << "Process_delete Error: Pass not found \n";
    }
    inode_locks[target_block].unlock();
    return false;
}


bool check_sequence(string& username, uint32_t cur_session, uint32_t sequence) {
    {
        unique_lock<mutex> session_l(session_lock);
        // check session number exist
        if (session <= cur_session) {
            return false;
        }
    }

    {
        unique_lock<mutex> session_map_l(session_map_lock);
        // if username does not match, drop request without using sequence number
        if(session_map[cur_session].first != username) {
            return false;
        }
        // if sequence not greater than the current greatest sequence, invalid 
        if(session_map[cur_session].second >= sequence) {
            return false;
        }
        // use up sequence, no matter the request is errorous or not
        session_map[cur_session].second = sequence;
    }
    return true;
}

bool has_null(const char* str, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (str[i] == '\0') return true;
    }
    return false;
}


bool send_back(int connectionfd, string& username, parser::Request& vals, string& response) {

    // Encrypt and send response
    // Original response will be well formed
    
    // 1. Encrypt
    int response_size = response.size();
    // {
    //     unique_lock<mutex> cout_l(cout_lock);
    //     cout << "Send_back: Original response size is " << response_size << ". \n";
    // }
    char raw[2*response_size + 64];
    int encrypt_size = fs_encrypt(users[username].c_str(), response.c_str(), response_size, raw);
    if(encrypt_size < 0) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Send_back Error: encryption failure. \n";
        // }
        return false;
    }

    // 2. Send Cleartext
    string cleartext = to_string(encrypt_size);
    if(send(connectionfd, cleartext.c_str(), cleartext.size() + 1, MSG_NOSIGNAL) < 0) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Send_back Error: Response cleartext not sucessfully sent. \n";
        // }
        return false;
    };
    
    // 3. Send Response
    if(send(connectionfd, &raw, encrypt_size, MSG_NOSIGNAL) < 0) {
        // {
        //     unique_lock<mutex> cout_l(cout_lock);
        //     cout << "Send_back Error: Response not sucessfully sent. \n";
        // }
        return false;
    };
    return true;
}



// File server main function
int main(int argc, char *argv[]) {
    if(argc != 1 && argc != 2) {
        cout << "Main Error: Wrong number of arguments. \n";
        exit(1);
    }
    // If port is specified, overwrite server_port
    if(argc == 2) {
        server_port = atoi(argv[1]);
    }
    cout << "Main: The input port is " << server_port << ". \n";

    // read in command line 
    cout << "Main: Enter usernames and passwords: \n";
    for (string line; getline(cin, line);) {
        parser::add_user(line, users);
    }    
    cout << "\n";

    // Debug - Check the user and password match
    cout << "=== CHECK USER & PASSWORD === \n";
    for(auto& t: users) {
        cout << "user is " << t.first << " and password is " << t.second << "\n";
    }

    // Read exisiting file system, starting from root (push in used)
    traverse(0);
    
    // Update block availibility - Push blocks that not in used to aval
    for(unsigned int i = 1; i < FS_DISKSIZE; ++i) {
        if(!used_block.count(i)) avail_block.push(i);
    }
    // cout << "At time of initialization, number of unused block is: " << avail_block.size() << "\n";

    // (1) Create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		cout << "Main: Error in creating socket. \n";
		exit(1);
	}

	// (2) Set the "reuse port" socket option
	int yesval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yesval, sizeof(yesval)) == -1) {
    	cout << "Main: Error setting socket options. \n";
    	exit(1);
    }

    // (3a) Create a sockaddr_in struct for the proper port and bind() to it.
	struct sockaddr_in addr, binded_addr, cli_addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(server_port);
	if(bind(sockfd, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
		cout << "Main: Error on binding. \n";
		exit(1);
	}

    // (3b) Print the binded port
    bzero(&binded_addr, sizeof(binded_addr));
	unsigned int len = sizeof(binded_addr);
	getsockname(sockfd, (struct sockaddr *) &binded_addr, &len);
	server_port = ntohs(binded_addr.sin_port);
    std::cout << "\n@@@ port " << server_port << std::endl;


	// (4) Begin listening for incoming connections.
	listen(sockfd, 30);
    

    // (5) Take in request and handle each in a new thread
    while(1) {
        socklen_t addr_len = sizeof(cli_addr);
		int connectionfd = accept(sockfd, (struct sockaddr *) & addr, &addr_len);
		if (connectionfd == -1) {
			perror("Main: Error accepting connection. \n");
			exit(1);
		}
        boost::thread StartThread(handle_request, connectionfd);
        StartThread.detach();
    }

    // Should not exit the first thread
    return 0;
}