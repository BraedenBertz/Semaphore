#include "global.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t tofuQueue;
sem_t meatQueue;
sem_t breadQueue;
sem_t mutex_sandwich;
pthread_barrier_t barrier;

void tofu_thread() {
    sem_wait(&mutex_sandwich);
    tofu++;
    if(bread < 2) {
        sem_post(&mutex_sandwich);
    } else {
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread -= 2;
        sem_post(&tofuQueue);
        tofu--;
    }
    
    sem_wait(&tofuQueue);
    assemble_sandwich('T');
    pthread_barrier_wait(&barrier);
    sem_post(&mutex_sandwich);
}