#include <stdio.h>
#include <unistd.h>
#include "hijos.c"

#define ERROR -1


pid_t childPid;

//*********************
//     Método main
//*********************

int main(int argc, char **argv) {
    return creaHijos(childPid, argc, argv);
}
