#ifndef MASTER_H
#define MASTER_H


#include <vector>

#include <poll.h>

#include "message.h"

using namespace std;


typedef struct {
    int swNum;
    int prev, next;
    int ipLow, ipHigh;
} SwitchInfo;

class MasterSwitch {

    public:
        MasterSwitch (int numSwitch);
        void initFIFO ();
        void info ();
        
        void startPoll ();

        void sendHELLO_ACK ( int switchNum, int fd );
        
        void addSwitchInfo (HELLOPacket pk);

    private:
        int numSwitch;

        int helloCount, askCount;
        int hello_ackCount, addCount;

        // 0 -> stdin, stdout
        // 1 ~ MAXNSW all packets switch
        int fifos[MAXMSFD][2];
        struct pollfd pfds[MAXMSFD];
        
        vector<SwitchInfo> switchInfos;
        
        int forkSwitches();
        
        void setPfd ( int i, int rfd );
        
};



#endif