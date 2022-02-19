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
        int fifos[MAXPORT][2];
        string filename;
        struct pollfd pfd[2*MAXPORT+1];
        vector<Rule> ftable;

};


#endif