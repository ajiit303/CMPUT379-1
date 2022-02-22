#include <cstring>

#include <assert.h>
#include <unistd.h>

#include "handler.h"
#include "message.h"

using namespace std;


Frame rcvFrame (int fd) {
    int len;
    Frame frame;

    assert( fd >= 0 );
    memset( (char *)&frame, 0, sizeof(frame) );

    len = read( fd, (char *)&frame, sizeof(frame) );
    if ( len != sizeof(frame) )
        warning( "Received fram has length = %d (expected = %d)\n", len, sizeof(frame) );
    
    return frame;
}

Packet composeADD ( int srcIP_lo, int srcIP_hi, int destIP_lo, int destIP_hi, 
    int actionType, int actionVal, int pkCount ) {
    Packet pk;

    memset( (char *)&pk, 0, sizeof(pk) );
    
    pk.addPacket.srcIP_lo = srcIP_lo;
    pk.addPacket.srcIP_hi = srcIP_hi;
    pk.addPacket.destIP_lo = destIP_lo;
    pk.addPacket.destIP_hi = destIP_hi;
    pk.addPacket.actionType = actionType;
    pk.addPacket.actionVal = actionVal;
    pk.addPacket.pkCount = pkCount;

    return pk;
}

Packet composeASK ( int srcIP, int destIP ) {
    Packet pk;

    memset( (char *)&pk, 0, sizeof(pk) );

    pk.askPacket.srcIP = srcIP;
    pk.askPacket.destIP = destIP;

    return pk;
}

Packet composeHELLO ( int switchNum, int prev, int next, int srcIP_lo, int srcIP_high ) {
    Packet pk;

    memset( (char *)&pk, 0, sizeof(pk) );

    pk.helloPacket.switchNum = switchNum;
    pk.helloPacket.prev = prev;
    pk.helloPacket.next = next;
    pk.helloPacket.srcIP_lo = srcIP_lo;
    pk.helloPacket.srcIP_hi = srcIP_high;

    return pk;
}

Packet composeHELLO_ACK () {
    Packet pk;

    memset( (char *)&pk, 0, sizeof(pk) );

    return pk;
}

void sendFrame ( int fd, PacketType packetType, Packet *packet ) {
    Frame frame;

    assert( fd >= 0 );
    memset( (char *)&frame, 0, sizeof(frame) );

    frame.packetType = packetType;
    frame.packet = *packet;

    write( fd, (char *)&frame, sizeof(frame) ); 
}