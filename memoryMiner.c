#include "memoryMiner.h"
#define BUFFER_SIZE 1000

/*
void imprimeHijo(int numero) {
    printf("Id proceso: %d\n", (numero + 1));
}
*/

void collectMemData(int numero) {
    //imprimeHijo(numero);

    FILE *ramInfo = fopen("/proc/meminfo", "rb");
    char line[1024];
    char kb[2];
    char header[10];
    long int memTotal;

    for (int i = 0; i < 2; ++i) {
        fgets(line, 1024, ramInfo);
        printf("%s", line);
        sscanf(line,"%s %d %s", header, &memTotal, kb);
    }

    fclose(ramInfo);
}
