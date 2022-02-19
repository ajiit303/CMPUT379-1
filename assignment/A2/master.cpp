#include <assert.h>
#include <cstring>
#include <iostream>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>


#include "const.h"
#include "handler.h"
#include "master.h"
#include "tor.h"

using namespace std;


MasterSwitch::MasterSwitch (int numSwitch) {
    assert( numSwitch > 0 && numSwitch <= MAX_NSW );

    memset( fifos, 0, sizeof(fifos) );
    this->numSwitch = numSwitch;
}

void MasterSwitch::run () {
    pfd[0].fd = STDIN_FILENO;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;
}

