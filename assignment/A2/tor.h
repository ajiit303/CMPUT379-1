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
        void info();
        void startPoll();
        void sendHELLO();

    private:
        int switchNum;
        int prev, next;
        int ipLow, ipHigh;

        string filename;
        
        int admitCount = 0, hello_ackCount = 0, addCount = 0, relayinCount = 0;
        int helloCount = 0, askCount = 0, relayoutCount = 0;

        // 0 -> stdin, stdout
        // 1 -> port 0, master
        // 2 -> port 1, prev
        // 3 -> port 2, next
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