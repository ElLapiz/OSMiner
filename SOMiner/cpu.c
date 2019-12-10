#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

FILE *dato;

char buffer[BUFFER_SIZE];
int totalRead = 0;


void getCPUData() {
    dato = fopen("/proc/stat", "r");
 while (fgets(buffer, BUFFER_SIZE, dato) != NULL){
     totalRead = strlen(buffer);
     buffer[totalRead - 1] = buffer[totalRead -1] == "\n"
                                ? '\0'
                                : buffer [totalRead - 1];
 }//while ends
 fclose(dato);
}