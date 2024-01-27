#include "global.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

extern sem_t tofuQueue;
extern sem_t meatQueue;
extern sem_t breadQueue;
extern sem_t mutex_sandwich;
extern pthread_barrier_t barrier;
void meat_thread() {
    sem_wait(&mutex_sandwich);
    meat++;
    if(bread < 2) {
        sem_post(&mutex_sandwich);
    } else {
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread -= 2;
        sem_post(&meatQueue);
        meat--;
    }
    sem_wait(&meatQueue);
    assemble_sandwich('M'); 
    pthread_barrier_wait(&barrier);
    sem_post(&mutex_sandwich);
}