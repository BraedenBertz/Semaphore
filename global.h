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

//testing syncs
sem_t CCheckin;
sem_t VCheckin;
sem_t Sando;
sem_t CCheckout;
sem_t VCheckout;

int tofu = 0;
int meat = 0;
int bread = 0;
sem_t tofuQueue;
sem_t meatQueue;
sem_t breadQueue;
sem_t mutex_sandwich;//Barrier for blocking sandwich ingredients
pthread_barrier_t sldfjs;

int vegInKitchen = 0;
int carInKitchen = 0;


extern void create_sandwich(bool veggie);

void assemble_sandwich(char c) {
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
sem_t TestLock;

void printStatus() {
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
    printf("\nvegetarians: %d\n\
    carnivores: %d\n\
    status: %s\n\
    Carnivore_turn: %d\n\
    Vegetarian_turn: %d\n\
    VegetarianQueue: %d\n\
    CarnivoreQueue: %d\n\
    mutex: %d\n\
    tid: %d\n",
    vegetarians, carnivores, stat, value, value2, value3, value4, value5, tid);
           fflush(stdout);
}
#endif