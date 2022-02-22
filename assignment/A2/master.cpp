#include <assert.h>
#include <cstring>
#include <iostream>
#include <vector>

#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "master.h"
#include "message.h"
#include "tor.h"
#include "utils.h"

using namespace std;


MasterSwitch::MasterSwitch (int numSwitch) {
    assert( numSwitch > 0 && numSwitch <= MAX_NSW );

    memset( fifos, 0, sizeof(fifos) );
    this->numSwitch = numSwitch;
}

void MasterSwitch::initFIFO () {
    fifos[0][0] = STDIN_FILENO;
    fifos[0][1] = STDOUT_FILENO;
    setPfd( 0, STDIN_FILENO );

    for ( int i = 1; i <= numSwitch; i++ ) {
        mkopen( i, 0, fifos[i][0], fifos[i][1] );
        setPfd( i, fifos[i][0] );
    }
}

void MasterSwitch::setPfd ( int i, int rfd ) {
    pfds[i].fd = rfd;
    pfds[i].events = POLLIN;
    pfds[i].revents = 0;
}


void MasterSwitch::startPoll () {
    int in = 0;
    int len = 0;
    int rval = 0;
    
    char buf[MAXBUF];
    memset( buf, 0, sizeof(buf) );

    cout << "start polling" << endl;

    while (1) {
        rval = poll( pfds, MAXMSFD, 0 );
        for ( in = 0; in < MAXMSFD; in++ ) {
            if ( pfds[in].revents && POLLIN ) {
                if ( in == 0 ) {

                } else {
                    Frame frame = rcvFrame( fifos[in][0] );

                }
            } 
        }
    }
}
