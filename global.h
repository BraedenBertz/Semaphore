#ifndef GLOBAL_H
#define GLOBAL_H

#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include "barrier.h"
//tofu and bread are counters, protected by mutex. barrier is where
//each set of three threads meets after invoking assemble_sandwhich and before allowing the next
//set of threads to proceed.
//tofuQueue is the semaphore tofu threads wait on; breadQueue is the
//semaphore bread threads wait on. I am using the naming convention for queues, 
//so tofuQueue.wait() means “join the tofu queue” and
//tofuQueue.signal() means “release an tofu thread from the queue.”
int tofu = 0;
int meat = 0;
int bread = 0;
sem_t tofuQueue;
sem_t meatQueue;
sem_t breadQueue;
sem_t mutex_sandwich;
struct resuseable_barrier barrier;//Barrier for blocking sandwich ingredients

void assemble_sandwich(char c) {
    printf("%c\n", c);
}

int carnivores = 0;//The number of checked in carnivores
int vegetarians = 0;//The number of checked in vegetarians
enum STATUS {
    NEUTRAL,
    CARNIVORE,
    VEGETARIAN,
    TRANSITION_TO_CARNIVORE,
    TRANSITION_TO_VEGETARIAN,
};
enum STATUS status = NEUTRAL;
sem_t mutex;//mutex for the scoreboard
sem_t Carnivore_turn;
sem_t Vegetarian_turn;
sem_t CarnivoreQueue;
sem_t VegetarianQueue;

char* CarnivoreStart = "\033[31;1m";
char* End = "\033[00m";
char* exitStart = "\033[30;1m";
char* VegetarianStart = "\033[32;1m";
void printStatus(char* msg1, char* msg2) {
    int value, value2, value3, value4;
    sem_getvalue(&Carnivore_turn, &value);
    sem_getvalue(&Vegetarian_turn, &value2);
    sem_getvalue(&VegetarianQueue, &value3);
    sem_getvalue(&CarnivoreQueue, &value4);
    pid_t tid = syscall(SYS_gettid);
    int value5;
    sem_getvalue(&mutex, &value5);//always should be zero
    char* stat;
    switch (status) {
        case 0: stat = "NEUTRAL";break;
        case 1: stat = "Carnivore";break;
        case 2: stat = "Vegetarian";break;
        case 3: stat = "TRANSITION_TO_CARNIVORE";break;
        case 4: stat = "TRANSITION_TO_VEGETARIAN";break;
    }
    printf("\n%s%s%s\n\
    vegetarians: %d\n\
    carnivores: %d\n\
    status: %s\n\
    Carnivore_turn: %d\n\
    Vegetarian_turn: %d\n\
    VegetarianQueue: %d\n\
    CarnivoreQueue: %d\n\
    mutex: %d\n\
    tid: %d\n",
           msg1, msg2, End,
           vegetarians, carnivores, stat, value, value2, value3, value4, value5, tid);
           fflush(stdout);
}
#endif