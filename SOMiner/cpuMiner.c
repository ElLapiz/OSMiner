#include <pthread.h>
#include "cpuMiner.h"
#include "queue.c"

#define BUFFER_SIZE 1000

typedef struct data data;

data datos[50];
char default_tag[1000] = "aqui viene el tag del Json";  //TODO: hacer el timestamp y lo que haga falta para el JSON
pthread_mutex_t datos_lock;    //Mutex asignado para avisar del uso de "datos[]"

struct data creaStructData(int user, int nice, int system, int idle, int iowait, int irq, int softirq);

//Usar para testing


int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq);

void syncAgregaData(long user, long nice, long system, long idle, long iowait, long irq, long softirq);

struct data creaStructData(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    struct data data;
    data.metric = get_average_idle_percentage(user, nice, system, idle, iowait, irq, softirq);
    data.tag = &default_tag;
    return data;
}

int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    int average_idle = ( idle * 100 ) / ( user + nice + system + idle + iowait + irq + softirq );
    //printf("CPU disponible: %d% \n", average_idle);
    //printf("************************************************************************************************* \n");
    return average_idle;
}

/*void imprimeHijoCPU(int numero) {
    printf("Id proceso cpu: %d\n", (numero + 1));
}*/

data agregaData(long user, long nice, long system, long idle, long iowait, long irq, long softirq){

    pthread_mutex_lock(&datos_lock);  //bloquea datos_lock

    struct data data = creaStructData(user, nice, system, idle, iowait, irq, softirq);
    insert(data, datos);

    pthread_mutex_unlock(&datos_lock);  //desbloquea datos_lock
}

data extraeData(){
    pthread_mutex_lock(&datos_lock);  //bloquea datos_lock
    struct data data;
    data = removeData(datos);
    pthread_mutex_unlock(&datos_lock);  //desbloquea datos_lock
    return data;
    //TODO: mandar data a JSON, crear hilos
}



data syncExtraeData(){
    if (pthread_mutex_init(&datos_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        return extraeData();
    }
}



void syncAgregaData(long user, long nice, long system, long idle, long iowait, long irq, long softirq) {
    if (pthread_mutex_init(&datos_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        agregaData(user, nice, system, idle, iowait, irq, softirq);
    }
}



//************************
//********hilos***********
//************************

void* mandaJSON(void *arg){
    data data = syncExtraeData(); //datos a convertir
    printf("%d", data); //para efectos de prueba
    //TODO: convertir a JSON y mandar a servidor
}

void* minaDatos(void *arg) {
    char buffer[BUFFER_SIZE];
    char header[3];
    long int user, nice,system, idle, iowait,irq, softirq, other1, other2, other3;
    FILE * dato = fopen("/proc/stat", "r");
    fgets(buffer, BUFFER_SIZE, dato);
    printf("%s \n", buffer);
    sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &other1 , &other2, &other3);
    //printf("************************************************************************************************* \n");
    //imprimeHijoCPU(numero);
    syncAgregaData(user, nice, system, idle, iowait, irq, softirq);
    fclose(dato);
}


//Funcion principal

void collectCpuData(int numero) {
pthread_t minaDatos;
pthread_t mandaJson;

bool padre_vive = prctl(PR_GET_PDEATHSIG); //Recibe señal de muerte de padre

while(padre_vive){         //activa hilos
    pthread_create(&minaDatos, NULL , minaDatos , NULL);
    pthread_create(&mandaJson, NULL , mandaJSON , NULL);
}
pthread_mutex_destroy(&datos_lock); //destruye el mutex
}


