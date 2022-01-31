#include <sys/wait.h>

#include "commands.h"
#include "handler.h"
#include "utils.h"


static const struct CommandDB commands[NCMD] = {
    {"cdir", 1, REQUIRE},
    {"pdir", 0, OPTIONAL},
    {"lstasks", 0, OPTIONAL},
    {"run", 5, OPTIONAL},
    {"stop", 1, REQUIRE},
    {"continue", 1, REQUIRE},
    {"terminate", 1, REQUIRE},
    {"check", 1, REQUIRE},
    {"exit", 0, OPTIONAL},
    {"quit", 0, OPTIONAL}
};

void cdir (char *pathname) {

    if ( strlen(pathname) == 0 )
        return;

    char folders[MAXNTOKENS][MAXCHARS];
    char envVars[MAXCHARS][MAXCHARS];
    char pathnameEnv[MAXLEN];
    
    memset( &folders, 0, sizeof(folders) );
    memset( &envVars, 0, sizeof(envVars) );
    memset( &pathnameEnv, 0, sizeof(pathnameEnv) );

    int numFolders = split( pathname, folders, "/" );
    for ( int i = 0; i < numFolders; i++ ) {
        
        if ( strchr( folders[i], '$' ) ) {
            int numMatch = gsub( folders[i], "$", "|&" );

            int numEnvVar = split( folders[i], envVars, "|" );

            memset(folders[i], 0, sizeof(folders[i]));

            for ( int j = 0; j < numEnvVar; j++) {
                
                if ( startWith( envVars[j], '&' ) ) {
                    char *envPath = NULL;
                    size_t bufferSize;
                    
                    envPath = pathAlloc(&bufferSize);
                    
                    gsub( envVars[j], "&", "" );
                    strcpy( envPath, envVars[j] );
                    if ( envPath != NULL && xgetenv(envPath) != 0) {
                        memset( envVars[j], 0, sizeof(envVars[j]) );
                        strcpy( envVars[j], envPath );
                    }

                    free(envPath);
                } 
                    
                strcat( folders[i], envVars[j] );
            }
        }

        strcat( pathnameEnv, folders[i] );
        strcat( pathnameEnv, "/" );
    }

    if ( !startWith(pathnameEnv, '/' ) ) {
        char *cwd = NULL;

        cwd = xgetcwd();

        if ( cwd != NULL ) {
            strcat( cwd, "/" );
            strcat( cwd, pathnameEnv );
            strcpy( pathnameEnv, cwd );
        }
    }

    if ( chdir(pathnameEnv) < 0 )
        warning( "cdir: %s: No such file or directory\n", pathname );
    else 
        printf( "cdir: change directory successfully\n" );
}

int checkArgs ( int cmdIndex, int numArgs ) {
    if ( commands[cmdIndex].optional )
        return 1;
    
    if ( numArgs > commands[cmdIndex].numArgs ) {
        warning( "%s: too many arugments\n", commands[cmdIndex].name );
        return 0;
    }

    if ( numArgs < commands[cmdIndex].numArgs ) {
        warning( "%s: not enought arguments\n", commands[cmdIndex].name );
        return 0;
    }

    return 1;
}

int getTaskNo ( char *cmdName, char *strTaskNo, int numTasks, struct TaskDB *taskList ) {
    int taskNo = -1;
 
    if ( numTasks == 0 ) {
        warning( "%s: No tasks are running\n", cmdName );
        return taskNo;
    }

    taskNo = atoi(strTaskNo);
    if ( taskNo < 0 || taskNo > NTASK || taskList[taskNo].pid == -1 ) {
        warning( "%s: Invalid taskNo\n", cmdName );
        taskNo = -1;
    }

    return taskNo;
}

int getcmdIndex (char *command) {
    int cmdIndex = -1;

    for (int i = 0; i < NCMD; i++) {
        if ( strcmp( commands[i].name, command ) == 0 ) {
            cmdIndex = i;
            break;
        }
    }

    return cmdIndex;
}

void lstasks (struct TaskDB *taskList) {
    printf( "%s %10s\n", "Index", "Pid" );
    for ( int i = 0; i < NTASK; i++ ) {
        if (taskList[i].pid != -1)
            printf( "%d%15d\n", taskList[i].index, taskList[i].pid );
    }
}

void pdir () {
    char *cwd = NULL;
    
    cwd = xgetcwd();

    if ( cwd != NULL )
        printf( "%s\n", cwd );

    free(cwd);
}

pid_t run ( char *pgm, char **args ) {
    pid_t pid;

    if ( ( pid = fork() ) < 0 ) 
        warning( "run: fork error\n" );

    if ( pid > 0 )
        printf( "run: new processes is created\n" );
    
    if ( pid == 0 && strlen(pgm) != 0 ) {
        
        
        for ( int i = 2; i < 6; i++ ) {

        }
    }

    return pid;
}

void stop ( pid_t pid ) {
    if ( kill( pid, SIGSTOP ) == -1)
        fatal( "stop: cannot stop task %d\n", pid );
    else {
        int status;
        if ( waitpid( pid, &status, WUNTRACED ) != pid )
            fatal( "stop: cannot get status of task %d\n" );
        
        if ( !WSTOPSIG(status) )
            fatal( "stop: cannot stop task %d\n" );
        else
            printf( "stop: task %d stopped\n", pid );
    }
}

void terminate ( pid_t pid ) {
    if ( kill( pid, SIGKILL ) == -1 )
        fatal( "stop: cannot stop task %d\n", pid );
    else {
        int status;
        if ( waitpid( pid, &status, WUNTRACED ) != pid )
            fatal( "terminate: cannot get status of task %d\n", pid );
        
        if ( !WIFSIGNALED(status) )
            fatal( "terminate: cannot terminate task %d\n", pid );
        else
            printf( "terminate: task %d terminated\n", pid );
    }
}

void xcontinue ( pid_t pid ) {
    if ( kill( pid, SIGCONT ) == -1 )
        fatal( "continue: cannot continue task %d\n", pid );
    else {
        int status;
        if ( waitpid( pid, &status, WCONTINUED ) != pid )
            fatal( "continue: cannot get status of task %d\n", pid );
        
        if ( !WIFCONTINUED(status) )
            fatal( "continue: cannot continue task %d\n", pid );
        else
            printf( "continue: task %d is resumed\n", pid );
    }
}

char *xgetcwd () {
    char *cwd = NULL;
    size_t bufferSize;

    cwd = pathAlloc(&bufferSize);

    if ( cwd == NULL ) {
        warning( "xgetcwd: cannot allocate memory for path string\n" );
    }

    if ( getcwd( cwd, bufferSize ) == NULL ) {
        warning( "xgetcwd:cannot get current directory\n" );
    }

    return cwd;
}

int xgetenv (char *envVar) {
    char *path = NULL;
    
    path = getenv(envVar);

    if (path == NULL)
        return 0;

    memset( envVar, 0, sizeof(envVar) );
    strcpy( envVar, path );

    return 1;
}
