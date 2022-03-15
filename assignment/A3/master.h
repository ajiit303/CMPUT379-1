#ifndef MASTER_H
#define MASTER_H


#include <assert.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include <netdb.h>
#include <poll.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "master.h"
#include "message.h"

using namespace std;


typedef struct {
    int swNum;
    int prev, next;
    int ipLow, ipHigh;
} SwitchInfo;

class MasterSwitch {

    public:
        MasterSwitch ( int numSwitch, int portNumber );

        void info ();

        void initSocket();
        void startPoll ();

        void sendADD ( int switchNum, int dest, int wfd );
        void sendHELLO_ACK ( int switchNum, int wfd );
        
        void addSwitchInfo (HELLOPacket helloPk);
        Packet generateRule ( int switchNum, int destIP );

    private:
        int numSwitch;
        int portNumber;
        int serverFd = -1;

        int helloCount = 0, askCount = 0;
        int hello_ackCount = 0, addCount = 0;

        // 0 -> stdin, stdout
        // 1 ~ MAXNSW all packets switch
        int fds[MAXMSFD];
        struct pollfd pfds[MAXMSFD];
        
        vector<SwitchInfo> switchInfos;
        
        int forkSwitches();
        
        void setPfd ( int i, int rfd );
        
};



#endif