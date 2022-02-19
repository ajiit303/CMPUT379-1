#ifndef TOR_H
#define TOR_H


#include <string>
#include <vector>

#include "rule.h"

using namespace std;


class PacketSwitch {
    public:
        PacketSwitch ( int switchNum, int prev, int next, int ipLow, int ipHigh,
        string filename );

    private:
        int switchNum;
        int prev, next;
        int ipLow, ipHigh;
        int fifos[MAX_NSW+1][MAX_NSW+1];
        string filename;
        vector<Rule> ftable;

};


#endif