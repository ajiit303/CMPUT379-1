#include <assert.h>
#include <cstring>
#include <cstdio>
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

    memset( &fifos, 0, sizeof(fifos) );
    memset( &pfds, 0, sizeof(pfds) );

    if ( pthread_mutex_init( &this->ftableLock, NULL ) != 0 ) {
        fatal( "Fail to create resource lock for forwarding table" );
        exit(1);
    }
}

PacketSwitch::~PacketSwitch() {
    
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

void PacketSwitch::setPfd ( int i, int rfd ) {
    pfds[i].fd = rfd;
    pfds[i].events = POLLIN;
    pfds[i].revents = 0;
}

void PacketSwitch::info() {
    cout << endl << "Forwarding table" << endl;

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
    ", RELAYOUT:" << to_string(relayoutCount) << endl << endl;
}

void * PacketSwitch::readFile () {
    string sline = "";
    
    char line[1024];
    vector<string> tokens;
    
    memset( &line, 0, sizeof(line) );

    FILE *f = fopen( filename.c_str(), "r");
    assert ( f != nullptr );

    while ( hello_ackCount <= 0 ) { }

    while ( fgets( line, 1024, f ) ) {
        if ( strlen(line) == 0 || line[0] == '#' || line[0] == '\n' || line[0] == ' ' ) {
            sline = "";
            tokens.clear();
            memset( &line, 0, sizeof(line) );
            continue;
        }

        sline = string(line);
        sline = sline.substr( 0, sline.find("\n") );
        split( sline, " ", tokens );

        processLine(tokens);

        sline = "";
        tokens.clear();
        memset( &line, 0, sizeof(line) );
    }

    return NULL;
}

void PacketSwitch::processLine ( vector<string> &tokens ) {
    int swNum = stoSwNum( tokens.front() );
    if ( swNum != switchNum ) return;

    if ( tokens.at(1) == "delay" ) {
        int second = stoi( tokens.back() );

        cout << "** Entering a delay period of " << tokens.back() << " mesc" << endl << endl;

        usleep(second);

        cout << "** Delay period ended" << endl << endl;

        return;
    }
    
    int srcIP = stoi( tokens.at(1) );
    int destIP = stoi( tokens.back() );

    int forwarded = forwardPacket( srcIP, destIP );

    if (forwarded) return;    
        
    sendASK ( srcIP, destIP );
    
    int prevAddCount = addCount;
    while ( prevAddCount == addCount ) { }

    forwarded = forwardPacket( srcIP, destIP );
    assert( forwarded == 1 );
}

void * PacketSwitch::startPoll () {
    int in = 0;
    int len = 0;
    int rval = 0;

    char buf[MAXBUF];
    string prefix;

    Frame frame;
    PacketType packetType;

    cout << "start polling" << endl << endl;

    sendHELLO();

    while (1) {
        rval = poll( pfds, MAXPKFD, 0 );
        len = 0;
        prefix = "";

        memset( &buf, 0, sizeof(buf) );
        memset( &frame, 0, sizeof(frame) );
        packetType = UNKNOWN;

        for ( in = 0; in < MAXPKFD; in++ ) {
            if ( pfds[in].revents & POLLIN ) {
                if ( in == 0 ) {
                    len = read( fifos[in][0], buf, MAXBUF );
                    
                    if ( strcmp( buf, "info\n" ) == 0 )
                        info();
                    else if ( strcmp( buf, "exit\n" ) == 0 ) {
                        info();
                        exit(0);
                    } else 
                        warning( "a2w22: %s: command not found\n", buf );
                } else {

                frame = rcvFrame(fifos[in][0]);
                packetType = frame.packetType;


                prefix = makePrefix( in - 1, switchNum,0 );
                printFrame( prefix.c_str(), &frame ); 

                switch(packetType) {
                    case ADD:
                        addRule(frame.packet.addPacket);
                        addCount++;
                        break;
                    case HELLO_ACK:
                        hello_ackCount++;
                        break;
                    case RELAY:
                        relayinCount++;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

string PacketSwitch::makePrefix ( int src, int dest, int option ) {
    string strSrc = ( src == 0 ) ? "master" : ( "psw" + to_string(src) );
    string strDest = ( dest == 0 ) ? "master" : ( "psw" + to_string(dest) );
    string strOption = option ? "Transmitted" : "Received";
    string prefix = strOption + "(src = " + strSrc + ", dest = " + strDest + ")";

    return prefix;
}

void PacketSwitch::addRule (ADDPacket addPk) {
    ftable.push_back( Rule( addPk.srcIP_lo, addPk.srcIP_hi, 
                            addPk.destIP_lo, addPk.destIP_hi, 
                            addPk.actionType, addPk.actionVal ) );
}

int PacketSwitch::forwardPacket ( int srcIP, int destIP ) {

    for ( auto it = ftable.begin(); it != ftable.end(); it++ ) {
        if ( it->isMatch(srcIP, destIP) ) {
            
            if ( it->actionType != DROP && !it->isReach(destIP) )
                sendRelay( it->actionVal, srcIP, destIP );
            
            it->pkCount++;
            admitCount++;
            return 1;
        }
    }

    return 0;
}

void PacketSwitch::sendASK ( int srcIP, int destIP ) {
    Packet askPk = composeASK( srcIP, destIP );

    string prefix = makePrefix( switchNum, 0, 1 );

    sendFrame( prefix.c_str(), fifos[1][1], ASK, &askPk );

    askCount++;
}

void PacketSwitch::sendHELLO () {
    Packet helloPk = composeHELLO( switchNum, prev, next, ipLow, ipHigh );

    string prefix = makePrefix( switchNum, 0, 1 );

    sendFrame( prefix.c_str(), fifos[1][1], HELLO, &helloPk );

    helloCount++;
}

void PacketSwitch::sendRelay ( int actionVal, int srcIP, int destIP ) {
    admitCount++;
    relayoutCount++;
}
