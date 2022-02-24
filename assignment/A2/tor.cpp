#include <assert.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "message.h"
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

    this->ftable.push_back( Rule(0, MAXIP, this->ipLow, this->ipHigh, FORWARD, 3) );
}

int PacketSwitch::getSwitchNum () {
    return switchNum;
}

int PacketSwitch::getPrev () {
    return prev;
}

int PacketSwitch::getNext () {
    return next;
}

int PacketSwitch::getIPLow () {
    return ipLow;
}

int PacketSwitch::getIPHigh () {
    return ipHigh;
}

void PacketSwitch::initFIFO () {
    int count = 0;

    fifos[0][0] = STDIN_FILENO;
    fifos[0][1] = STDOUT_FILENO;
    setPfd( count, fifos[0][0] );
    count++;

    mkopen( 0, switchNum, fifos[1][0], fifos[1][1] );

    setPfd( count, fifos[1][0] );
    count++;

    if ( prev != -1 ) {
        mkopen( prev, switchNum, fifos[2][0], fifos[2][1] );
        setPfd( count, fifos[2][0] );
        count++;
    }

    if ( next != -1 ) {
        mkopen( next, switchNum, fifos[3][0], fifos[3][1] );
        setPfd( count, fifos[3][0] );
        count++;
    }
}

void PacketSwitch::info() {
    cout << "Forwarding table" << endl;

    int count = 0;

    for ( auto it = ftable.begin(); it != ftable.end(); it++ ) {
        cout << "[" << to_string(count) << "]" << *it;
    }

    cout << "Packet Stats:" << endl;

    cout << "\tReceived " << "ADMIT:" << to_string(admitCount) << 
    ", HELLO_ACK:" << to_string(hello_ackCount) << 
    ", ADD:" << to_string(addCount) << 
    ", RELAYIN:" << to_string(relayinCount) << endl;

    cout << "\tTransmitted: " << "HELLO:" << to_string(helloCount) << 
    ", ASK:" << to_string(askCount) << 
    ", RELAYOUT:" << to_string(relayoutCount);
}

void PacketSwitch::setPfd ( int i, int rfd ) {
    pfds[i].fd = rfd;
    pfds[i].events = POLLIN;
    pfds[i].revents = 0;
}

void PacketSwitch::startPoll () {
    int in = 0;
    int rval = 0;
    int len = 0;

    char buf[MAXBUF];
    memset( buf, 0, sizeof(buf) );

    cout << "start polling" << endl;

    sendHELLO();

    while (1) {
        rval = poll( pfds, MAXPKFD, 0 );
        for ( in = 0; in < MAXPKFD; in++ ) {
            if ( pfds[in].revents && POLLIN ) {
                
                if ( in == 0 ) {
                    len = read( fifos[in][0], buf, MAXBUF );
                    
                    if ( strcpy( buf, "info" ) == 0 )
                        info();
                    else if ( strcpy( buf, "exit" ) == 0 ) {
                        info();
                        return;
                    } else 
                        warning( "a2w22: %s: command not found\n", buf );
                    
                    break;
                }

                Frame frame = rcvFrame(fifos[in][0]);
                PacketType packeType = frame.packetType;

                string prefix = "Received (src = " + 
                ( in == 1 ) ? "master" : "psw" + to_string(in) + 
                ", dest = psw" + to_string(switchNum);

                printFrame( prefix.c_str(), &frame ); 

                switch(packeType) {
                    case ADD:
                        break;
                    case HELLO_ACK:
                        break;
                    case RELAY:
                        break;
                }
            }
        }
    }
}

void PacketSwitch::sendHELLO () {
    Packet helloPk = composeHELLO( switchNum, prev, next, ipLow, ipHigh );

    string prefix = 
        "Transmitted (src = psw" + to_string(switchNum) + "dest= master) [HELLO]: ";
    
    sendFrame( prefix.c_str(), fifos[1][1], HELLO, &helloPk );

    helloCount++;
}
