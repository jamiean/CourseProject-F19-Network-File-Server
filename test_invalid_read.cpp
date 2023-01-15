#include <iostream>
#include <cstdlib>
#include "fs_client.h"

using std::cout;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0, session2, trash;

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
    fs_session("user4", "password4", &trash, seq++);
    fs_session("user4", "password4", &trash, seq++);
    fs_session("user2", "password2", &session2, seq++);

    // CREATE
    fs_create("user1", "password1", session, seq++, "/dir", 'd');
    fs_create("user1", "password1", session, seq++, "//dir/file1", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file1", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/file2", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful", 'd');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/lethisoneistooooooooooooooooooooooooooooooofitoverflowsornot", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/and this one contains whitespace", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andthisonecontainsillegal\0yay", 'f');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andletstrytooverflowthetotalpathlengthherewegohahahahah", 'd');
    fs_create("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andletstrytooverflowthetotalpathlengthherewegohahahahah/imangyssomivehongkongjournalist", 'd');
    fs_writeblock("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/and", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andthisonecontainsillegal", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andletstrytooverflowthetotalpathlengthherewegohahahahah/imangyssomivehongkongjournalist", 0, writedata);

    // WRITEBACK
    fs_writeblock("user1", "password1", session, seq++, "/dir/file1", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/dir/file1", 1, writedata);
    fs_writeblock("user1", "wrong pwd", session, seq++, "/dir/file1", 2, writedata); // invalid


    // READBLOCK
    // wrong pwd, user, session num
    fs_readblock("user1", "wrongpwd1", session, seq++, "/dir/file1", 0, readdata); // invalid
    fs_readblock("wronguser", "password1", session, seq++, "/dir/file1", 0, readdata); // invalid
    fs_readblock("user3", "password3", session, seq++, "/dir/file1", 0, readdata); // invalid
    fs_readblock("user2", "password2", session2, seq++, "/dir/file1", 0, readdata); // invalid

    // wrong dir / block
    fs_readblock("user1", "password1", session, seq++, "dir/file1", 0, readdata); // invalid
    fs_readblock("user1", "password1", session, seq++, "/non/exist", 0, readdata); // invalid
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", -1, readdata); // invalid
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 255, readdata); // invalid
    fs_readblock("user1", "password1", session, seq++, "/dir/file1/", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "//dir/file1/", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 1, readdata);
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 2, readdata);
    fs_readblock("user1", "password1", session, seq++, "/dir/file2", 0, readdata);

    // wrong seq
    fs_readblock("user1", "password1", session, 3, "/dir/file1", 2, readdata); // invalid
    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 2, readdata);

    fs_readblock("user1", "password1", session, seq++, "/dir/file1", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/dir", 0, readdata);

    fs_delete("user1", "password1", session, seq++, "/dir/file1");
    fs_delete("user2", "password2", session2, seq++, "/dir/file2");
    fs_delete("user3", "password3", 123, seq++, "/dir/file2");
    fs_delete("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andletstrytooverflowthetotalpathlengthherewegohahahahah");
    fs_delete("user1", "password1", session, seq++, "/dir/file2");
    fs_delete("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful/andthisonecontainsillegal");
    fs_delete("user1", "password1", session, seq++, "/dir/ihavetosaythisprojectissooooopainful");
    fs_delete("user1", "password1", session, seq++, "/dir");
}
