#include "utils.h"

#ifndef COMMANDS_H
#define COMMANDS_H

void cdir (char *pathname);

void lstasks ( struct TaskDB *taskList, int numTasks );

pid_t run ( char *pgm, char args[4] );

void pdir ();

int xgetenv (char *envVar);

char *xgetcwd ();

#endif
