#include <iostream>
#include <cstdlib>
#include <string>
#include "fs_client.h"

using namespace std;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session1,session2,session3,session4 = 0;
    unsigned int seq1 = 0;
    unsigned int seq2 = 0;
    unsigned int seq3 = 0;
    unsigned int seq4 = 0;
    // const char *writedata1 = "1e hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    // const char *writedata2 = "2e hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    // const char *writedata3 = "3e hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    // const char *writedata4 = "4e hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    // char readdata[FS_BLOCKSIZE];

    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user1", "password1", &session1, seq1++);
    fs_session("user2", "password2", &session2, seq2++);
    fs_session("user3", "password3", &session3, seq3++);
    fs_session("user4", "password4", &session4, seq4++);

    fs_create("user1", "password1", session1, seq1++, "/dir1-1", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-2", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-3", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-4", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-5", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-6", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-7", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-8", 'f');
    fs_create("user1", "password1", session1, seq1++, "/dir1-9", 'f');

    fs_create("user2", "password2", session2, seq2++, "/dir2-1", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-2", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-3", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-4", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-5", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-6", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-7", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-8", 'f');
    fs_create("user2", "password2", session2, seq2++, "/dir2-9", 'f');

    fs_create("user3", "password3", session3, seq3++, "/dir3-1", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-2", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-3", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-4", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-5", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-6", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-7", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-8", 'f');
    fs_create("user3", "password3", session3, seq3++, "/dir3-9", 'f');

    fs_create("user4", "password4", session4, seq4++, "/dir4-1", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-2", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-3", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-4", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-5", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-6", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-7", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-8", 'f');
    fs_create("user4", "password4", session4, seq4++, "/dir4-9", 'f');

    fs_delete("user2", "password2", session2, seq2++, "/dir2-1");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-2");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-3");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-4");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-5");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-6");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-7");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-8");
    fs_delete("user2", "password2", session2, seq2++, "/dir2-9");

    fs_delete("user3", "password3", session3, seq3++, "/dir3-1");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-2");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-3");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-4");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-5");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-6");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-7");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-8");
    fs_delete("user3", "password3", session3, seq3++, "/dir3-9");

    fs_delete("user1", "password1", session1, seq1++, "/dir1-1");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-2");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-3");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-4");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-5");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-6");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-7");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-8");
    fs_delete("user1", "password1", session1, seq1++, "/dir1-9");

    fs_delete("user4", "password4", session4, seq4++, "/dir4-1");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-2");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-3");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-4");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-5");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-6");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-7");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-8");
    fs_delete("user4", "password4", session4, seq4++, "/dir4-9");
}
