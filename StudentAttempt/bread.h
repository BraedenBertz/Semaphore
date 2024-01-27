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

void bread_thread() {
    sem_wait(&mutex_sandwich);
    bread++;
    if(tofu >= 1 && bread >= 2) {
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread-=2;
        sem_post(&tofuQueue);
        tofu--;
    }
    else if(meat >= 1 && bread >= 2) {
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread-=2;
        sem_post(&meatQueue);
        meat--;
    } else {
        sem_post(&mutex_sandwich);
    }        
    sem_wait(&breadQueue); 
    assemble_sandwich('B');
    pthread_barrier_wait(&barrier);
}