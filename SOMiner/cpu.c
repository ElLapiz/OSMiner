#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

int main() {
    FILE *dato;

    char buffer[BUFFER_SIZE];
    char header[3];
    long int n1;
    long int n2;
    long int n3;
    long int n4;
    long int n5;
    long int n6;
    long int n7;
    long int n8;
    long int n9;
    long int n10;

    dato = fopen("/proc/stat", "r");
    fgets(buffer, BUFFER_SIZE, dato);
    printf("%s \n", buffer);

    sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9, &n10);

    printf("%d", n2);

    fclose(dato);


}