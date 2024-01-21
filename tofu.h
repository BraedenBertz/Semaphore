#ifndef TOFU_H
#define TOFU_H
#include "global.h"

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
    // wait_barrier(&barrier);
    pthread_barrier_wait(&sldfjs);
    
    sem_post(&mutex_sandwich);
}
#endif