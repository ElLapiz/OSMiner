#include "queue.h"
#include <stdbool.h>

#define MAX 100

int front = 0;
int rear = -1;
int itemCount = 0;

struct data{
    char *tag;
    int metric;
};

bool isFull() {
    return itemCount == MAX;
}

void insert(struct data data, struct data datos[]) {
    if(!isFull()) {

        if(rear == MAX-1) {
            rear = -1;
        }
        datos[++rear] = data;
        itemCount++;
    }
}

struct data removeData(struct data datos[]){
    struct data data = datos[front++];
    if(front == MAX) {
        front = 0;
    }

    itemCount--;

    return data;
}
