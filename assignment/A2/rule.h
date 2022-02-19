#ifndef RULE_H
#define RULE_H


class Rule {
    public:
        Rule (int srcIP_lo, int srcIP_hi, int destIP_lo, int destIP_hi, 
        int actionVal );

    private:
        int srcIP_lo, srcIP_hi;
        int destIP_lo, destIP_hi;
        int actionVal;
        int pktCount = 0;
};


#endif