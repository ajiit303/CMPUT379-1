#include "commands.h"
#include "handler.h"
#include "utils.h"


void cdir (char *pathname) {
    // /path/to/dir absolute path
    // $ENV/path/to/ 
    // path/to/dir relative path

    if ( strlen(pathname) == 0 )
        return;

    char folders[MAXTOKENS][MAXTOKENLENGTH];
    char pathnameEnv[MAXTOKENS * MAXTOKENLENGTH];
    
    memset( &folders, 0, sizeof(folders) );
    memset( &pathnameEnv, 0, sizeof(pathnameEnv) );

    int numFolders = split( pathname, folders, "/" );
    for (int i = 0; i < numFolders; i++) {
        
        if ( startWith( folders[i], '$' ) ) {
            char *path = NULL;

            gsub( folders[i], "$" );

            path = xgetenv( folders[i] );
            if ( path != NULL )
                strcpy( folders[i], path );

        } else if ( strchr( folders[i], '$' ) )
            gsub( folders[i], "$" );

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

void pdir () {
    char *cwd = NULL;
    
    cwd = xgetcwd();

    if ( cwd != NULL )
        printf( "%s\n", cwd );
}

char *xgetcwd () {
    char *cwd = NULL;
    size_t bufferSize;

    cwd = pathAlloc(&bufferSize);

    if ( cwd == NULL ) {
        warning( "Failed to allocate memory for path string" );
    }

    if ( getcwd( cwd, bufferSize ) == NULL ) {
        warning( "Failed to get current directory" );
    }

    return cwd;
}

char *xgetenv (char *env) {
    char *path = NULL;
    
    path = getenv(env);

    return path;
}

