#ifndef FORKPROCESS_DISKMINNER_H
#define FORKPROCESS_DISKMINNER_H

#include <stdio.h>
#include "diskMinner.c"

void collectDiskData(int numero);
static void* minarDisco(void *arg);
void syncAgregaDataDisco(long int total_activity);
data agregaDataDisk(long int total_activity);
void* envarDisco(void *arg);
void syncExtraeDataDisco();
void extraeDataDisco();
#endif //FORKPROCESS_DISKMINNER_H
