#include "commands.h"
#include "handler.h"
#include "utils.h"


int main () {

  /* --------- Start time recording --------- */
    struct tms tmsStart;
    memset( &tmsStart, 0, sizeof(tmsStart) );
    
    clock_t startWallTime = times(&tmsStart);
    if ( startWallTime == -1 ) {
        warning( "Fail to start recording user CPU time" );
    }


 /* --------- Set CPU time limit --------- */
    const struct rlimit limit = {
        300, // Soft limit
        600 // Hard limit
    };

    if ( setrlimit( RLIMIT_CPU, &limit ) != 0 ) {
        warning( "Fail to set CPU time limit" );
    }


    char *currentPath = NULL;
    char inStr[MAXTOKENLENGTH * MAXTOKENS];
    char tokens[MAXTOKENS][MAXTOKENLENGTH];
    struct TaskDB taskList[NTASK];
    pid_t parent_pid = 0;

    memset( &inStr, 0, sizeof(inStr) );
    memset( &tokens, 0, sizeof(tokens) );
    memset( &tokens, 0, sizeof(taskList) );

    currentPath = getenv("HOME");
    parent_pid = getpid();


 /* --------- Main Loop --------- */
    printf( "msh379 [%d]: ", parent_pid );
    while( scanf("%[^\n]", inStr) != EOF ) {
        getchar();

        int numTokens = split( inStr, tokens, " " );

        if ( numTokens != 0 ) {
            char * command = tokens[0];

            if ( strcmp( command, "cdir" ) == 0 ) {
                
            } else if ( strcmp( command, "pdir" ) == 0 ) {
                pdir();
            } else if ( strcmp( command, "lstasks" ) == 0 ) {
                
            } else if ( strcmp( command, "run" ) == 0 ) {
                
            } else if ( strcmp( command, "stop" ) == 0 ) {
                
            } else if ( strcmp( command, "continue" ) == 0 ) {
                
            } else if ( strcmp( command, "terminate" ) == 0 ) {
                
            } else if ( strcmp( command, "check" ) == 0 ) {
                
            } else if ( strcmp( command, "exit" ) == 0 ) {
                
            } else if ( strcmp( command, "quit" ) == 0 ) {
                break;
            } else {
                printf("msh379: %s: command not found", command);
            }
        }

        printf( "msh379 [%d]: ", parent_pid );

        memset( &inStr, 0, sizeof(inStr) );
        memset( &tokens, 0, sizeof(tokens) );
    }


 /* --------- Stop time recording --------- */
    struct tms tmsEnd;
    memset( &tmsEnd, 0, sizeof(tmsEnd) );

    clock_t endWallTime = times(&tmsEnd);
    if ( endWallTime == -1 ) {
        warning( "Fail to end recording user CPU time" );
    }

    printf("\n");
    printTime(endWallTime - startWallTime, &tmsStart, &tmsEnd);

    return 0;
}

