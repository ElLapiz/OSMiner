//
// Created by alepiz on 14/12/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

int front = 0;
int rear = -1;
int itemCount = 0;

struct data{
    char tag[1000];
    int metric;
};


struct data peek(struct data datos[]) {
    return datos[front];
}

bool isEmpty(struct data datos[]) {
    return itemCount == 0;
}

bool isFull(struct data datos[]) {
    return itemCount == MAX;
}

int size(struct data datos[]) {
    return itemCount;
}

void insert(struct data data, struct data datos[]) {

    if(!isFull(datos)) {

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