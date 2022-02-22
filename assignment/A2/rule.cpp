#include <string>

#include <assert.h>

#include "const.h"
#include "rule.h"

using namespace std;


Rule::Rule ( int srcIP_lo, int srcIP_hi, int destIP_lo, int destIP_hi, 
    int actionType, int actionVal ) {
    
    assert( srcIP_lo < srcIP_hi && srcIP_lo >= 0 );
    assert( srcIP_hi <= MAXIP );
    assert( destIP_lo < destIP_hi && destIP_lo >= 0 );
    assert( destIP_hi <= MAXIP );

    this->srcIP_lo = srcIP_lo;
    this->srcIP_hi = srcIP_hi;
    this->destIP_lo = destIP_lo;
    this->destIP_hi = destIP_hi;
    this->actionType = actionType;
    this->actionVal = actionVal;
}

ostream& operator<< ( ostream& outs, const Rule& obj ) {
    
}