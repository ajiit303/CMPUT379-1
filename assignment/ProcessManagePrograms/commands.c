#include "commands.h"
#include "handler.h"
#include "utils.h"


void pdir() {
    char *currentDir;
    size_t bufferSize;

    currentDir = pathAlloc(&bufferSize);

    if ( currentDir == NULL ) {
        warning( "Failed to allocate memory for path string" );
        
        return;
    }

    if ( getcwd( currentDir, bufferSize ) == NULL ) {
        warning( "Failed to get current directory" );

        return;
    }

    printf( "%s\n", currentDir );
}