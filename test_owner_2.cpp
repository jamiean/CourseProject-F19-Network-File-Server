#include <iostream>
#include <cstdlib>
#include <string>
#include <strings.h>
#include "fs_client.h"
#include <cstring>

using namespace std;

int main(int argc, char *argv[])

{
    char *server;
    int server_port;
    unsigned int session, seq=0, seq2 = 0, session2;

    const char *writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata2 = "We hold these truths to be self-evid53453434er546t78jiouyvyctersdtyuhnjowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata3 = "We hold these truths to be sel323255egsergsdfgsrger546t78jiouyvyctersdtyuhnjowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata4 = "We hold these truths to be self-evident, that asegsergsdfgsrger546t78jiouyvyctersdtyuhnjowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    const char *writedata5 = "We hold these truths to be self-evidentgeegergsergsdfgsrger546t78jiouyvyctersdtyuhnjowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];

    if (argc != 3) {
        cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    fs_clientinit(server, server_port);
    fs_session("user1", "password1", &session, seq++);
    fs_session("user2", "password2", &session2, seq2++);
    fs_create("user1", "password1", session, seq++, "/a1", 'd');
    fs_create("user1", "password1", session, seq++, "/f1", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f1", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b2", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f2", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b3", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f3", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b4", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f4", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b5", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f5", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b6", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f6", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b7", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f7", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b8", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f8", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b9", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/f9", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1/f1", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1", 'd');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/f1", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 'f');
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 'f');
    // write files in d1

    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 1, writedata3);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 2, writedata4);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 3, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 0, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 1, writedata3);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 2, writedata4);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 3, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 4, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 5, writedata5);

    fs_writeblock("user1", "password1", session, seq++, "/a1/f1", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f1", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f2", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f2", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f3", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f3", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f4", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f4", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f5", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f5", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f6", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f7", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f8", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f8", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f9", 0, writedata5);
    fs_writeblock("user1", "password1", session, seq++, "/a1/f9", 1, writedata2);
    
    // write files in c1
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/f1", 0, writedata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 5, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2", 1, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 3, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 2, readdata);

    // write files in b1
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/f1", 0, writedata);

    // write files in a1
    fs_writeblock("user1", "password1", session, seq++, "/a1/f1", 0, writedata);

    // write files in root
    fs_writeblock("user1", "password1", session, seq++, "/f1", 0, writedata);


    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1");
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, readdata);
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2");
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/f1", 0, readdata);
    fs_create("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 'f');
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, writedata);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 1, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 2, writedata2);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 3, writedata3);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 4, writedata4);
    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 5, writedata5);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 4, readdata);
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1");
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, readdata);
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/d1/f2");
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/f1", 0, readdata);
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/d1");
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1/f1");
    fs_delete("user1", "password1", session, seq++, "/a1/b1/c1");
    fs_delete("user1", "password1", session, seq++, "/a1/b1/f1");
    fs_delete("user1", "password1", session, seq++, "/a1/b5");
    fs_delete("user1", "password1", session, seq++, "/a1/f5");
    fs_delete("user1", "password1", session, seq++, "/a1/b6");
    fs_delete("user1", "password1", session, seq++, "/a1/f6");
    fs_delete("user1", "password1", session, seq++, "/a1/b7");
    fs_delete("user1", "password1", session, seq++, "/a1/f7");
    fs_delete("user1", "password1", session, seq++, "/a1/b8");
    fs_delete("user1", "password1", session, seq++, "/a1/f8");
    fs_delete("user1", "password1", session, seq++, "/a1/b1");
    fs_delete("user1", "password1", session, seq++, "/a1/f1");
    fs_delete("user1", "password1", session, seq++, "/a1/b2");
    fs_delete("user1", "password1", session, seq++, "/a1/f2");
    fs_delete("user1", "password1", session, seq++, "/a1/b3");
    fs_delete("user1", "password1", session, seq++, "/a1/f3");
    fs_delete("user1", "password1", session, seq++, "/a1/b4");
    fs_delete("user1", "password1", session, seq++, "/a1/f4");

    fs_delete("user1", "password1", session, seq++, "/a1/b9");
    fs_delete("user1", "password1", session, seq++, "/a1/f9");
    fs_delete("user1", "password1", session, seq++, "/a1");
    fs_delete("user1", "password1", session, seq++, "/f1");

    fs_create("user2", "password2", session2, seq2++, "/a1", 'd');
    fs_create("user2", "password2", session2, seq2++, "/a1/b1", 'd');
    fs_create("user2", "password2", session2, seq2++, "/a1/f1", 'f');
    fs_create("user2", "password2", session2, seq2++, "/a1/b1/c1", 'd');
    fs_create("user2", "password2", session2, seq2++, "/a1/b1/c1/d1", 'd');
    fs_create("user2", "password2", session2, seq2++, "/a1/b1/c1/d1/f1", 'f');

    fs_writeblock("user2", "password2", session2, seq2++, "/a1/f1", 0, writedata2);


    fs_writeblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, writedata);
    fs_writeblock("user2", "password2", session2, seq2++, "/a1/b1/c1/d1/f1", 0, writedata);
    fs_writeblock("user2", "password2", session2, seq2++, "/a1/b1/c1/d1", 0, writedata);
    fs_writeblock("user2", "password2", session2, seq2++, "/a2/b2/c2/d2/f2", 0, writedata2);
    fs_writeblock("user2", "password2", session2, seq2++, "/a2/b2/c2/d2", 0, writedata2);
    fs_writeblock("user2", "password2", session2, seq2++, "/", 0, writedata2);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1/f1", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a1/b1/c1/d1", 0, readdata);
    fs_readblock("user1", "password1", session, seq++, "/a2/b2/c2/d2/f2", 0, readdata);


    fs_delete("user2", "password2", session2, seq2++, "/a1/b1/c1/d1/f1");
    fs_delete("user2", "password2", session2, seq2++, "/a1/b1/c1/d1");
    fs_delete("user2", "password2", session2, seq2++, "/a1/b1/c1");
    fs_delete("user2", "password2", session2, seq2++, "/a1/b1");
    fs_delete("user2", "password2", session2, seq2++, "/a1/f1");
    fs_delete("user2", "password2", session2, seq2++, "/a1");


}
