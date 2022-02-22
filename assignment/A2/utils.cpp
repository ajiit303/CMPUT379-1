#include <cstring>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>

#include "handler.h"
#include "utils.h"

using namespace std;


int getSwitchNum (string str) {
    if ( str.compare("null") == 0 )
        return -1;

    return stoi( str.substr( string("psw").size() ) );
}

void mkopen ( int s1, int s2, int &rfd, int &wfd ) {
    mkopenr ( s1, s2, rfd );
    mkopenw ( s2, s1, wfd );
}

void mkopenr ( int s1, int s2, int &rfd ) {
    string name = "fifo-" + to_string(s1) + "-" + to_string(s2);

    cout << "making " + name + "..." << endl;
    if ( mkfifo( name.c_str(), S_IRWXU ) < 0 ) {
        if ( errno != EEXIST ) {
            fatal( "mkfifo error()" );
            exit(1);
        }
    }
    cout << name + " is made" << endl << endl;

    cout << "opening " + name + " for read..." << endl;
    if ( ( rfd = open( name.c_str(), O_RDWR ) ) < 0 ) {
        fatal( "open error() : %d", errno );
        exit(1);
    }
    cout << name + " is open" << endl << endl;
}

void mkopenw ( int s1, int s2, int &wfd ) {
    string name = "fifo-" + to_string(s1) + "-" + to_string(s2);

    cout << "making " + name + "..." << endl;
    if ( mkfifo( name.c_str(), S_IRWXU ) < 0 ) {
        if ( errno != EEXIST ) {
            fatal( "mkfifo error()" );
            exit(1);
        }
    }
    cout << name + " is made" << endl << endl;

    cout << "opening " + name + " for write..." << endl;
    if ( ( wfd = open( name.c_str(), O_RDWR ) ) < 0 ) {
        fatal( "open error() : %d", errno );
        exit(1);
    }
    cout << name + " is open" << endl << endl;
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