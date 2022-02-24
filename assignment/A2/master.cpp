#include <assert.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "master.h"
#include "message.h"
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

void MasterSwitch::info () {
    cout << "Switch information:" << endl;

    for ( auto it = switchInfos.begin(); it != switchInfos.end(); it++ ) {
        cout << "[psw" << to_string(it->swNum) << "]" << 
        " port1 = " << to_string(it->prev) << 
        ", port2 = " << to_string(it->next) << 
        ", port3 = " << to_string(it->ipLow) << "-" << to_string(it->ipHigh) << endl;
    }

    cout << endl;

    cout << "Packet Stats:" << endl;
    
    cout << "\tReceived:\t" << "HELLO:" << to_string(helloCount) << 
    ", ASK:" << to_string(askCount) << endl;

    cout << "\tTransmitted: " << "HELLO_ACK:" << to_string(hello_ackCount) << 
    ", ADD:" << to_string(addCount) << endl;
}

void MasterSwitch::setPfd ( int i, int rfd ) {
    pfds[i].fd = rfd;
    pfds[i].events = POLLIN;
    pfds[i].revents = 0;
}

void MasterSwitch::addSwitchInfo (HELLOPacket pk) {
    SwitchInfo switchInfo;

    memset( (char *)&switchInfo, 0, sizeof(switchInfo) );

    switchInfo.swNum = pk.switchNum;
    switchInfo.prev = pk.prev;
    switchInfo.next = pk.next;
    switchInfo.ipLow = pk.srcIP_lo;
    switchInfo.ipHigh = pk.srcIP_hi;

    auto pos = switchInfos.begin() + switchInfo.swNum - 1;

    switchInfos.insert( pos, switchInfo );

    helloCount++;
}

void MasterSwitch::sendHELLO_ACK ( int switchNum, int wfd ) {
    Packet pk = composeHELLO_ACK();

    string prefix = "Transmitted (src = master, dest = " + to_string(switchNum) + ")";

    sendFrame( prefix.c_str(), wfd, HELLO_ACK, &pk );

    hello_ackCount++;
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
                    len = read( fifos[in][0], buf, MAXBUF );
                    
                    if ( strcpy( buf, "info" ) == 0 )
                        info();
                    else if ( strcpy( buf, "exit" ) == 0 ) {
                        info();
                        return;
                    } else 
                        warning( "a2w22: %s: command not found\n", buf );
                } else {
                    Frame frame = rcvFrame( fifos[in][0] );
                    PacketType packetType = frame.packetType;
                    
                    string prefix = "Received (src = psw" + to_string(in) + ", dest = master)";
                    
                    printFrame( prefix.c_str(), &frame );

                    switch(packetType) {
                        case ASK:
                            break;
                        case HELLO:
                            addSwitchInfo(frame.packet.helloPacket);
                            sendHELLO_ACK( in, fifos[in][1] );
                            break;
                    }
                }
            } 
        }
    }
}
