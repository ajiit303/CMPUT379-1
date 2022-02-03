#include <signal.h>

#include "utils.h"


#ifndef COMMANDS_H
#define COMMANDS_H
#define NCMD 10
#define NARG 6
#define MAXBUF 1024
#define REQUIRE 0
#define OPTIONAL 1

/**
 * @brief A struct for representation of a command
 */
struct CommandDB {
    char name[16]; // name of the command
    int numArgs; // number of required arguments
    int optional; // arguments is optional or mandatory
};

void cdir (char *pathname);

void check (char *targetId);

int checkArgs ( int cmdIndex, int numArgs );

int getTaskNo ( char *cmdName, char *strTaskNo, int numTasks, struct TaskDB *taskList );

int getcmdIndex (char *command);

int isChildPs ( int ppid, int pidList[], int numPid );

void lstasks (struct TaskDB *taskList);

pid_t run ( char *pgm, char args[][MAXCHARS], int numArgs );

int stop ( pid_t pid );

int xcontinue ( pid_t pid );

void terminate ( pid_t pid );

void pdir ();

void xexit ( clock_t startWallTime, struct tms *tmsStart, struct TaskDB *taskList );

int xgetenv (char *envVar);

char *xgetcwd ();

#endif
