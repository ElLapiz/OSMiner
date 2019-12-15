#include "networkMIner.h"

void imprimeHijoRed(int numero) {
    printf("Id proceso red: %d\n", (numero + 1));
}

void collectNetData(int numero) {

    char line[1024], header[10];
    long int bytes;

    FILE * dato = fopen("/proc/net/dev", "r");

    for (int i = 0; i < 3; ++i) {
        fgets(line, 1024, dato);
        printf("%s \n", line);
        sscanf(line,"%s %d ", header, &bytes);
    }
    printf("************************************************************************************************* \n");
    imprimeHijoRed(numero);
    printf("Cantidad de bytes transmitidos y recibidos: %d \n", bytes);
    printf("************************************************************************************************* \n");
    publishData(bytes, "network_metric");

    fclose(dato);
}
