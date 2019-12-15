#ifndef SISTEMINFOMINERS_CPUMINER_H
#define SISTEMINFOMINERS_CPUMINER_H

#include <stdio.h>
#include "cpuMiner.c"

void collectCpuData(int numero);
void imprimeHijoCPU(int numero);
int get_average_idle_percentage(int user, int nice,int system, int idle, int iowait, int irq, int softirq);

#endif //SISTEMINFOMINERS_CPUMINER_H
