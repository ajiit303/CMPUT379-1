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
    char command[MAXLEN];
    int index;
    pid_t pid;
};

static long maxPath = 0;

/** @brief Omit a specific string pattern based on the specific filter and 
 * replace with string, repStr
 * 
 * @param t Input String
 * @param omitSet Omitted string
 * @param repStr Replacement string
 * @return Void
 */
int gsub ( char *inStr, char *filter, char *repStr);

/**
 * @brief Get the estimated string size for a path since
 * a path can be very long. Clone version of the code in
 * Chapter 2.5.5 in the textbook APUE
 * 
 * @param sizep pointer of size
 * @return char* 
 */
char *pathAlloc (size_t *sizePtr);

/**
 * @brief Return true if a string, str starts with a 
 * character, compare.
 * 
 * @param str Input string
 * @param compare 
 * @return int 
 */
int startWith ( const char *str, const char compare );

/**
 * @brief Calculate and print the user and CPU times for the current process 
 * (and its terminated children). Clon version of the code in Chapter 8.17 in
 * the textbook APUE
 * 
 * @param wallTime wall clock time
 * @param tmsStart ptr of time structure generated at the beginning of the program
 * @param tmsEnd ptr of time structure generated at the ending of the program
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
