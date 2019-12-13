#include <unistd.h>
#include "fork.h"

#define ERROR -1


pid_t childPid;

int main(int argc, char **argv) {
    return creaHijos(childPid, argc, argv);
}