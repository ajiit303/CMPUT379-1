#ifndef MESSAGE_H
#define MESSAGE_H

#include "const.h"

using namespace std;


/**
 * HELLO switch number, number of its neighbouring switches, ranger of IP
 * HELLO_ACK Reply of HELLO (no message)
 * 
 * ASK not find a matching rule in table, sends an SK packet to the master switch
 * ADD master switch replies with a rule stored in a packet of type ADD
 * 
 * RELAY a switch forward a received packet header to a neighbour 
 */

typedef enum { UNKNOWN, ADD, ASK, HELLO, HELLO_ACK, RELAY } PacketType;

const char PacketTypeName[][16] = { "UNKNOWN", "ADD", "ASK", "HELLO", "HELLO_ACK", "RELAY",  };

typedef struct { int srcIP_lo; int srcIP_hi;
    int destIP_lo; int destIP_hi;
    int actionType; int actionVal;
    int pkCount; } ADDPacket;

typedef struct { int srcIP; int destIP; } ASKPacket;

typedef struct { int switchNum;
    int prev; int next;
    int srcIP_lo; int srcIP_hi; } HELLOPacket;

typedef struct { } HELLO_ACKPacket;

typedef struct { int srcIP; int destIP; } RELAYPacket;

typedef union { ADDPacket addPacket;
                ASKPacket askPacket;
                HELLOPacket helloPacket; 
                HELLO_ACKPacket hello_ackPacket;
                RELAYPacket replayPacket; } Packet;

typedef struct { PacketType packetType; Packet packet; } Frame;

Frame rcvFrame (int fd);

Packet composeADD ( int srcIP_lo, int srcIP_hi, int destIP_lo, int destIP_hi, 
    int actionType, int actionVal, int pkCount );

Packet composeASK ( int srcIP, int destIP );

Packet composeHELLO ( int switchNum, int prev, int next, int srcIP_lo, int srcIP_high );

Packet composeHELLO_ACK ();

Packet composeRELAY ( int srcIP, int destIP );

void printFrame ( const char *prefix, Frame *frame );

void sendFrame ( const char *prefix, int fd, PacketType packetType, Packet *packet );

#endif