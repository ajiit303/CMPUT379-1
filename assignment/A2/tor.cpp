#include <assert.h>
#include <cstring>
#include <string>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "tor.h"
#include "utils.h"

using namespace std;


PacketSwitch::PacketSwitch ( int switchNum, int prev, int next, int ipLow, int ipHigh,
        string filename ) {
    assert( switchNum > 0 && switchNum <= MAX_NSW );
    assert( prev <= MAX_NSW && prev != 0 );
    assert( next <= MAX_NSW && next != 0 );
    assert( ipLow >= 0 && ipLow < ipHigh );
    assert( ipHigh <= MAXIP );

    this->switchNum = switchNum;
    this->prev = prev;
    this->next = next;
    this->ipLow = ipLow;
    this->ipHigh = ipHigh;
    this->filename = filename;

    this->ftable.push_back( Rule(0, MAXIP, this->ipLow, this->ipHigh, FORWARD) );
}

void PacketSwitch::run() {
    fifos[0][0] = STDIN_FILENO;
    fifos[0][1] = STDOUT_FILENO;

    pfds[0].fd = STDIN_FILENO;
    pfds[0].events = POLLIN;
    pfds[0].revents = 0;

    mkopen( 0, switchNum, fifos[1][0], fifos[1][1] );

    pfds[1].fd = fifos[1][0];
    pfds[1].events = POLLIN;
    pfds[1].revents = 0;

    if ( prev != -1 ) {
        mkopen( switchNum, prev, fifos[2][0], fifos[2][1] );
        pfds[2].fd = fifos[2][0];
        pfds[2].events = POLLIN;
        pfds[2].revents = 0;
    }

    if ( next != -1 ) {
        mkopen( next, switchNum, fifos[3][0], fifos[3][1] );
        pfds[3].fd = fifos[3][0];
        pfds[3].events = POLLIN;
        pfds[3].revents = 0;
    }
    
    int in = 0;
    int rval = 0;
    int len = 0;

    char buf[MAXBUF];
    memset( buf, 0, sizeof(buf) );

    strcpy( buf, "hello" );

    write( fifos[1][1], buf, sizeof(buf) );

    while (1) {
        rval = poll( pfds, MAXPORT + 1, 0 );
    }
}

