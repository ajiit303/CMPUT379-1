#include <string>

#include "utils.h"

using namespace std;


int getSwitchNum (string str) {
    if ( str.compare("null") )
        return -1;

    return stoi( str.substr( string("psw").size() ) );
}

int split ( string str, string del, string token[] ) {
    int num = 0;
    int start = 0;
    int end = str.find(del);

    while (end != -1) {
        token[num++] = str.substr( start, end - start );
        start = end + del.size();
        end = str.find( del, start );
    }

    token[num++] = str.substr( start, end - start );

    return num;
}