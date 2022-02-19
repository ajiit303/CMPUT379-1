#ifndef MASTER_H
#define MASTER_H


#include <vector>

#include <poll.h>

#include "tor.h"

using namespace std;


class MasterSwitch {
    public:
        MasterSwitch (int numSwitch);
        void run();

    private:
        int numSwitch;
        int fifos[MAX_NSW][2];
        struct pollfd pfd[2*MAX_NSW+1];
        vector<PacketSwitch> switches;
        
        int forkSwitches();
};


#endif