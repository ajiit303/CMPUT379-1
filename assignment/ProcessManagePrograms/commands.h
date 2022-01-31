#include <signal.h>

#include "utils.h"


#ifndef COMMANDS_H
#define COMMANDS_H
#define NCMD 10
#define REQUIRE 0
#define OPTIONAL 1

struct CommandDB {
    char name[16];
    int numArgs;
    int optional;
};

void cdir (char *pathname);

void check (pid_t pid);

int checkArgs ( int cmdIndex, int numArgs );

int getTaskNo ( char *cmdName, char *strTaskNo, int numTasks, struct TaskDB *taskList );

int getcmdIndex (char *command);

void lstasks (struct TaskDB *taskList);

pid_t run ( char *pgm, char args[4] );

void stop ( pid_t pid );

void xcontinue ( pid_t pid );

void terminate ( pid_t pid );

void pdir ();

int xgetenv (char *envVar);

char *xgetcwd ();

#endif
