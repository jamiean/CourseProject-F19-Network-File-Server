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

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];

    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user1", "password1", &session, seq++);
    fs_create("user1", "password1", session, seq++, "/dir", 'd');
    fs_create("user1", "password1", session, seq++, "/dir/file1", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file2", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file3", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file4", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file5", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file6", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file7", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file8", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/dir2", 'd');
    // string temp = "/dir/file";
    // for(int i = 1; i <= 8; ++i) {
    //     string to_delete = temp + to_string(i);
    //     fs_delete("user1", "password1", session, seq++, to_delete);
    // }
    fs_writeblock("user1", "password1", session, seq++, "/dir/file1", 0, writedata);
    fs_delete("user1", "password1", session, seq++, "/dir/file1");
    fs_delete("user1", "password1", session, seq++, "/dir/file2");
    fs_delete("user1", "password1", session, seq++, "/dir/file3");
    fs_delete("user1", "password1", session, seq++, "/dir/file4");
    fs_delete("user1", "password1", session, seq++, "/dir/file5");
    fs_delete("user1", "password1", session, seq++, "/dir/file6");
    fs_delete("user1", "password1", session, seq++, "/dir/file7");
    fs_delete("user1", "password1", session, seq++, "/dir/file8");
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 0, readdata);
    cout << "The read data is: \n";
    for(unsigned int i = 0; i < FS_BLOCKSIZE; ++i) {
        cout << readdata[i];
    }
    cout << "\n";
}
