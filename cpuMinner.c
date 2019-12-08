#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.c"

char * readFile(){
    FILE *cpuInfo = fopen("/proc/stat", "rb");
    static char line[1024];
    while (fgets(line, 1024, cpuInfo) != NULL) {
        if (strstr(line, "cpu") != NULL) {
            puts(line);
            break;
        }
    }
    fclose(cpuInfo);
    return line;
}

void collectCpuData(int numero) {
    imprimeHijo(numero);
    printf("Collecting cpu data...\n");

    //Read /proc/stat
    char  * data =   strtok(readFile(), " ");
    int * information = parse_char_to_int(data, 9);

    //get avegare idle percentage
    //get_average_idle_percentage(data);
}


