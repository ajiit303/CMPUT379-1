#ifndef UTILS_H
#define UTILS_H


#include <string>

using namespace std;


int getSwitchNum (string str);

void mkopen ( int s1, int s2, int &rfd, int &wfd );

void mkopenr ( int s1, int s2, int &rfd );

void mkopenw ( int s1, int s2, int &rfd );

int split ( string str, string del, string tokens[] );

#endif