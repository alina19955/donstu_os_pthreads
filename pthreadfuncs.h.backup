#ifndef PTHREADFUNCS_H
#define PTHREADFUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>

#define COUNT_THREADS 4
#define COUNT_ITERATIONS 10000

struct ThreadArgs {
    int id;
    char tag[10];
    char message[64];
};

extern int g_fd;//объявление 
extern pthread_mutex_t g_lock;//объявление
extern int counter;//объявление

pid_t getThreadID(void);

// Возвращает 0 при успехе и -1 при ошибке
int write_line(const char *msg);

void *func_thread(void *arg);
void *producer(void *arg);
void *consumer(void *arg);
void about(void);

#endif
