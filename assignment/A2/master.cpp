#include <assert.h>
#include <cstring>
#include <iostream>
#include <vector>

#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "master.h"
#include "tor.h"
#include "utils.h"

using namespace std;


MasterSwitch::MasterSwitch (int numSwitch) {
    assert( numSwitch > 0 && numSwitch <= MAX_NSW );

    memset( fifos, 0, sizeof(fifos) );
    this->numSwitch = numSwitch;
}

void MasterSwitch::run () {
    fifos[0][0] = STDIN_FILENO;
    fifos[0][1] = STDOUT_FILENO;

    pfds[0].fd = STDIN_FILENO;
    pfds[0].events = POLLIN;
    pfds[0].revents = 0;

    for ( int i = 1; i <= numSwitch; i++ ) {
        mkopen( i, 0, fifos[i][0], fifos[i][1] );
    }

    for ( int i = 1; i <= numSwitch; i++ ) {
        pfds[i].fd = fifos[i][0];
        pfds[i].events = POLLIN;
        pfds[i].revents = 0;
    }

    int in = 0;
    int len = 0;
    int rval = 0;
    
    char buf[MAXBUF];
    memset( buf, 0, sizeof(buf) );

    while (1) {
        rval = poll( pfds, MAX_NSW + 1, 0 );
        for ( int in = 0; in < MAX_NSW + 1; in++ ) {
            if ( pfds[in].revents && POLLIN ) {
                len = read( fifos[in][0], buf, MAXBUF );
                for ( int i = 0; i < len; i++ ) 
                    cout << buf[i];
                cout << endl;
            }
        }
    }
}

