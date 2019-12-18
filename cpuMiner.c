#include <pthread.h>
#include <sys/prctl.h>
#include "cpuMiner.h"
#include "queue.h"
#include "http.h"
#include <signal.h>
#include <curl/curl.h>

#define BUFFER_SIZE 1000
#define ERROR -1

typedef struct data data;

data datos_cpu[50];

char default_tag[16] = "cpu_metric";
static pthread_mutex_t cpu_lock = PTHREAD_MUTEX_INITIALIZER;

void extraeDataCpu(){
    int t;
    t = pthread_mutex_lock(&cpu_lock);  //bloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }

    struct data data;
    data = removeData(datos_cpu);
    publishData(data.metric, data.tag); //Publica en el server

    t = pthread_mutex_unlock(&cpu_lock);  //desbloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }
}

void syncExtraeDataCpu(){
    if (pthread_mutex_init(&cpu_lock, NULL) != 0)
    {
        sleep(1);
    }else {
        extraeDataCpu();
    }
}

int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    return ( idle * 100 ) / ( user + nice + system + idle + iowait + irq + softirq );
}

struct data creaStructDataCpu(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    struct data datos;
    datos.metric = get_average_idle_percentage(user, nice, system, idle, iowait, irq, softirq);
    datos.tag = &default_tag;
    return datos;
}

data agregaDataCpu(long user, long nice, long system, long idle, long iowait, long irq, long softirq){
    int t;
    t = pthread_mutex_lock(&cpu_lock);  //bloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }

    struct data info = creaStructDataCpu(user, nice, system, idle, iowait, irq, softirq);
    insert(info, datos_cpu);

    t = pthread_mutex_unlock(&cpu_lock);  //desbloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }
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

    //sleep(300); //Cada cuanto envia

    int t;

        t = pthread_mutex_lock(&cpu_lock); //Mutex
        if (t != 0){
            _exit(ERROR);
        }

        char buffer[BUFFER_SIZE];
        char header[3];
        long int user, nice,system, idle, iowait,irq, softirq, other1, other2, other3;

        FILE * dato = fopen("/proc/stat", "r");
        fgets(buffer, BUFFER_SIZE, dato);
        sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &other1 , &other2, &other3);
        fclose(dato);
        syncAgregaDataCpu(user, nice, system, idle, iowait, irq, softirq);
        t = pthread_mutex_unlock(&cpu_lock);
        if (t != 0){
            _exit(ERROR);
        }
}

void* enviarCpu(void *arg){
        sleep(1);//Esperar a que se llene la cola
        syncExtraeDataCpu(); //datos a convertir
}

//----------------------------------------------------------
//Funcion principal
//----------------------------------------------------------
void collectCpuData(int numero) {

    pthread_t minaDatosCpu, enviaCpu;
    int miner_thread, mutex;

    miner_thread = pthread_create(&minaDatosCpu, NULL, minarDatosCpu, NULL);
    if (miner_thread != 0){
        _exit(ERROR);
    }

    /*
    enviaCpu = pthread_create(&enviaCpu, NULL, enviarCpu, NULL);
    if (miner_thread != 0){
        _exit(ERROR);
    }
    */

    enviarCpu(NULL);

    //Clean
    mutex = pthread_mutex_destroy(&cpu_lock); //destruye el mutex
    if (mutex != 0){
        _exit(ERROR);
    }
    miner_thread = pthread_join(&minaDatosCpu, NULL);
    if (miner_thread != 0){
        _exit(ERROR);
    }

    free(mutex);
    free(miner_thread);
    exit(EXIT_SUCCESS);
}