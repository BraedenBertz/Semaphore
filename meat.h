#ifndef MEAT_H
#define MEAT_H
#include "global.h"

void meat_thread() {
    sem_wait(&mutex_sandwich);
    //once we get in here, we have full control of the counters. Since this is
    //an oxygen thread, we wan't to increment the oxygen counter
    meat++;
    if(bread < 2) {
        //we cannot create a new molecule, so we must relinquish
       // printf("cant bond yet: oxy\n"); 
        sem_post(&mutex_sandwich);
    } else {
        //printf("signaling %d oxy %d hydro: oxy\n", oxygen, hydrogen);
        //we can possibly create a molecule, lets signal those hydrogens
        sem_post(&breadQueue);
        sem_post(&breadQueue);
        bread -=2;
        sem_post(&meatQueue);
        meat--;
    }
    //printf("waiting to bond: oxy\n"); 
    sem_wait(&meatQueue);
    assemble_sandwich('M');
    //printf("bonding: oxy\n"); 
    wait_barrier(&barrier);
    sem_post(&mutex_sandwich);
    //printf("outta ehre: oxy\n"); 
}
#endif