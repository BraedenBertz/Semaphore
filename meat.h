#ifndef MEAT_H
#define MEAT_H
#include "global.h"
#include <assert.h>
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
    //wait_barrier(&barrier);
    pthread_barrier_wait(&sldfjs);
    // printf("\n");
    sem_post(&mutex_sandwich); 
}
#endif