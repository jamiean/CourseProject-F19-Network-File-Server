#include <iostream>
#include <cstdlib>
#include <string>
#include <strings.h>
#include "fs_client.h"
#include <cstring>

using namespace std;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0;

    char writedata[512];


    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user1", "password1", &session, seq++);
    fs_create("user1", "password1", session, seq++, "/a1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 'd');

    seq = 0;
    fs_session("user2", "password2", &session, seq++);
    fs_create("user2", "password2", session, seq++, "/temp1", 'd');
    fs_create("user2", "password2", session, seq++, "/temp1/temp2", 'd');
    for(int i = 0; i < 24; ++i) {
        string filename = "/temp1/temp2/file" + to_string(i);
        fs_create("user2", "password2", session, seq++, filename.c_str(), 'f');
        bzero(writedata, 512);
        memcpy(writedata, filename.c_str(), filename.size());
        fs_writeblock("user2", "password2", session, seq++, filename.c_str(), 0, writedata);
    }
    fs_delete("user2", "password2", session, seq++, "/temp1/temp2/file1");
    fs_delete("user2", "password2", session, seq++, "/temp1/temp2/file8");
    fs_delete("user2", "password2", session, seq++, "/temp1/temp2/file16");
    for(int i = 8; i < 16; ++i) {
        string filename = "/temp1/temp2/file" + to_string(i);
        fs_delete("user2", "password2", session, seq++, filename.c_str());
    }
}
