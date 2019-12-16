#ifndef FORKPROCESS_QUEUE_H
#define FORKPROCESS_QUEUE_H

#include "queue.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool isFull();
void insert(struct data data, struct data datos[]);
struct data removeData(struct data datos[]);

#endif //FORKPROCESS_QUEUE_H
