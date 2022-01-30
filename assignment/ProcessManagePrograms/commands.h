#include <signal.h>

#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

void cdir (char *pathname);

void check (pid_t pid);

int getSigacts (char *command);

void lstasks (struct TaskDB *taskList);

pid_t run ( char *pgm, char args[4] );

void stop ( pid_t pid );

void xcontinue ( pid_t pid );

void terminate ( pid_t pid );

void pdir ();

int xgetenv (char *envVar);

char *xgetcwd ();

#endif
