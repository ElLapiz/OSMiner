#include "fork.h"
#include "cpuMiner.h"
#include "memoryMiner.h"
#include "networkMIner.h"
#include "diskMinner.h"

#include <signal.h>
#include <sys/prctl.h>   //Para señales POSIX
#include <unistd.h>

#define ERROR -1

pid_t childPid;

int toAscii(char **param){
    return (int)param;
}

void asignaTrabajo(int numero) {
    switch (numero) {
        case 99:   //c
            collectCpuData(numero);
            break;
        case 100:  //d
            collectDiskData(numero);
            break;
        case 109:  //m
            collectMemData(numero);
            break;
        case 110: //n
            collectNetData(numero);
            break;
        deafult:
            break;
    }
    free(numero);
}

int creaHijos(pid_t childPid, int argc, char **argv) {
    int i, choice;

    for (i = 1; i <= sizeof(argc); i++) {
        switch (childPid = fork()) {
            case -1:
                return ERROR;
            case 0:
                signal(PR_SET_PDEATHSIG, SIGCHLD);
                choice = toAscii(argv[i][0]);
                asignaTrabajo(choice);
                _exit(0);
            default:
                sleep(1);
                break;
        }
    }
}
