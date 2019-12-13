#include "cpuMiner.h"

#define BUFFER_SIZE 1000

void imprimeHijoCPU(int numero) {
    printf("Id proceso cpu: %d\n", (numero + 1));
}

void get_average_idle_percentage(int user, int nice,int system, int idle, int iowait, int irq, int softirq) {
    int average_idle = ( idle * 100 ) / ( user + nice + system + idle + iowait + irq + softirq );
    printf("CPU disponible: %d% \n", average_idle);
}

void collectCpuData(int numero) {
    imprimeHijoCPU(numero);

    char buffer[BUFFER_SIZE];
    char header[3];
    long int user, nice,system, idle, iowait,irq, softirq, other1, other2, other3;

    FILE * dato = fopen("/proc/stat", "r");
    fgets(buffer, BUFFER_SIZE, dato);
    printf("%s \n", buffer);

    sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &other1 , &other2, &other3);

    get_average_idle_percentage(user, nice,system, idle, iowait,irq, softirq);

    fclose(dato);
}
