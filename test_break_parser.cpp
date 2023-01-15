#include <iostream>
#include <cstdlib>
#include "fs_client.h"
#include "fs_crypt.h"

using std::cout;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0, session2;


    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // SESSION
    // wrong password
    
}
