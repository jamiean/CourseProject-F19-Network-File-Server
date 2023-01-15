#include "parser.h"
#include <unordered_map>
// #include <boost/filesystem.hpp>

using namespace std;

// extern unordered_map<string, string> users;

enum StringValue {  FS_UNDEFINED,
                    FS_SESSION,
                    FS_READBLOCK,
                    FS_WRITEBLOCK,
                    FS_CREATE,
                    FS_DELETE};

unordered_map<string, StringValue> request_map = { {"FS_SESSION", FS_SESSION}, 
                                                    {"FS_READBLOCK", FS_READBLOCK},
                                                    {"FS_WRITEBLOCK", FS_WRITEBLOCK},
                                                    {"FS_CREATE", FS_CREATE},
                                                    {"FS_DELETE", FS_DELETE}};

void parser::add_user(string& input, unordered_map<string, string>& users) {
    string name, word;
    istringstream iss(input);
    iss >> name >> word;
    // cout << "add user name: " << name << " word: " << word << endl;

    users[name] = word; 
}

int parser::parse_cleartext(string&input, string& username, int& size, unordered_map<string, string>& users) {
    // input has no 
    istringstream iss(input);
    iss >> username >> size;

    string parsed_request = username + ' ' + to_string(size);
    // cout << "Parser: parsed request is: " << parsed_request << " parsed size is: " << parsed_request.size() << " input size is: " << input.size() << endl;
    if (parsed_request.compare(input))   return -1;
    // cout << "Parser: parse clear text username: " << username << " size: " << size << ". \n";
    if (users.count(username) && size >= 0 && size <= 1424 && username.length() <= FS_MAXUSERNAME && iss.tellg() == -1)    return 1;
    return -1;
}

bool is_valid_path(string& path) {
    // check if it is valid path
    if (path.length() > FS_MAXPATHNAME || path.length() == 0)    return false;
    if (path[0] != '/') return false;
    if (path[path.size() - 1] == '/')   return false;
    if (path.find(' ') != path.npos || path.find("//") != path.npos)    return false;
    return true;
}

bool is_valid_block(long block) {
    return (block <= FS_MAXFILEBLOCKS && block >= 0);
}

bool parser::parse_path(string& path, vector<string>& filenames) {
    // discard the first '\'
    // invariant: path is non-empty
    istringstream iss(path.substr(1));
    string file;

    while (getline(iss, file, '/')) {
        if (file.length() > 0 && file.length() <= FS_MAXFILENAME)  filenames.push_back(file);
        else    return false;
    }
    return true;
}

parser::Request parser::parse_request(string& input) {
    Request request;
    string req;
    string filetype;
    string filepath;
    long sess = -1, seq = -1, blk = -1;

    // cout << "Parser: Parser request input is: " << input << ". \n";
    // cout << "Parser: Input length " << input.length() <<endl;
    size_t null_pos = input.find('\0');
    // cout << "nullpos" << null_pos << endl;
    if (null_pos == input.npos) return request;
    string request_str = input.substr(0, null_pos);
    // cout << "Parser: Temp length " << request_str.length() << endl;
    string body = input.substr(null_pos + 1);
    request.body = body;

    istringstream iss(request_str);
    iss >> req;
    if (!request_map.count(req))    return request;

    switch (request_map[req]) {
        case FS_SESSION:
            iss >> sess >> seq;
            // valid if: 1) session == 0, 2) swquence >= 0 (no validity check)
            if (sess == 0 && seq >= 0 && iss.tellg() == -1) {
                string parsed_request = string("FS_SESSION ") + to_string(sess) + ' ' + to_string(seq) + '\0';
                if (parsed_request.compare(input))    break;
                request.session = sess;
                request.sequence = seq;
                request.request = 1;
            }
            break;
        
        case FS_READBLOCK:
            iss >> sess >> seq >> filepath >> blk;
            if (sess >= 0 && seq >= 0 && is_valid_block(blk) && is_valid_path(filepath) && iss.tellg() == -1) {
                // check if the request are exactly the same
                string parsed_request = string("FS_READBLOCK ") + to_string(sess) + ' ' + to_string(seq) + ' ' + filepath + ' ' + to_string(blk) + '\0';
                if (parsed_request.compare(input))   break;
                request.session = sess;
                request.sequence = seq;
                request.block = blk;
                if (parse_path(filepath, request.filenames))    request.request = 2;                
            }
            break;

        case FS_WRITEBLOCK:
            iss >> sess >> seq >> filepath >> blk;
            if (sess >= 0 && seq >= 0 && is_valid_block(blk) && is_valid_path(filepath) && iss.tellg() == -1) { 
                string parsed_request = string("FS_WRITEBLOCK ") + to_string(sess) + ' ' + to_string(seq) + ' ' + filepath + ' ' + to_string(blk) + '\0' + body;
                if (parsed_request.compare(input))    break;
                // cout << "Parser: the data size=" << body.size() << endl;
                if (body.size() != FS_BLOCKSIZE)    break;
                request.session = sess;
                request.sequence = seq;
                request.block = blk;
                // success if every filename is valid
                if (parse_path(filepath, request.filenames))    request.request = 3;
            }
            break;

        case FS_CREATE:
            iss >> sess >> seq >> filepath >> filetype;

            // sanity check: valid session/sequence number; don't check pathname
            if (seq >= 0 && seq >= 0 && is_valid_path(filepath) && iss.tellg() == -1) {
                string parsed_request = string("FS_CREATE ") + to_string(sess) + ' ' + to_string(seq) + ' ' + filepath + ' ' + filetype + '\0';
                if (parsed_request.compare(input))    break;
                request.session = sess;
                request.sequence = seq;
                if (parse_path(filepath, request.filenames))    request.request = 4;
                if (filetype == "f") request.type = true;
                else if (filetype == "d")   request.type = false;
                else    request.request = 0;
            }
            break;

        case FS_DELETE:
            iss >> sess >> seq >> filepath;
            if (sess >= 0 && seq >= 0 && is_valid_path(filepath) && iss.tellg() == -1) {
                string parsed_request = string("FS_DELETE ") + to_string(sess) + ' ' + to_string(seq) + ' ' + filepath + '\0';
                if (parsed_request.compare(input))    break;
                request.session = sess;
                request.sequence = seq;
                if (parse_path(filepath, request.filenames))    request.request = 5;
            }
            break;

        case FS_UNDEFINED:
            break;
    }
    return request;
}