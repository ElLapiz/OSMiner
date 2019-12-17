#ifndef FORKPROCESS_NETWORKMINER_H
#define FORKPROCESS_NETWORKMINER_H

#include "networkMIner.c"

void collectNetData(int numero);
static void* minarNetwork(void *arg);
void syncAgregaDataNetwork(long int total_activity);
data agregaDataNetwork(long int total_activity);
void* enviarNetwork(void *arg);
void syncExtraeDataNetwork();
void extraeDataNetwork();

#endif //FORKPROCESS_NETWORKMINER_H
