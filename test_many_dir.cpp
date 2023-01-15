#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "fs_client.h"

using std::cout;
using namespace std;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0, session2;

    // const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    // char readdata[FS_BLOCKSIZE];

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
    string base("/dir/subdir-");
    for (int i = 0; i < 50; i++) {
        string s = base + to_string(i);
        fs_create("user1", "password1", session, seq++, s.c_str(), 'd');
        // s += "/file";
        // fs_create("user1", "password1", session, seq++, s.c_str(), 'f');
    }
    // usleep(10000000);


    // for (int i = 0; i < 50; i++) {
    //     string s = base + to_string(i) + "/file";
    //     fs_delete("user1", "password1", session, seq++, s.c_str());
    // }

    for (int i = 0; i < 50; i++) {
        string s = base + to_string(i);
        // if (i <= 34) usleep(5000000);
        fs_delete("user1", "password1", session, seq++, s.c_str());
    }
}
