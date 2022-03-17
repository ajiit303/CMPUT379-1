#include "main.h"


static int master = 0;

struct sigaction oldAct, newAct;

static MasterSwitch *masterSwitch;
static PacketSwitch *packetSwitch;


/**
 * @brief Signal handling function for SIGUSR1
 * 
 * @param sigNo 
 */
void infoHanlder ( int sigNo ) {
    if (master) 
        masterSwitch->info();
    else
        packetSwitch->info();
}

int main ( int argc, char *args[] ) {

    // initialize sigaction struct
    newAct.sa_handler = &infoHanlder;
    newAct.sa_flags |= SA_SIGINFO;

    // install signal handler
    sigaction( SIGUSR1, &newAct, &oldAct );

    if ( argc == 4 ) {
        int numSwitch = atoi(args[2]);
        int portNumber = atoi(args[3]);

        if ( numSwitch > 0 && numSwitch <= MAX_NSW ) {
            master = 1;
            masterSwitch = new MasterSwitch( numSwitch, portNumber );
            
            masterSwitch->createSocket();
            masterSwitch->bindSocket();
            masterSwitch->masterListen();
            masterSwitch->startPoll();

            delete masterSwitch;
        }
    } else if ( argc == 8 ) {

        int switchNum = stoSwNum( string(args[1]) );
        string filename = string(args[2]);
        int prev = stoSwNum( string(args[3]) );
        int next = stoSwNum( string(args[4]) );

        vector<string> ipRange;

        split( string(args[5]), "-", ipRange );

        int ipLow = stoi(ipRange.front());
        int ipHigh = stoi(ipRange.back());

        string serverName = string(args[6]);
        int portNumber = atoi(args[7]);

        packetSwitch = new PacketSwitch( switchNum, prev, next, ipLow, ipHigh, 
            filename, serverName, portNumber );

        packetSwitch->connectMaster();
        packetSwitch->createFIFO();

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