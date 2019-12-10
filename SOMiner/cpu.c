#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

FILE *data;
data = fopen("/proc/stat", "rb");
char buffer[BUFFER_SIZE];
int totalRead = 0;


void collectCPUData(int numero) {
 while (fgets(buffer, BUFFER_SIZE, data)!= NULL){
     totalRead = strlen(buffer);
     buffer[totalRead - 1] = buffer[totalRead -1] == "\n"
                                ? '\0'
                                : buffer [totalRead - 1]; }//while ends
}