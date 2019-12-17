#ifndef FORKPROCESS_MEMORYMINER_H
#define FORKPROCESS_MEMORYMINER_H

#include "memoryMiner.c"
#include <stdio.h>

void collectMemData(int numero);
static void* minarMemory(void *arg);
void syncAgregaDataMemory(long int total_activity);
data agregaDataMemory(long int total_activity);
void* enviarMemory(void *arg);
void syncExtraeDataMemory();
void extraeDataMemory();
#endif //FORKPROCESS_MEMORYMINER_H
