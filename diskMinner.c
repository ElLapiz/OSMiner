#include "diskMinner.h"
#include <pthread.h>
#include <sys/prctl.h>
#include "queue.h"
#include "http.h"

#define BUFFER_SIZE 1000

typedef struct data data;

data datos_disk[50];

char default_tag_disk[16] = "disk_metric";
static pthread_mutex_t disk_lock = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------
//  Metodos sender
//------------------------------------------
struct data creaStructDisckData(long int total_activity) {
    struct data percentaje;
    percentaje.metric = total_activity;
    percentaje.tag = &default_tag_disk;
    return percentaje;
}

data agregaDataDisk(long int total_activity){
    pthread_mutex_lock(&disk_lock);  //bloquea datos_lock
    struct data info = creaStructDisckData(total_activity);
    insert(info, datos_disk);
    pthread_mutex_unlock(&disk_lock);  //desbloquea datos_lock
}


void syncAgregaDataDisco(long int total_activity) {
    if (pthread_mutex_init(&disk_lock, NULL) != 0){
        sleep(1);
    } else {
        agregaDataDisk(total_activity);
    }
}


//------------------------------------------
//  Hilos
//------------------------------------------
static void* minarDisco(void *arg) {
   pthread_mutex_lock(&disk_lock); //Mutex


    char line[1024], valor3[10];
    long int valor1, valor2, reads, valor5, valor6, valor7, lectures, reads_sum, lectures_sum, total_activity;
    FILE * dato = fopen("/proc/diskstats", "r");

    for (int i = 0; i < 17; ++i) {
        fgets(line, 1024, dato);
        sscanf(line,"%d %d %s %d %d %d %d %d", &valor1, &valor2, valor3, &reads, &valor5, &valor6, &valor7, &lectures);
        reads_sum += reads;
        lectures_sum += lectures;
    }
    total_activity = reads_sum + lectures_sum;

    syncAgregaDataDisco(total_activity);
    fclose(dato);

    pthread_mutex_unlock(&disk_lock);
}

void extraeDataDisco(){
    pthread_mutex_lock(&disk_lock);  //bloquea datos_lock
    struct data data;
    data = removeData(datos_disk);
    publishData(data.metric, data.tag); //Publica en el server
    pthread_mutex_unlock(&disk_lock);  //desbloquea datos_lock
}

void syncExtraeDataDisco(){
    if (pthread_mutex_init(&disk_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        extraeDataDisco();
    }
}


void* envarDisco(void *arg){
    syncExtraeDataDisco(); //datos a convertir
}

void collectDiskData(int numero) {

    pthread_t minaDisco, sendDisco;
    int t_minaDisco;

    t_minaDisco = pthread_create(&minaDisco, NULL, minarDisco, NULL);
    if (t_minaDisco != 0)
        printf("Error creando hilo de mineria CPU");

    sleep(1);//Esperar a que se llene la cola por primera vez

    /*
    s = pthread_create(&mandaJson, NULL, sendDisco, NULL);
    if (s != 0)
        printf("Error creando hilo de envio CPU");
    */

    envarDisco(1);

    pthread_mutex_destroy(&disk_lock); //destruye el mutex
    pthread_join(&minaDisco, NULL);
    //free(datos_disk);
    free(t_minaDisco);
    exit(EXIT_SUCCESS);

}
