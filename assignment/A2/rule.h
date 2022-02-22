#ifndef RULE_H
#define RULE_H


#include <iostream>

using namespace std;


class Rule {
    public:
        Rule (int srcIP_lo, int srcIP_hi, int destIP_lo, int destIP_hi, 
        int actionType, int actionVal );

    private:
        int srcIP_lo, srcIP_hi;
        int destIP_lo, destIP_hi;
        int actionType, actionVal;
        int pktCount = 0;

};

ostream& operator<< ( ostream& outs, const Rule& obj );



#endif