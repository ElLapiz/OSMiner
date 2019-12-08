#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.c"


int get_average_idle_percentage(int *pInt);

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
    char  *data =   strtok(readFile(), " ");

    //Parse char to int
    int *numeric_data = parse_char_to_int(data, 9);

    printf("hola mundo");

    //get avegare idle percentage
    int idle = get_average_idle_percentage(numeric_data);
    ver_idle(1);
}

int get_average_idle_percentage(int *numeric_data) {
    int average_idle =  (numeric_data[3] * 100) / (numeric_data[0] + numeric_data[1] + numeric_data[2] +
            numeric_data[3] + numeric_data[4] + numeric_data[5] + numeric_data[6]);
    return average_idle;
}



