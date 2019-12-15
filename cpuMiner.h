#ifndef SISTEMINFOMINERS_CPUMINER_H
#define SISTEMINFOMINERS_CPUMINER_H

#include <stdio.h>
#include "cpuMiner.c"

void collectCpuData(int numero);
static void* minarDatos(void *arg);
void* mandarJSON(void *arg);
void syncAgregaData(long user, long nice, long system, long idle, long iowait, long irq, long softirq);
data syncExtraeData();
data extraeData();
data agregaData(long user, long nice, long system, long idle, long iowait, long irq, long softirq);
int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq);
struct data creaStructData(int user, int nice, int system, int idle, int iowait, int irq, int softirq);

#endif //SISTEMINFOMINERS_CPUMINER_H