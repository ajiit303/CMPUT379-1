#include "handler.h"
#include "utils.h"


void printTime ( clock_t wallTime, struct tms *tmsStart, struct tms *tmsEnd ) {
    long clockTick = 0;

    if ( clockTick == 0 ) {
        if ( (clockTick = sysconf(_SC_CLK_TCK)) < 0 ) {
            warning( "Failed to fetch clock ticks per second" );
        }
    }

    printf( "   real:   %7.2f\n", wallTime / (double) clockTick );
    
    printf( "   user:   %7.2f\n", 
        ( tmsEnd->tms_utime - tmsStart->tms_utime ) / (double) clockTick );
    
    printf( "   sys:   %7.2f\n", 
        ( tmsEnd->tms_stime - tmsStart->tms_stime ) / (double) clockTick );
    
    printf( "   child user:   %7.2f\n", 
        ( tmsEnd->tms_cutime - tmsStart->tms_cutime ) / (double) clockTick );

        printf( "   child sys:   %7.2f\n", 
        ( tmsEnd->tms_cstime - tmsStart->tms_cstime ) / (double) clockTick );

}

char *pathAlloc (size_t *sizePtr) {
    char *ptr = NULL;
    int errno = 0;
    size_t size = 0;

    if (maxPath == 0) {
        errno = 0;

        if ( ( maxPath = pathconf( "/", _PC_PATH_MAX ) ) < 0 ) {
            if ( errno == 0 ) {
                maxPath = 1024;
            } else {
                warning( "pathconf error for _PC_PATH_MAX" );
            }
        } else {
            maxPath++;
        }
    }

    size = maxPath;
    ptr = malloc(size);

    if ( sizePtr != NULL ) {
        *sizePtr = size;
    }

    return ptr;
}

int split ( char *inStr, char tokens[MAXTOKENS][MAXTOKENS], char fs[] ) {
    int numToken = 0;

    char * token = NULL;

    token = strtok( inStr, fs );
    while ( token != NULL ) {
        strcpy(tokens[numToken++], token);

        token = strtok( NULL, fs );
    }

    token = NULL;

    return numToken;
}
