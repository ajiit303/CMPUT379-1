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
    pid_t parent_pid;

    char inStr[MAXLEN];
    char tokens[MAXNTOKENS][MAXCHARS];
    struct TaskDB taskList[NTASK];

    memset( &inStr, 0, sizeof(inStr) );
    memset( &tokens, 0, sizeof(tokens) );
    
    for ( int i = 0; i < NTASK; i++ ) {
        taskList[i].index = -1;
        taskList[i].pid = -1;
    }

    numTasks = 0;
    parent_pid = getpid();


 /* --------- Main Loop --------- */
    printf( "msh379 [%d]: ", parent_pid );
    while( scanf("%[^\n]", inStr) != EOF) {
        getchar();

        int numTokens = split( inStr, tokens, " " );

        if ( numTokens == 0 ) { 
            printf( "msh379 [%d]: ", parent_pid );
            continue;
        }

        char *command = tokens[0];
        int cmdIndex = getcmdIndex( command );

        if ( cmdIndex == -1 ) {
            warning( "msh379: %s: command not found\n" );
            printf( "msh379 [%d]: ", parent_pid );
            memset( inStr, 0, sizeof(inStr) );
            memset( tokens, 0, sizeof(tokens) );
            continue;
        }
        
        if ( checkArgs( cmdIndex, numTokens - 1 ) == 0 ) {
            printf( "msh379 [%d]: ", parent_pid );
            memset( inStr, 0, sizeof(inStr) );
            memset( tokens, 0, sizeof(tokens) );
            continue;
        }

        switch(cmdIndex) {
            case 0:
                cdir(tokens[1]); break;
            case 1:
                pdir(); break;
            case 2:
                lstasks(taskList); break;
            case 3:
                if (numTasks == NTASK - 1) {
                    warning( "run: number of task limit reached, cannot run more task\n" ); break;
                }

                if ( numTokens > 2 && numTokens > 6 ) {
                    warning( "run: too many arguments for program %s\n", tokens[1] ); break;
                }

                pid_t pid;

                pid = run( tokens[1], tokens, numTokens - 2 );
                if ( pid == 0 ) return 0;

                if ( pid > 0 ) {
                    sleep(1);
                    char command[MAXCHARS];
                    memset( command, 0, sizeof(command) );

                    for ( int i = 1; i < numTokens; i++ ) {
                        strcat( command, " " );
                        strcat( command, tokens[i] );
                    }

                    for ( int i = 0; i < NTASK; i++ ) {
                        if ( taskList[i].index == -1 && taskList[i].pid == -1 ) {
                            taskList[i].index = i;
                            taskList[i].pid = pid;
                            strcpy( taskList[i].command, command );
                            numTasks++;
                            break;
                        }
                    }
                }

                break;
            case 4: {
                int taskNo = getTaskNo( tokens[0], tokens[1], numTasks, taskList );
                if ( taskNo == -1 ) break;
                stop(taskList[taskNo].pid);
                
                break;
            }
            case 5: {
                int taskNo = getTaskNo( tokens[0], tokens[1], numTasks, taskList );
                if ( taskNo == -1 ) break;
                xcontinue(taskList[taskNo].pid);
                
                break;
            }
            case 6: {
                int taskNo = getTaskNo( tokens[0], tokens[1], numTasks, taskList );
                if ( taskNo == -1 ) break;
                    
                terminate(taskList[taskNo].pid);

                taskList[taskNo].index = -1;
                taskList[taskNo].pid = -1;

                numTasks--;

                break;
            }
            case 7:
                check(tokens[1]);
                break;
            case 8:
                xexit( startWallTime, &tmsStart, taskList );
                return 0;
            case 9:
                xexit( startWallTime, &tmsStart, NULL );
                return 0;
        }

        printf( "msh379 [%d]: ", parent_pid );
        memset( inStr, 0, sizeof(inStr) );
        memset( tokens, 0, sizeof(tokens) );        
    }
}