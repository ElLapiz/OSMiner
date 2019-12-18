#ifndef SISTEMINFOMINERS_CPUMINER_H
#define SISTEMINFOMINERS_CPUMINER_H

#include <stdio.h>
#include "cpuMiner.c"

void collectCpuData(int numero);
static void* minarDatosCpu(void *arg);
void* enviarCpu(void *arg);
void syncAgregaDataCpu(long user, long nice, long system, long idle, long iowait, long irq, long softirq);
void syncExtraeDataCpu();
void extraeDataCpu();
data agregaDataCpu(long user, long nice, long system, long idle, long iowait, long irq, long softirq);
int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq);
struct data creaStructDataCpu(int user, int nice, int system, int idle, int iowait, int irq, int softirq);

#endif //SISTEMINFOMINERS_CPUMINER_H