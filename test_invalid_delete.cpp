#include <iostream>
#include <cstdlib>
#include "fs_client.h"

using std::cout;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, session2, seq=0;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];

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
    fs_writeblock("user1", "password1", session, seq++, "/dir/file", 0, writedata);


    // READBLOCK
    fs_readblock("user1", "password1", session, seq++, "/dir/file", 0, readdata);



    // DELETE
    // wrong user/pwd
    fs_delete("wronguser1", "password1", session, seq, "/dir/file");
    fs_delete("user1", "wrongpassword1", session, seq++, "/dir/file");
    fs_delete("user2", "password2", session, seq++, "/dir/file");
    fs_delete("user2", "password2", session2, seq++, "/dir/file");

    // invalid path name
    fs_delete("user1", "password1", session, seq++, "dir/file");
    fs_delete("user1", "password1", session, seq++, "/dir/ file");
    fs_delete("user1", "password1", session, seq, "/dir/nonexist");

    // replay attack
    fs_delete("user1", "password1", session, seq, "/dir/file");
    fs_delete("user1", "password1", session, seq++, "/dir/file");

    // delete non empty dir 
    fs_delete("user1", "password1", session, seq++, "/dir");
    fs_delete("user1", "password1", session, seq++, "/dir/file");
}
