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


    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user1", "password1", &session, seq++);
    fs_create("user1", "password1", session, seq++, "/dir", 'd');

    string temp = "/dir/dir";
    for(int i = 0; i <= 124 * 8; ++i) {
        string to_create = temp + to_string(i);
        const char *create = to_create.c_str();
        fs_create("user1", "password1", session, seq++, create, 'd');
    }


    fs_delete("user1", "password1", session, seq++, "/dir/dir2");
    fs_create("user1", "password1", session, seq++, "/dir/mytest", 'd');

}
