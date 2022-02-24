#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <pthread.h>
#include <unistd.h>

#include "const.h"
#include "handler.h"
#include "master.h"
#include "tor.h"
#include "utils.h"

using namespace std;

typedef void * (*THREADFUNCPTR) (void *);


int main ( int argc, char *args[] ) {
    if ( argc == 3 ) {
        int numSwitch = atoi(args[2]);

        if ( numSwitch > 0 && numSwitch <= MAX_NSW ) {
            MasterSwitch masterSwitch = MasterSwitch(numSwitch);
            masterSwitch.initFIFO();
            masterSwitch.startPoll();
        }
    } else if ( argc == 6 ) {

        int switchNum = stoSwNum( string(args[1]) );
        int prev = stoSwNum( string(args[3]) );
        int next = stoSwNum( string(args[4]) );

        vector<string> ipRange;

        split( string(args[5]), "-", ipRange );

        int ipLow = stoi(ipRange.front());
        int ipHigh = stoi(ipRange.back());

        string filename = string(args[2]);

        PacketSwitch * packetSwitch = new 
            PacketSwitch( switchNum, prev, next, ipLow, ipHigh, filename );

        packetSwitch->initFIFO();

        int rval;
        pthread_t fileThreadId, pollThreadId;

        rval = pthread_create( &fileThreadId, NULL, (THREADFUNCPTR)&PacketSwitch::readFile, packetSwitch );

        if (rval) {
            fatal( "pthread_create() error" );
            exit(1);
        }

        rval = pthread_create( &pollThreadId, NULL, (THREADFUNCPTR)&PacketSwitch::startPoll, packetSwitch );

        if (rval) {
            fatal( "pthread_create() error" );
            exit(1);
        }

        rval = pthread_join( fileThreadId, NULL );
        if (rval) {
            fatal( "pthread_join() error %d", rval );  
        }

        rval = pthread_join( pollThreadId, NULL );
        if (rval) {
            fatal( "pthread_join() error %d", rval );  
        }

        delete packetSwitch;
    }

    return 0;
}