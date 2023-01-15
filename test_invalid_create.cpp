#include <iostream>
#include <cstdlib>
#include "fs_client.h"

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
    fs_session("user1", "wrongpwd", &session, seq++);
    // no user
    fs_session("wronguser", "password1", &session, seq++);
    fs_session("wronguserlong", "password1", &session, seq++);


    fs_session("user1", "password1", &session, seq++);
    fs_session("user2", "password2", &session2, seq++);

    // CREATE
    // wrong pwd, user, session num
    fs_create("wronguser", "password1", session, seq++, "/dir", 'd');
    fs_create("user1", "wrongpwd1", session, seq++, "/dir", 'd');
    fs_create("usertoooooooolong1", "wrongpwd1", session, seq++, "/dir", 'd');
    fs_create("user2", "password2", session, seq++, "/dir", 'd');
    cout << "Create test done\n";

    // wrong dir / type
    fs_create("user1", "password1", session, seq++, "/di r", 'd');
    fs_create("user1", "password1", session, seq++, "dir/file", 'd');
    fs_create("user1", "password1", session, seq++, "dir//file", 'd');
    fs_create("user1", "password1", session, seq++, "/dir", 'a');
    fs_create("user1", "password1", session, seq++, "/non/exist", 'd');
    fs_create("user1", "password1", session, seq++, "/dir/fil\0e", 'f');

    // wrong seq
    fs_create("user1", "password1", session, seq - 1, "/dir", 'd');
    fs_create("user1", "password1", session, seq++, "/dir", 'd');
    fs_create("user1", "password1", session, seq++, "/fail/atcreate", 'd');

    fs_create("user1", "password1", session, seq++, "/dir/file", 'f');
}
