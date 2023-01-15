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
    fs_session("user3", "password3", &session, seq++);
    fs_create("user3", "password3", session, seq++, "/dir", 'd');
    fs_create("user3", "password3", session, seq++, "/door", 'd');


    fs_create("user3", "password3", session, seq++, "/dir/1", 'd');
    fs_create("user3", "password3", session, seq++, "/dir/2", 'd');
    fs_create("user3", "password3", session, seq++, "/dir/3", 'd');
    fs_create("user3", "password3", session, seq++, "/dir/4", 'd');
    fs_create("user3", "password3", session, seq++, "/dir/5", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/6", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/7", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/8", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/9", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/10", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/11", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/12", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/13", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/14", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/15", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/16", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/17", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/18", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/19", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/20", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/21", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/22", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/23", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/24", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/25", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/26", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/27", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/28", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/29", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/30", 'f');
    fs_create("user3", "password3", session, seq++, "/dir/31", 'f');

    
    fs_delete("user3", "password3", session, seq++, "/dir/9");
    fs_delete("user3", "password3", session, seq++, "/dir/10");
    fs_delete("user3", "password3", session, seq++, "/dir/11");
    fs_delete("user3", "password3", session, seq++, "/dir/12");
    fs_delete("user3", "password3", session, seq++, "/dir/13");
    fs_delete("user3", "password3", session, seq++, "/dir/14");
    fs_delete("user3", "password3", session, seq++, "/dir/15");
    fs_delete("user3", "password3", session, seq++, "/dir/16");

    fs_delete("user3", "password3", session, seq++, "/dir/1");
    fs_delete("user3", "password3", session, seq++, "/dir/2");
    fs_delete("user3", "password3", session, seq++, "/dir/3");
    fs_delete("user3", "password3", session, seq++, "/dir/4");
    fs_delete("user3", "password3", session, seq++, "/dir/5");
    fs_delete("user3", "password3", session, seq++, "/dir/6");
    fs_delete("user3", "password3", session, seq++, "/dir/7");
    

    fs_delete("user3", "password3", session, seq++, "/dir/17");
    fs_delete("user3", "password3", session, seq++, "/dir/18");
    fs_delete("user3", "password3", session, seq++, "/dir/19");
    fs_delete("user3", "password3", session, seq++, "/dir/20");
    fs_delete("user3", "password3", session, seq++, "/dir/21");
    fs_delete("user3", "password3", session, seq++, "/dir/22");
    fs_delete("user3", "password3", session, seq++, "/dir/23");
    fs_delete("user3", "password3", session, seq++, "/dir/24");
    fs_delete("user3", "password3", session, seq++, "/dir/25");
    fs_delete("user3", "password3", session, seq++, "/dir/26");
    fs_delete("user3", "password3", session, seq++, "/dir/27");
    fs_delete("user3", "password3", session, seq++, "/dir/28");
    fs_delete("user3", "password3", session, seq++, "/dir/29");
    fs_delete("user3", "password3", session, seq++, "/dir/30");
    fs_delete("user3", "password3", session, seq++, "/dir/31");

    fs_delete("user3", "password3", session, seq++, "/dir/8");

    // if it shrinks, 10 should be written in the new block id same with 9
    // if not shrink, 10 should be written in the old block id same with 1 - 8
    fs_create("user3", "password3", session, seq++, "/dir/10", 'f');

}
