#include "diskMinner.h"

void imprimeHijoDisk(int numero) {
    printf("Id proceso disco: %d\n", (numero + 1));
}

void collectDiskData(int numero) {

    char line[1024], valor3[10];
    long int valor1, valor2, reads, valor5, valor6, valor7, lectures, reads_sum, lectures_sum, total_activity;

    FILE * dato = fopen("/proc/diskstats", "r");

    for (int i = 0; i < 17; ++i) {
        fgets(line, 1024, dato);
        sscanf(line,"%d %d %s %d %d %d %d %d", &valor1, &valor2, valor3, &reads, &valor5, &valor6, &valor7, &lectures);
        reads_sum += reads;
        lectures_sum += lectures;
    }

    total_activity = reads_sum + lectures_sum;
    printf("************************************************************************************************* \n");
    imprimeHijoDisk(numero);
    printf("La antidad de actividad de IO en el disco es un total de: %d \n", total_activity);
    printf("************************************************************************************************* \n");

    fclose(dato);
}
