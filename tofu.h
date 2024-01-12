#ifndef TOFU_H
#define TOFU_H
#include "global.h"

void tofu_thread() {
    sem_wait(&mutex_sandwich);
    //once we get in here, we have full control of the counters. Since this is
    //an oxygen thread, we wan't to increment the oxygen counter
    tofu++;
    if(bread < 2) {
        //we cannot create a new molecule, so we must relinquish
        //printf("cant bond yet: tofu\n"); 
        sem_post(&mutex_sandwich);
    } else {
        //printf("signaling %d tofu %d bread: oxy\n", tofu, bread);
        //we can possibly create a molecule, lets signal those hydrogens
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread -=2;
        sem_post(&tofuQueue);
        tofu--;
    }
    //printf("waiting to bond: tofu\n"); 
    sem_wait(&tofuQueue);
    assemble_sandwich('T');
    //printf("bonding: tofu\n"); 
    wait_barrier(&barrier);
    sem_post(&mutex_sandwich);
    //printf("outta ehre: tofu\n"); 
}
#endif