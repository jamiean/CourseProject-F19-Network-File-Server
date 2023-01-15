#include <iostream>
#include "parser.h"

using namespace std;
using namespace parser;

void print_request(Request r) {
    switch (r.request)
    {
    case 1:
        cout << "FS_SESSION " ;
        break;
    case 2:
        cout << "FS_READBLOCK " ;
        break;

    case 3:
        cout << "FS_WRITEBLOCK " ;
        break;

    case 4:
        cout << "FS_CREATEBLOCK " ;
        break;

    case 5:
        cout << "FS_DELETEBLOCK " ;
        break;
    
    default:
        cout << "UNDEF ";
        break;
    }

    cout <<  "session=" << r.session << " sequence=" << r.sequence << " type=" << r.type << " block=" << r.block << " data=" << r.body << " path:";
    for (const string& s : r.filenames) {
        cout << "/" << s;
    }
    cout << endl << endl;
}

int main() {
    string request1("FS_SESSION 0 20\0", 16);
    string request1_1("FS_SESSION 0 -20\0", 17);
    string request1_2("FS_SESSION 0 -20 \0", 18);
    string request1_3("FS_SESSION 3 20\0", 16);
    string request1_4("FS_SESSION 0 20");
    string request2("afjwafjh890[239u9ph\0ibuyb 9pyuaf9pua[fou2a3io832hjrphifahsdns\0 12p98eyp9rniunfalafn", 83);
    string request3("FS_CREATE 22 4113213 /hello/world f\0", 36);
    string request3_1("FS_CREATE 22 4113213 /hello/world f0\0", 37);
    string request3_2("FS_CREATE 22 4113213 /hello/world f shi\0", 40);
    string request3_3("FS_CREATE 22 4113213 /hello/wor\0ld f0\0", 38);
    string request4("FS_READBLOCK 114514 2132 /helloword 2\0", 38);
    string request4_1("FS_READBLOCK 114514 2132 2\0", 27);
    string request4_2("FS_READBLOCK -14514 2132 /helloword 2\0", 38);
    string request4_3("FS_READBLOCK 114514 2132 / 2\0", 29);
    string request6("FS_WRITEBLOCK 22 4113213 /hello/world 10\0crtjykulihjokpiunbvch jknijohuiyuvgjhk nuhilybjkhblikjhlnibhjkiyhkjn;ohbilhunbln;hubilhkuhibhknubhjknubilhkj nbhkjnblihjn;hubilhknbilhjknbhjknbhkjnltes34w5e46r7t8yui9kopmj khgjvfcrtdftguyunjhbjgvhjknlmk gvfchykudnlewflweflijweilfjiwjflewijlfiejfelefskdmfklmwelifwelifjwelijfweif0wjfiwwecrtjykulihjokpiunbvch jknijohuiyuvgjhk nuhilybjkhblikjhlnibhjkiyhkjn;ohbilhunbln;hubilhkuhibhknubhjknubilhkj nbhkjnblihjn;hubilhknbilhjknbhjknbhkjnltes34w5e46r7t8yui9kopmj khgjvfcrtdftguyunjhbjgvhjknlmk gvfchykudnlewflweflij2", 53);
    string request6_1("FS_WRITEBLOCK 22 4113213 /hello/world 10 \0this is data", 54);
    string request6_2("FS_WRITEBLOCK 22 4113213 /hello/world -1\0this is data", 53);
    string request6_3("FS_WRITEBLOCK 22 4113213 hello/world 10\0this is data", 52);
    string request6_4("FS_WRITEBLOCK 22 4113213 /thisistooooloofefefefefeooooooooooooooooooooooooooooowwwwwwwwoooong/world 10\0this is data", 115);
    string request6_5("FS_WRITEBLOCK 22 4113213 /hello/wo rld 10\0this is data", 54);
    string request7_1("\0", 1);

    cout << endl << "###### testing CREATE #####" << endl;
    Request r = parse_request(request3);
    print_request(r);

    r = parse_request(request3_1);
    print_request(r);

    r = parse_request(request3_2);
    print_request(r);

    r = parse_request(request3_3);
    print_request(r);

    cout << endl << "###### testing SESSION #####" << endl;
    r = parse_request(request1);
    print_request(r);

    r = parse_request(request1_1);
    print_request(r);

    r = parse_request(request1_2);
    print_request(r);

    r = parse_request(request1_3);
    print_request(r);

    r = parse_request(request1_4);
    print_request(r);

    cout << endl << "###### testing WRITEBLOCK #####" << endl;
    r = parse_request(request6);
    print_request(r);

    r = parse_request(request6_1);
    print_request(r);

    r = parse_request(request6_2);
    print_request(r);

    r = parse_request(request6_3);
    print_request(r);

     r = parse_request(request6_4);
    print_request(r);

    r = parse_request(request6_5);
    print_request(r);

    cout << endl << "###### testing READBLOCK #####" << endl;
    r = parse_request(request4);
    print_request(r);

    r = parse_request(request4_1);
    print_request(r);

    r = parse_request(request4_2);
    print_request(r);

    r = parse_request(request4_3);
    print_request(r);

    cout << endl << "###### testing GARBAGE #####" << endl;
    r = parse_request(request7_1);
    print_request(r);


    return 0;
}