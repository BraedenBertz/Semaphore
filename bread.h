#ifndef BREAD_H
#define BREAD_H
#include "global.h"
#include <assert.h>

void bread_thread() {
    sem_wait(&mutex_sandwich);
    //once we get in here, we have full control of the counters. Since this is
    //a hydrogen thread, we want to increment the hydrogen thread
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
    // wait_barrier(&barrier);
    pthread_barrier_wait(&sldfjs);
}
#endif