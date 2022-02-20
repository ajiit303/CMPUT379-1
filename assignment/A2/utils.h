#ifndef UTILS_H
#define UTILS_H


#include <string>

using namespace std;


int getSwitchNum (string str);

void mkopen( int s1, int s2, int &rfd, int &wfd );

int split ( string str, string del, string tokens[] );

#endif