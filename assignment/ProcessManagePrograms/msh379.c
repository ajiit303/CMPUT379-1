#include <signal.h>

#include "commands.h"
#include "handler.h"
#include "utils.h"


int main () {

  /* --------- Start time recording --------- */
    struct tms tmsStart;
    memset( &tmsStart, 0, sizeof(tmsStart) );
    
    clock_t startWallTime = times(&tmsStart);
    if ( startWallTime == -1 )
        warning( "Fail to start recording user CPU time" );


 /* --------- Set CPU time limit --------- */
    const struct rlimit limit = {
        300, // Soft limit
        600 // Hard limit
    };

    if ( setrlimit( RLIMIT_CPU, &limit ) != 0 )
        warning( "Fail to set CPU time limit" );


    int numTasks;
    int exitFlag;
    pid_t parent_pid;

    char inStr[MAXLEN];
    char tokens[MAXNTOKENS][MAXCHARS];
    struct TaskDB taskList[NTASK];

    memset( &inStr, 0, sizeof(inStr) );
    memset( &tokens, 0, sizeof(tokens) );
    memset( &taskList, 0, sizeof(taskList) );

    numTasks = 0;
    parent_pid = getpid();


 /* --------- Main Loop --------- */
    printf( "msh379 [%d]: ", parent_pid );
    while( scanf("%[^\n]", inStr) != EOF ) {
        getchar();

        int numTokens = split( inStr, tokens, " " );

        if ( numTokens != 0 ) {

            char *command = tokens[0];

            if ( strcmp( command, "cdir" ) == 0 ) 
                cdir(tokens[1]);
            else if ( strcmp( command, "pdir" ) == 0 )
                pdir();
            else if ( strcmp( command, "lstasks" ) == 0 )
                lstasks(taskList);
            else if ( strcmp( command, "run" ) == 0 ) {
                if ( numTokens > 6 )
                    warning( "run: too many arguments\n" );
                else if (numTasks == NTASK - 1) {
                    warning( "run: number of task limit reached, cannot run more task\n" );
                } else {
                    pid_t pid;

                    pid = run( tokens[1], tokens[2] );
                    if (pid == 0) {
                        for ( ; ; ) {

                        }
                    } else {
                        taskList[numTasks].index = numTasks;
                        taskList[numTasks].pid = pid;
                        numTasks++;
                    }
                }
            } else if ( strcmp( command, "stop" ) == 0 ) {
                if ( numTokens == 1 ) {
                    warning( "stop: no specified task\n" );
                } else if ( numTokens > 2 ) {
                    warning( "stop: too many arguments\n" );
                } else if ( numTasks == 0 ) {
                    warning( "stop: No tasks are running\n" );
                } else {
                    int taskNo = atoi(tokens[1]);
                    if ( taskNo < 0 || taskNo > 31 || taskNo != taskList[taskNo].index )
                        warning( "stop: Invalid taskNo\n" );
                    else
                        stop(taskList[taskNo].pid);
                }
            } else if ( strcmp( command, "continue" ) == 0 ) {
                if ( numTokens == 1 ) {
                    warning( "continue: no specified task\n" );
                } else if ( numTokens > 2 ) {
                    warning( "continue: too many arguments\n" );
                } else if ( numTasks == 0 ) {
                    warning( "continue: No tasks are running\n" );
                } else {
                    int taskNo = atoi(tokens[1]);
                    if ( taskNo < 0 || taskNo > 31 || taskList[taskNo].pid == 0 )
                        warning( "continue: Invalid taskNo\n" );
                    else
                        xcontinue(taskList[taskNo].pid);
                }
            } else if ( strcmp( command, "terminate" ) == 0 ) {
                if ( numTokens == 1 ) {
                    warning( "terminate: no specified task\n" );
                } else if ( numTokens > 2 ) {
                    warning( "terminate: too many arguments\n" );
                } else if ( numTasks == 0 ) {
                    warning( "terminate: No tasks are running\n" );
                } else {
                    int taskNo = atoi(tokens[1]);
                    if ( taskNo < 0 || taskNo > 31 || taskList[taskNo].pid == 0 )
                        warning( "terminate: Invalid taskNo\n" );
                    else {
                        terminate(taskList[taskNo].pid);

                        taskList[taskNo].index = 0;
                        taskList[taskNo].pid = 0;
                        numTasks--;
                    }
                }
            } else if ( strcmp( command, "check" ) == 0 ) {
                
            } else if ( strcmp( command, "exit" ) == 0 ) {
                exitFlag = 0;
                
                for ( int i = 0; i < NTASK; i++ ) {
                    if ( !taskList[i].pid == 0 )
                        terminate(taskList[i].pid);
                }

                break;
            } else if ( strcmp( command, "quit" ) == 0 ) {
                exitFlag = 1;
                break;
            } else {
                printf("msh379: %s: command not found\n", command);
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
    if ( endWallTime == -1 )
        warning( "Fail to end recording user CPU time\n" );

    printf("\n");
    printTime(endWallTime - startWallTime, &tmsStart, &tmsEnd);

    return exitFlag;
}

