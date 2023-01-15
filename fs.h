/*
 * fs.h
 *
 * Header file for the server application.
 */

#ifndef _FS_H_
#define _FS_H_

#include "fs_server.h"
#include "fs_param.h"
#include "parser.h"

#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>		// close()
#include <iostream>
#include <netdb.h>		// gethostbyname(), struct hostent
#include <time.h>       /* time_t, struct tm, difftime, time, mktime */
#include <iomanip>
#include <algorithm>
#include <boost/thread.hpp>
#include <unordered_set>
#include <queue>

using namespace std;

//static variables
static const unsigned int FS_MAX_DIR = FS_BLOCKSIZE / sizeof(fs_direntry);

// Global variables
unordered_map<string, string> users;  // <Username, Password> pairs
int server_port = 0;
uint32_t session = 0;
unordered_map<uint32_t, pair<string,uint32_t>> session_map;

unordered_set<int> used_block;   // Disk Used Block
queue<int> avail_block;          // Disk Available Block
boost::mutex used_block_lock;
// boost::mutex avail_block_lock;
boost::mutex session_lock;
boost::mutex session_map_lock;
boost::mutex socket_lock;


// TODO: protext interleaving in cout
extern boost::mutex cout_lock;

// Structure of interneleaving R/W
// int -- inode disk block number
// shared_mutex -- the mutex get shared
unordered_map<int, boost::shared_mutex> inode_locks;

// File server functions

/*
* Traverse root to get the beginning directory information
*/
extern void traverse(int block);

/*
* Traverse from to get to the second-last level
*/
extern bool check_target(fs_inode& target, int& target_block, unsigned int level, parser::Request& request, string& username, bool use_writelock);

/*
* Given a connection, process and send back request
*/
extern void handle_request(int connectionfd);

/*
* look for first null terminated string, return 1 on sucess, -1 on failure
*/
extern int receive_cleartext(int connectionfd, string& res);

/*
* process a session request, return true on success
*/
extern bool process_session(parser::Request& vals, string& res, string& username);

/*
* process a read request, return true on success
*/
extern bool process_read(parser::Request& vals, string& res, string& username);

/*
* process a write request, return true on success
*/
extern bool process_write(parser::Request& vals, string& res, string& username);

/*
* process a create request, return true on success
*/
extern bool process_create(parser::Request& vals, string& res, string& username);

/*
* process a delete request, return true on success
*/
extern bool process_delete(parser::Request& vals, string& res, string& username);

/*
* encrypt, send back response, and data
*/
extern bool send_back(int connectionfd, string& username, parser::Request& vals, string& response);

/*
* check session and sequence validity
*/
extern bool check_sequence(string& username, uint32_t session, uint32_t sequence);

#endif /* _FS_H_ */