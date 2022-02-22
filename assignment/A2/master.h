#ifndef MASTER_H
#define MASTER_H


#include <vector>

#include <poll.h>

#include "tor.h"

using namespace std;


class MasterSwitch {
    public:
        MasterSwitch (int numSwitch);
        void initFIFO ();
        void startPoll();

    private:
        int numSwitch;
        
        int fifos[MAXMSFD][2];
        struct pollfd pfds[MAXMSFD];
        
        vector<PacketSwitch> switches;
        
        int forkSwitches();
        
        void setPfd ( int i, int rfd );
        
};


#endif