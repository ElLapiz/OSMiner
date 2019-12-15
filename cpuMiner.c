#include <pthread.h>
#include <sys/prctl.h>
#include "cpuMiner.h"
#include "queue.h"

#include "http.h"
#include "tlpi_hdr.h"

#define BUFFER_SIZE 1000

typedef struct data data;

data datos[50];
char default_tag[1000] = "aqui viene el tag del Json";  //TODO: hacer el timestamp y lo que haga falta para el JSON


int get_average_idle_percentage(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    int average_idle = ( idle * 100 ) / ( user + nice + system + idle + iowait + irq + softirq );
    printf("CPU disponible: %d% \n", average_idle);
    return average_idle;
}

/*
struct data creaStructData(int user, int nice, int system, int idle, int iowait, int irq, int softirq) {
    struct data data;
    data.metric = get_average_idle_percentage(user, nice, system, idle, iowait, irq, softirq);
    data.tag = &default_tag;
    return data;
}

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
    //TODO: mandar data a JSON, crear threads, publicar POST
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


void* mandaJSON(void *arg){
    data data = syncExtraeData(); //datos a convertir
    printf("Data: %d", data); //para efectos de prueba
    //TODO: convertir a JSON y mandar a servidor
}

*/

//------------------------------------------------------
//          hilos
//------------------------------------------------------
static volatile int glob = 0;
static pthread_mutex_t datos_lock = PTHREAD_MUTEX_INITIALIZER;


static void* minarDatos(void *arg) {

    int  s;
    s = pthread_mutex_lock(&datos_lock); //Mutex

    char buffer[BUFFER_SIZE];
    char header[3];
    long int user, nice,system, idle, iowait,irq, softirq, other1, other2, other3;
    FILE * dato = fopen("/proc/stat", "r");
    fgets(buffer, BUFFER_SIZE, dato);
    printf("%s \n", buffer);
    sscanf(buffer,"%s %d %d %d %d %d %d %d %d %d %d", header, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &other1 , &other2, &other3);
    //syncAgregaData(user, nice, system, idle, iowait, irq, softirq);
    get_average_idle_percentage(user, nice, system, idle, iowait, irq, softirq);
    fclose(dato);

    s = pthread_mutex_unlock(&datos_lock);
    printf("desbloqueado \n");
    if (s != 0)
        printf("Error desbloqueand mutex CPU");

}

void* mandarJSON(void *arg){
    printf("En segundo hilo");
    /*
    data data = syncExtraeData(); //datos a convertir
    printf("Data: %d", data); //para efectos de prueba
    //TODO: convertir a JSON y mandar a servidor
     */
}

//----------------------------------------------------------
//Funcion principal
//----------------------------------------------------------
void collectCpuData(int numero) {
    pthread_t minaDatos, mandaJson;
    int loops = 10, s;

    //bool padre_vive = prctl(PR_GET_PDEATHSIG); //Recibe señal de muerte de padre

    for(int i =0; i<4;i++){         //activa hilos

        s = pthread_create(&minaDatos, NULL, minarDatos, &loops);
        if (s != 0)
            printf("Error creando hilo de mineria CPU");

        s = pthread_create(&mandaJson, NULL, mandarJSON, &loops);
        if (s != 0)
            printf("Error creando hilo de envio CPU");

    }
    pthread_mutex_destroy(&datos_lock); //destruye el mutex


    exit(EXIT_SUCCESS);


    /*

    while(padre_vive){         //activa hilos
        pthread_create(&minaDatos, NULL , minaDatos , NULL);
        pthread_create(&mandaJson, NULL , mandaJSON , NULL);
    }
    pthread_mutex_destroy(&datos_lock); //destruye el mutex
*/
}