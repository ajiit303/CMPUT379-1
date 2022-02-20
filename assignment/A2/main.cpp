#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

#include "const.h"
#include "master.h"
#include "tor.h"
#include "utils.h"

using namespace std;


int main ( int argc, char *args[] ) {

    if ( argc == 3 ) {
        int numSwitch = atoi(args[2]);

        if ( numSwitch > 0 && numSwitch <= MAX_NSW ) {
            MasterSwitch masterSwitch = MasterSwitch(numSwitch);
            masterSwitch.run();
        }
    } else if ( argc == 6 ) {

        int switchNum = getSwitchNum( string(args[1]) );
        int prev = getSwitchNum( string(args[3]) );
        int next = getSwitchNum( string(args[4]) );

        string ipRange[2];

        split( string(args[5]), "-", ipRange );

        int ipLow = stoi(ipRange[0]);
        int ipHigh = stoi(ipRange[1]);

        string filename = string(args[2]);

        PacketSwitch packetSwitch = PacketSwitch( switchNum, prev, next, 
        ipLow, ipHigh, filename );

        packetSwitch.run();
    }

    return 0;
}