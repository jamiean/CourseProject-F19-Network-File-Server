#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include "fs_param.h"

using namespace std;

// extern unordered_map<string, string> users;

namespace parser {

typedef struct Request {
    Request(): request(0), session(0), sequence(0), block(0){
        filenames.clear();
    }
    // 1: SESSION, 2: READBOLCK 3: WRITEBLOCK 4: CREATE 5: DELETE 0: UNDEF
    short request; 
    bool type; // true for file and false for directory
    uint32_t session; // 
    uint32_t sequence;
    uint32_t block; // could be modified to be block on disk
    // string pathname;
    string body;
    vector<string> filenames; 
} Request;

/*
* Given a line of user and password input, parse and store in users
*/
extern void add_user(string& input, unordered_map<string, string>& users);

/*
* Given <username> <size>, parse username as a string and size as a numeric value
*/
extern int parse_cleartext(string&input, string& username, int& size, unordered_map<string, string>& users);

/*
* Given a string, determine type and parse information
* return -1 for malformed input, 1 for FS_SESSION, 2 for FS_READBLOCK, 3 for FS_WRITEBLOCK ...
* results stored in a vector in order
*/
extern Request parse_request(string& input);

extern bool parse_path(string& path, vector<string>& filenames);


// And so on...

} // parser

#endif /* _PARSER_H_ */