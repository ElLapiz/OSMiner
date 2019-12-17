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
    pthread_mutex_lock(&memory_lock);  //bloquea datos_lock
    struct data info = creaStructDataMemory(bytes);
    insert(info, datos_memory);
    pthread_mutex_unlock(&memory_lock);  //desbloquea datos_lock
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
    int  s;
    s = pthread_mutex_lock(&memory_lock); //Mutex



    FILE *ramInfo = fopen("/proc/meminfo", "r");

    char kb[2], header[10],  line[1024];
    long int memTotal, value[2], memory_idle;

    for (int i = 0; i < 3; ++i) {
        fgets(line, 1024, ramInfo);
        printf("%s", line);
        sscanf(line,"%s %d %s", header, &memTotal, kb);
        value[i] = memTotal;
    }

    memory_idle = (value[1] * 100) / value[2];
    syncAgregaDataMemory(memory_idle);

    fclose(ramInfo);

    s = pthread_mutex_unlock(&memory_lock);
    if (s != 0)
        printf("Error desbloqueand mutex CPU");

}

void extraeDataMemory(){
    pthread_mutex_lock(&memory_lock);  //bloquea datos_lock
    struct data data;
    data = removeData(datos_memory);
    publishData(data.metric, data.tag); //Publica en el server
    pthread_mutex_unlock(&memory_lock);  //desbloquea datos_lock
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
    syncExtraeDataMemory(); //datos a convertir
}



void collectMemData(int numero) {
    pthread_t minaMemoria, sendMemoria;
    int t_minaMemoria;

    t_minaMemoria = pthread_create(&minaMemoria, NULL, minarMemory, NULL);
    if (t_minaMemoria != 0)
        printf("Error creando hilo de mineria CPU");

    sleep(1);//Esperar a que se llene la cola por primera vez

    /*
    s = pthread_create(&sendMemoria, NULL, enviarMemory, NULL);
    if (s != 0)
        printf("Error creando hilo de envio CPU");
    */

    enviarMemory(1);

    pthread_mutex_destroy(&memory_lock); //destruye el mutex
    pthread_join(&minaMemoria, NULL);
    free(datos_memory);
    //free(t_minaMemoria);
    exit(EXIT_SUCCESS);

}