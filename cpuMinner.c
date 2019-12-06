#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void get_average_idle_percentage(char *data);

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

int isNumeric (const char * s){
    if (s == NULL || *s == '\0' || isspace(*s))
        return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

void get_average_idle_percentage(char *token) {
    int sum;

    while(token != NULL){
        if(isNumeric(token)){
            //sum = (int)token;
            printf("%s\n", token);
        }
        token = strtok(NULL, " ");
    }
}

void collectCpuData(int numero) {
    imprimeHijo(numero);
    printf("Collecting cpu data...\n");

    //Read /proc/stat
    char  * data =   strtok(readFile(), " ");

    //get avegare idle percentage
    get_average_idle_percentage(data);
}


