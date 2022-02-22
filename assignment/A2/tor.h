#ifndef TOR_H
#define TOR_H


#include <string>
#include <vector>

#include <poll.h>

#include "rule.h"

using namespace std;


class PacketSwitch {
    public:
        PacketSwitch ( int switchNum, int prev, int next, int ipLow, int ipHigh,
        string filename );
        void initFIFO();
        void printFtable();
        void startPoll();
        void sendHELLO();

    private:
        int switchNum;
        int prev, next;
        int ipLow, ipHigh;
        string filename;
        
        int fifos[MAXPKFD][2];
        struct pollfd pfds[MAXPKFD];

        vector<Rule> ftable;
        
        int getSwitchNum ();
        int getPrev ();
        int getNext ();
        int getIPLow ();
        int getIPHigh ();

        void setPfd ( int i, int rfd );

};


#endif