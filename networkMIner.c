#include "networkMIner.h"
#include <pthread.h>
#include <sys/prctl.h>
#include "queue.h"
#include "http.h"


#define BUFFER_SIZE 1000

typedef struct data data;

data datos_net[50];

char default_tag_network[16] = "network_metric";
static pthread_mutex_t network_lock = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------
//  Metodos sender
//------------------------------------------
struct data creaStructDataNetwork(long int total_activity) {
    struct data percentaje;
    percentaje.metric = total_activity;
    percentaje.tag = &default_tag_network;
    return percentaje;
}

data agregaDataNetwork(long int total_activity){
    pthread_mutex_lock(&network_lock);  //bloquea datos_lock
    struct data info = creaStructDataNetwork(total_activity);
    insert(info, datos_net);
    pthread_mutex_unlock(&network_lock);  //desbloquea datos_lock
}


void syncAgregaDataNetwork(long int total_activity) {
    if (pthread_mutex_init(&network_lock, NULL) != 0){
        sleep(1);
    } else {
        agregaDataNetwork(total_activity);
    }
}


//------------------------------------------
//  Hilos
//------------------------------------------
static void* minarNetwork(void *arg) {
    int  s;
    s = pthread_mutex_lock(&network_lock); //Mutex

    char line[1024], header[10];
    long int bytes;

    FILE * dato = fopen("/proc/net/dev", "r");

    for (int i = 0; i < 3; ++i) {
        fgets(line, 1024, dato);
        printf("%s \n", line);
        sscanf(line,"%s %d ", header, &bytes);
    }

    syncAgregaDataNetwork(bytes);
    fclose(dato);

    s = pthread_mutex_unlock(&network_lock);
    if (s != 0)
        printf("Error desbloqueand mutex CPU");

}

void extraeDataNetwork(){
    pthread_mutex_lock(&network_lock);  //bloquea datos_lock
    struct data data;
    data = removeData(datos_net);
    publishData(data.metric, data.tag); //Publica en el server
    pthread_mutex_unlock(&network_lock);  //desbloquea datos_lock
}

void syncExtraeDataNetwork(){
    if (pthread_mutex_init(&network_lock, NULL) != 0)
    {
        sleep(500);
    }else {
        extraeDataNetwork();
    }
}


void* enviarNetwork(void *arg){
    syncExtraeDataNetwork(); //datos a convertir
}


void collectNetData(int numero) {

    pthread_t minaNetwork, sendNetwork;
    int t_minaNetwork;

    t_minaNetwork = pthread_create(&minaNetwork, NULL, minarNetwork, NULL);
    if (t_minaNetwork != 0)
        printf("Error creando hilo de mineria CPU");

    sleep(1);//Esperar a que se llene la cola por primera vez

    /*
    s = pthread_create(&sendNetwork, NULL, enviarNetwork, NULL);
    if (s != 0)
        printf("Error creando hilo de envio CPU");
    */

    enviarNetwork(1);

    pthread_mutex_destroy(&network_lock); //destruye el mutex
    pthread_join(&minaNetwork, NULL);
    pthread_cancel(&minaNetwork);
    free(datos_net);
    //free(t_minaNetwork);
    exit(EXIT_SUCCESS);

}
