#ifndef FORKPROCESS_HTTP_H
#define FORKPROCESS_HTTP_H

#include "http.c"

int send_http_post(char *, char *, char *);
void publishData(long int value, char id[16]);

#endif //FORKPROCESS_HTTP_H
