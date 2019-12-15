#ifndef FORKPROCESS_QUEUE_H
#define FORKPROCESS_QUEUE_H

#include "queue.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct data peek(struct data datos[]);
bool isEmpty(struct data datos[]);
bool isFull(struct data datos[]);
int size(struct data datos[]);
void insert(struct data data, struct data datos[]);
struct data removeData(struct data datos[]);


#endif //FORKPROCESS_QUEUE_H
