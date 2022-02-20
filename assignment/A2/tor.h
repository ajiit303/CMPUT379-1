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
        void run();

    private:
        int switchNum;
        int prev, next;
        int ipLow, ipHigh;
        string filename;
        
        int fifos[MAXPORT+1][2];
        struct pollfd pfds[MAXPORT+1];

        vector<Rule> ftable;

};


#endif