#include <assert.h>
#include <cstring>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "const.h"
#include "master.h"
#include "tor.h"

using namespace std;


MasterSwitch::MasterSwitch (int numSwitch) {
    assert( numSwitch > 0 && numSwitch <= MAX_NSW );

    memset( fifos, 0, sizeof(fifos) );
    this->numSwitch = numSwitch;
}

void MasterSwitch::run () {
    for (size_t i = 1; i < numSwitch; i++)
    {
        
    }
    
}

