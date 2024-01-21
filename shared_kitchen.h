#include "global.h"
#include "bread.h"
#include "meat.h"
#include "tofu.h"

void init_factory() {
    sem_init(&TestLock, 0, 1);
    sem_init(&mutex, 0, 1);//This is a lock
    sem_init(&Carnivore_turn, 0, 1);//This is a turnstile
    sem_init(&Vegetarian_turn, 0, 1);//This is a turnstile
    sem_init(&CarnivoreQueue, 0, 0);//This is a queue
    sem_init(&VegetarianQueue, 0, 0);//This is a queue

    //these are concurrency controls for the sandwich
    sem_init(&meatQueue, 0, 0);//queue for meat
    sem_init(&tofuQueue, 0, 0);//queue for tofu
    sem_init(&breadQueue, 0, 0);//queue for bread
    sem_init(&mutex_sandwich, 0, 1);//mutex for sandwhich
    pthread_barrier_init(&sldfjs, NULL, 3);
}

void Carnivore() {
    //Wait to see if we can enter the store and checkin
    sem_wait(&Carnivore_turn);
    sem_post(&Carnivore_turn);
    sem_wait(&CCheckin);
    sem_wait(&mutex);
    //A worker can check us in, update the scoreboard
    carnivores++;

    //since we are checking in as a carnivore, we have to update the state of the kitchen
    switch (status) {
    case NEUTRAL: //No one is occupying the kitchen, so we carnivores take over
        status = CARNIVORE;//adjust the state of the kitchen
        sem_post(&mutex);//let other threads be able to update the scoreboard
        //crucially, since carnivores are in control, we can start making the sandwhich
        break;
    case VEGETARIAN: //The vegetarians are currently operating the kitchen
        if(carnivores > vegetarians) {
            //There are now more carnivore customers checked in than vegetarian customers
            //By majority rule, we are going to transition to carnivore kitchen
            status = TRANSITION_TO_CARNIVORE;
            //lock the Vegetarian_turnstile so that Vegetarians cannot checkin
            //ensures that no starvation occurs
            sem_wait(&Vegetarian_turn);
        }
        //give up the mutex so vegetarian threads can checkout
        sem_post(&mutex);
        //wait until the last Vegetarian exits and signals us to go
        sem_wait(&CarnivoreQueue);
        break;
    case TRANSITION_TO_CARNIVORE:
        //We are already transitioning to carnivore, so just join the queue
        //give up the mutex so vegetarian threads can checkout
        sem_post(&mutex);
        sem_wait(&CarnivoreQueue);
        break;
    default: 
        //implicitly we are transitioning to vegetarian, but this thread already entered the store and thus can checkin
        //kinda weird, but oh well
        sem_post(&mutex);
    }
    //start making the sandwhich order
    create_sandwich(false);
    sem_wait(&CCheckout);
    //sandwhiches aren't required to hold the mutex because the mutex is for the scoreboard
    sem_wait(&mutex);
    //checkout this thread
    carnivores--;
    
    //if the last carnivore is leaving, and we want to transition to vegetarian, then we
    //have to signal all those waiting vegetarian customers
    //and open the turnstile for vegetarians to enter the store again
    if(carnivores == 0) {
        if(status == TRANSITION_TO_VEGETARIAN) {
            int value;
            sem_getvalue(&Vegetarian_turn, &value);
            if(value == 0) {
                //open the Vegetarian turnstile
                sem_post(&Vegetarian_turn);
            }
            for(int i = 0; i < vegetarians; i++) {
                //let the waiting Vegetarians through
                sem_post(&VegetarianQueue);
            }
            //Change the state of the kitchen
            status = VEGETARIAN;
            sem_getvalue(&Carnivore_turn, &value);
            if(value == 0) {
                sem_post(&Carnivore_turn);
            }

        } else {
            //there are no waiting vegetarians, so we can just make the kitchen ready
            //for either sandwich shop
            status = NEUTRAL;
            for(int i = 0; i < vegetarians; i++) {
                //let the waiting Vegetarians through
                sem_post(&VegetarianQueue);
            }
            //sem_post(&Vegetarian_turn);
        }
    }

    if(status == CARNIVORE) {
        //check if this carnivore that is leaving will tip the scale
        if(vegetarians > carnivores) {
            //there are now more carnivores than vegetarians, so we 
            //will transition to vegetarians
            status = TRANSITION_TO_VEGETARIAN;
            //block the Carnivore turnstile, not allowing carnviores to enter the store
            sem_wait(&Carnivore_turn);
        }
    }

    //release the scoreboard mutex so other threads can proceed
    sem_post(&mutex);
}

void Vegetarian() {
    
    sem_wait(&Vegetarian_turn);//see if we are blocked from checking in
    sem_post(&Vegetarian_turn);//open the turnstile again
    sem_wait(&VCheckin);
    sem_wait(&mutex);//acquire the lock
    vegetarians++;

    switch (status) {
    case NEUTRAL:
        status = VEGETARIAN;
        sem_post(&mutex);//release the mutex
        break;
    case CARNIVORE:
        if(vegetarians > carnivores) {
            status = TRANSITION_TO_VEGETARIAN;
            sem_wait(&Carnivore_turn);//lock carnivores from entering the turnstile
        }
        sem_post(&mutex);//release the mutex
        sem_wait(&VegetarianQueue);//wait for this queue
        break;
    case TRANSITION_TO_VEGETARIAN: 
        sem_post(&mutex);//release the mutex
        sem_wait(&VegetarianQueue);//wait in the queue
        break;
    default: 
        sem_post(&mutex);//release the mutex
        break;
    }

    create_sandwich(true);
    sem_wait(&VCheckout);
    sem_wait(&mutex);
    vegetarians--;

    if(vegetarians == 0) {
        if(status == TRANSITION_TO_CARNIVORE) {
            int value;
            sem_getvalue(&Carnivore_turn, &value);
            if(value == 0) {
                sem_post(&Carnivore_turn);
            }
            for(int i = 0; i < carnivores; i++) {
                sem_post(&CarnivoreQueue);
            }
            status = CARNIVORE;
            sem_getvalue(&Vegetarian_turn, &value);
            if(value == 0) {
                sem_post(&Vegetarian_turn);
            }

        } else {
            status = NEUTRAL;
            for(int i = 0; i < carnivores; i++) {
                sem_post(&CarnivoreQueue);
            }
        }
    }

    if(status == VEGETARIAN) {
        if(carnivores > vegetarians) {
            status = TRANSITION_TO_CARNIVORE;
            sem_wait(&Vegetarian_turn);//lock Vegetarian turnstile
        }
    }
    sem_post(&mutex);
}