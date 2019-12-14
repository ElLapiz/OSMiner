#ifndef SISTEMINFOMINERS_FORK_H
#define SISTEMINFOMINERS_FORK_H

#include "fork.c"
#include <stdio.h>
#include <unistd.h>

int toAscii(char **param);
void asignaTrabajo(int numero);
int creaHijos(pid_t childPid, int argc, char **argv);

#endif //SISTEMINFOMINERS_FORK_H
