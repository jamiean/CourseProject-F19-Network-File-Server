#include <iostream>
#include <cstdlib>
#include "fs_client.h"

using std::cout;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0, session2;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    const char *faildata = "failed at WRITE block";

    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);

    // SESSION
    fs_session("user1", "password1", &session, seq++);
    fs_session("user2", "password2", &session2, seq++);

    // CREATE
    fs_create("user1", "password1", session, seq++, "/dir", 'd');
    fs_create("user1", "password1", session, seq++, "/dir/file", 'f');

    // WRITEBACK
    // wrong pwd, user, session num
    fs_writeblock("wronguser", "password1", session, seq++, "/dir/file", 0, faildata);
    fs_writeblock("usernametoooolong", "password1", session, seq++, "/dir/file", 0, faildata);
    fs_writeblock("user1", "wrongpwd1", session, seq++, "/dir/file", 0, faildata);
    fs_writeblock("user2", "password2", session, seq++, "/dir/file", 0, faildata);
    fs_writeblock("user2", "password2", session2, seq++, "/dir/file", 0, faildata);

    // wrong dir / block
    fs_writeblock("user1", "password1", session, seq++, "dir/file", 0, faildata);
    fs_writeblock("user1", "password1", session, seq++, "/non/exist", 0, faildata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/ff ile", 0, faildata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/file", -1, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/file", 255, writedata);

    // wrong seq
    fs_writeblock("user1", "password1", session, seq - 1, "/dir/file", 0, faildata);

    fs_writeblock("user1", "password1", session, seq++, "/dir", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/file", 0, writedata);


    
}
