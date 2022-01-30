#include "commands.h"
#include "handler.h"
#include "utils.h"


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
        warning( "msh379: cdir: %s: No such file or directory\n", pathname );
}

void lstasks ( struct TaskDB *taskList, int numTask ) {
    printf( "%s %10s\n", "Index", "Pid" );
    for ( int i = 0; i < numTask; i++ )
        printf( "%d%15d\n", taskList[i].index, taskList[i].pid );
}

pid_t run ( char *pgm, char args[4] ) {

    return 1;
}

void pdir () {
    char *cwd = NULL;
    
    cwd = xgetcwd();

    if ( cwd != NULL )
        printf( "%s\n", cwd );

    free(cwd);
}

char *xgetcwd () {
    char *cwd = NULL;
    size_t bufferSize;

    cwd = pathAlloc(&bufferSize);

    if ( cwd == NULL ) {
        warning( "Failed to allocate memory for path string\n" );
    }

    if ( getcwd( cwd, bufferSize ) == NULL ) {
        warning( "Failed to get current directory\n" );
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
