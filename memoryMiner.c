#include "memoryMiner.h"

void imprimeHijoMemoria(int numero) {
    printf("Id proceso memoria: %d\n", (numero + 1));
}

void collectMemData(int numero) {

    FILE *ramInfo = fopen("/proc/meminfo", "r");

    char kb[2], header[10],  line[1024];
    long int memTotal, value[2], memory_idle;

    for (int i = 0; i < 3; ++i) {
        fgets(line, 1024, ramInfo);
        printf("%s", line);
        sscanf(line,"%s %d %s", header, &memTotal, kb);
        value[i] = memTotal;
    }

    memory_idle = (value[1] * 100) / value[2];

    printf("************************************************************************************************* \n");
    imprimeHijoMemoria(numero);
    printf("Memoria disponible: %d % \n", memory_idle);
    printf("************************************************************************************************* \n");

    fclose(ramInfo);
}