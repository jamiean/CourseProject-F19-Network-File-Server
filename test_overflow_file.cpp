#include <iostream>
#include <cstdlib>
#include <string>
#include "fs_client.h"

using namespace std;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0;
    const char *writedata = "We\0";


    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user3", "password3", &session, seq++);
    fs_create("user3", "password3", session, seq++, "/dir3", 'f');

    for(int i = 0; i <= 124 * 8; ++i) {
        fs_writeblock("user3", "password3", session, seq++, "/dir3", i, writedata);
    }


    // fs_writeblock("user1", "password1", session, seq++, "/dir", 10, writedata);

}
