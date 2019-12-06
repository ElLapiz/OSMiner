#include <stdio.h>
#include <unistd.h>
#include <string.h>

void collectCpuData(int numero) {
    imprimeHijo(numero);
    printf("Collecting cpu data...\n");

    FILE *cpuInfo = fopen("/proc/stat", "rb");
    static char line[1024];

    while (fgets(line, 1024, cpuInfo) != NULL) {
        if (strstr(line, "cpu") != NULL) {
            puts(line);
        }
    }
}