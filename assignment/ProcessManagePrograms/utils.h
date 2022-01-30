#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/resource.h>
#include <sys/times.h>

#ifndef UTILS_H
#define UTILS_H

#define MAXCHARS 32
#define MAXNTOKENS 132
#define MAXLEN MAXCHARS * MAXNTOKENS
#define NTASK 32
#define GUESSPATHLEN 1024

struct TaskDB {
    int index;
    pid_t pid;
};

static long maxPath = 0;

/** @brief Replace variable such $HOME with its actual content
 * 
 * @param t Input String
 * @param omitSet Omitted character such as "$"
 * @param repStr 
 * @return Void
 */
int gsub ( char *inStr, char *filter, char *repStr);

/**
 * @brief Get the Path Size object (Need Citation)
 * 
 * @param sizep 
 * @return char* 
 */
char *pathAlloc (size_t *sizePtr);

int startWith ( const char *str, const char compare );

/**
 * @brief (Need Citation)
 * 
 * @param wallTime 
 * @param tmsStart 
 * @param tmsEnd 
 */
void printTime ( clock_t wallTime, struct tms *tmsStart, struct tms *tmsEnd );

/** @brief Split command and its arguments based on field separater
 *
 * @param inStr Input command line
 * @param token Arguments of command line (Max 32)
 * @param fs Field separater for separating arguments and command
 * @return Number of tokens
 */
int split ( char *inStr, char tokens[][MAXCHARS], char fs[] );

#endif
