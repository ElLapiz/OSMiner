void imprimeHijo(int numero) {
    printf("Hijo : %d\n", (numero + 1));
}

//void collectCpuData(int numero) {
    imprimeHijo(numero);
    printf("Hola vengo a recolectar datos de CPU\n");
    sleep(1);
}

void collectMemData(int numero) {
    imprimeHijo(numero);
    printf("Hola vengo a recolectar datos de memoria\n");
    sleep(1);
}

void collectNetData(int numero) {
    imprimeHijo(numero);
    printf("Hola vengo a recolectar datos de red\n");
    sleep(1);
}

void collectDiskData(int numero) {
    imprimeHijo(numero);
    printf("Hola vengo a recolectar datos de disco\n");
    sleep(1);
}