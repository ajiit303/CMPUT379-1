#include "master.h"


MasterSwitch::MasterSwitch ( int numSwitch, int portNumber ) {
    assert( numSwitch > 0 && numSwitch <= MAX_NSW );

    this->numSwitch = numSwitch;
    this->portNumber = portNumber;

    memset( pfds, 0, sizeof(pfds) );

    setPfd( STDININDEX, STDIN_FILENO );
}

void MasterSwitch::createSocket() {
    serverFd = socket( AF_INET, SOCK_STREAM, 0 );
    
    if ( serverFd < 0 ) {
        fatal( "sokcet() error\n" );
        exit(1);
    }

    cout << "master switch's socket is created" << endl;
}

void MasterSwitch::bindSocket() {
    struct sockaddr_in serverSockAddr;

    memset( (char *)&serverSockAddr, 0, sizeof(serverSockAddr) );

    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverSockAddr.sin_port = htons(portNumber);

    if ( bind( serverFd, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr) ) < 0 ) {
        fatal( "bind() error\n" );
        exit(1);
    }

    cout << "master switch's socket is binded" << endl;

    setPfd( MSWSFDINDEX, serverFd );
}

void MasterSwitch::masterListen() {
    listen( serverFd, MAX_NSW );

    cout << "master switch starts listening" << endl;
}

void MasterSwitch::masterAccept() {
    int pkSock = -1;
    
    struct sockaddr_in pkAddr;
    socklen_t pkAddrLen;

    pkAddrLen = sizeof(pkAddr);

    pkSock = accept( pfds[MSWSFDINDEX].fd, (struct sockaddr *)&pkAddr, &pkAddrLen );
    if ( pkSock < 0 )
        cout << "accept() error " << pkSock << endl;
    else {
        setPfd( accpectedConn, pkSock );

        accpectedConn++;

        cout << "New connection accepted" << endl << endl;
        cout << "Number of establish connection: " << accpectedConn - 2 << endl << endl;
    }
}

void MasterSwitch::startPoll () {
    int in = 0; // index 
    int len = 0; // measure number of bytes read from socket
    int rval = 0;

    char buf[MAXBUF];
    string prefix;

    Frame frame;
    PacketType packetType;

    cout << "start polling" << endl << endl;

    while (1) {
        len = 0;

        prefix = "";
        memset( &buf, 0, sizeof(buf) );
        memset( &frame, 0, sizeof(frame) );

        packetType = UNKNOWN;

        rval = poll( pfds, MAXMSFD, 0 );

        if ( accpectedConn < MAX_NSW && ( pfds[MSWSFDINDEX].revents & POLLIN ) )
            masterAccept();

        for ( in = 0; in < MAXMSFD; in++ ) {
            if ( pfds[in].revents & POLLIN ) {
                
                if ( in == 0 ) {
                    len = read( pfds[in].fd, buf, MAXBUF );

                    if ( strcmp( buf, "info\n" ) == 0 )
                        info();
                    else if ( strcmp( buf, "exit\n" ) == 0 ) {
                        info();
                        return;
                    } else 
                        warning( "a2w22: %s: command not found\n", buf );
                    
                    memset( &buf, 0, sizeof(buf) );

                } else {
                    
                    frame = rcvFrame( pfds[in].fd );
                    packetType = frame.packetType;
                    
                    prefix = "\nReceived (src = psw" + to_string(in) + ", dest = master)";
                    
                    printFrame( prefix.c_str(), &frame );

                    switch(packetType) {
                        case ASK:
                            askCount++;
                            sendADD( in, frame.packet.askPacket.destIP, pfds[in].fd );
                            break;
                        case HELLO:
                            helloCount++;
                            addSwitchInfo(frame.packet.helloPacket);
                            sendHELLO_ACK( in, pfds[in].fd );
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}

void MasterSwitch::info () {
    cout << endl << "Switch information:" << endl;

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
    ", ADD:" << to_string(addCount) << endl << endl;
}

/**
 * @brief 0 -> STDIN, 1 -> server socket file descriptor, 2 -> ... packet switch
 * 
 * @param i index of the file descriptor in the array
 * @param rfd 
 */
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

    cout << "psw" << switchInfo.swNum << "is connected" << endl << endl;
}

Packet MasterSwitch::generateRule ( int switchNum, int destIP ) {
    Packet rule;
    memset( &rule, 0, sizeof(rule) );

    for ( auto it = switchInfos.begin(); it != switchInfos.end(); it++ ) {
        if ( destIP >= it->ipLow && destIP <= it->ipHigh ) {
            int actionVal;

            if ( switchNum == it->swNum )
                actionVal = 3; // port 3
            else if ( switchNum > it->swNum )
                actionVal = 1; // port 2
            else
                actionVal = 2; // port 1

            rule = composeADD( 0, MAXIP, it->ipLow, it->ipHigh, FORWARD, actionVal, 0 );
            return rule;
        }
    }

    rule = composeADD( 0, MAXIP, destIP, destIP, DROP, 0, 0 );

    return rule;
}

void MasterSwitch::sendADD ( int switchNum, int destIP, int wfd ) {
    Packet addPk;
    memset( &addPk, 0, sizeof(addPk) );

    addPk = generateRule( switchNum, destIP );

    string prefix = "Transmitted (src = master, dest = " + to_string(switchNum) + ")";

    sendFrame( prefix.c_str(), wfd, ADD, &addPk );

    addCount++;
}

void MasterSwitch::sendHELLO_ACK ( int switchNum, int wfd ) {
    Packet hello_ackPk;

    memset( &hello_ackPk, 0, sizeof(hello_ackPk) );

    hello_ackPk = composeHELLO_ACK();

    string prefix = "Transmitted (src = master, dest = " + to_string(switchNum) + ")";

    sendFrame( prefix.c_str(), wfd, HELLO_ACK, &hello_ackPk );

    hello_ackCount++;
}


