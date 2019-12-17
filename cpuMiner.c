#include <pthread.h>
#include <sys/prctl.h>
#include "cpuMiner.h"
#include "queue.h"
#include "http.h"

#define BUFFER_SIZE 1000

typedef struct data data;

data datos[50];

char default_tag[16] = "cpu_metric";
static pthread_mutex_t cpu_lock = PTHREAD_MUTEX_INITIALIZER;


void extraeDataCpu(){
    pthread_mutex_lock(&cpu_lock);  //bloquea datos_lock
    struct data data;
    data = removeData(datos);
    publishData(data.metric, data.tag); //Publica en el server
    pthread_mutex_unlock(&cpu_lock);  //desbloquea datos_lock
}

void syncExtraeDataCpu(){
    if (pthread_mutex_init(&cpu_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        extraeDataCpu();
    }
}


int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    int average_idle = ( idle * 100 ) / ( user + nice + system + idle + iowait + irq + softirq );
    printf("CPU disponible: %d% \n", average_idle);
    return average_idle;
}

struct data creaStructDataCpu(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    struct data percentaje;
    percentaje.metric = get_average_idle_percentage(user, nice, system, idle, iowait, irq, softirq);
    percentaje.tag = &default_tag;
    return percentaje;
}

data agregaDataCpu(long user, long nice, long system, long idle, long iowait, long irq, long softirq){
    pthread_mutex_lock(&cpu_lock);  //bloquea datos_lock
    struct data info = creaStructDataCpu(user, nice, system, idle, iowait, irq, softirq);
    insert(info, datos);
    pthread_mutex_unlock(&cpu_lock);  //desbloquea datos_lock
}


void syncAgregaDataCpu(long user, long nice, long system, long idle, long iowait, long irq, long softirq) {
    if (pthread_mutex_init(&cpu_lock, NULL) != 0){
        sleep(1);
    } else {
        agregaDataCpu(user, nice, system, idle, iowait, irq, softirq);
    }
}

//------------------------------------------------------
//          hilos
//------------------------------------------------------
static void* minarDatosCpu(void *arg) {
    int  s;
    s = pthread_mutex_lock(&cpu_lock); //Mutex

    char buffer[BUFFER_SIZE];
    char header[3];
    long int user, nice,system, idle, iowait,irq, softirq, other1, other2, other3;
    FILE * dato = fopen("/proc/stat", "r");
    fgets(buffer, BUFFER_SIZE, dato);
    printf("%s \n", buffer);
    sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &other1 , &other2, &other3);
    syncAgregaDataCpu(user, nice, system, idle, iowait, irq, softirq);
    fclose(dato);

    s = pthread_mutex_unlock(&cpu_lock);
    if (s != 0)
        printf("Error desbloqueand mutex CPU");
}

void* enviarCpu(void *arg){
    syncExtraeDataCpu(); //datos a convertir
}

//----------------------------------------------------------
//Funcion principal
//----------------------------------------------------------
void collectCpuData(int numero) {

    pthread_t minaDatosCpu, enviaCpu;
    int s;

        s = pthread_create(&minaDatosCpu, NULL, minarDatosCpu, NULL);
        if (s != 0)
            printf("Error creando hilo de mineria CPU");

        sleep(1);//Esperar a que se llene la cola por primera vez

        /*
        s = pthread_create(&enviaCpu, NULL, enviaDisco, NULL);
        if (s != 0)
            printf("Error creando hilo de envio CPU");
        */

    enviarCpu(1);
    pthread_mutex_destroy(&cpu_lock); //destruye el mutex
    exit(EXIT_SUCCESS);
}