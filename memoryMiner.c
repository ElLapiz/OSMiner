#include "memoryMiner.h"
#include <pthread.h>
#include <sys/prctl.h>
#include "queue.h"
#include "http.h"

#define BUFFER_SIZE 1000

typedef struct data data;

data datos_memory[50];

char default_tag_memory[16] = "memory_metric";
static pthread_mutex_t memory_lock = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------
//  Metodos sender
//------------------------------------------
struct data creaStructDataMemory(long int bytes) {
    struct data percentaje;
    percentaje.metric = bytes;
    percentaje.tag = &default_tag_memory;
    return percentaje;
}

data agregaDataMemory(long int bytes){
    int t;
    t = pthread_mutex_lock(&memory_lock);  //bloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }

    struct data info = creaStructDataMemory(bytes);
    insert(info, datos_memory);

    t = pthread_mutex_unlock(&memory_lock);  //desbloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }
}


void syncAgregaDataMemory(long int bytes) {
    if (pthread_mutex_init(&memory_lock, NULL) != 0){
        sleep(1);
    } else {
        agregaDataMemory(bytes);
    }
}


//------------------------------------------
//  Hilos
//------------------------------------------
static void* minarMemory(void *arg) {

    //sleep(300); //Cada cuanto envia

    int  t;
    t = pthread_mutex_lock(&memory_lock); //Mutex
    if (t != 0){
        _exit(ERROR);
    }

    FILE *ramInfo = fopen("/proc/meminfo", "r");
    char kb[2], header[10],  line[1024];
    long int memTotal, value[2], memory_idle;

    for (int i = 0; i < 3; ++i) {
        fgets(line, 1024, ramInfo);
        printf("%s", line);
        sscanf(line,"%s %d %s", header, &memTotal, kb);
        value[i] = memTotal;
    }

    fclose(ramInfo);
    memory_idle = (value[1] * 100) / value[2];
    syncAgregaDataMemory(memory_idle);
    t = pthread_mutex_unlock(&memory_lock);
    if (t != 0){
        _exit(ERROR);
    }
}

void extraeDataMemory(){
    int t;
    t = pthread_mutex_lock(&memory_lock);  //bloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }

    struct data data;
    data = removeData(datos_memory);
    publishData(data.metric, data.tag); //Publica en el server

    t = pthread_mutex_unlock(&memory_lock);  //desbloquea datos_lock
    if (t != 0){
        _exit(ERROR);
    }
}

void syncExtraeDataMemory(){
    if (pthread_mutex_init(&memory_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        extraeDataMemory();
    }
}


void* enviarMemory(void *arg){
    sleep(1);//Esperar a que se llene la cola por primera vez
    syncExtraeDataMemory(); //datos a convertir
}



void collectMemData(int numero) {

    pthread_t minaMemoria, sendMemoria;
    int t_minaMemoria, mutex;

    t_minaMemoria = pthread_create(&minaMemoria, NULL, minarMemory, NULL);
    if (t_minaMemoria != 0){
        _exit(ERROR);
    }

    /*
    s = pthread_create(&sendMemoria, NULL, enviarMemory, NULL);
    if (s != 0)
        printf("Error creando hilo de envio CPU");
    */

    enviarMemory(1);

    mutex = pthread_mutex_destroy(&memory_lock); //destruye el mutex
    if (mutex != 0){
        _exit(ERROR);
    }

    t_minaMemoria = pthread_join(&minaMemoria, NULL);
    if (t_minaMemoria != 0){
        _exit(ERROR);
    }
    free(datos_memory);
    free(mutex);
    exit(EXIT_SUCCESS);

}