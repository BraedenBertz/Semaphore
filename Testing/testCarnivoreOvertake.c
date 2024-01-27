#include "../StudentAttempt/shared_kitchen.h"

sem_t vegetarian_wait;
sem_t carnivore_wait;
sem_t signaler;
int car = 0;
int veg = 0;
int vegThreads = 3;
int carThreads = 4;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

void test_sem_init() {
    sem_init(&vegetarian_wait, 0, 0);
    sem_init(&carnivore_wait, 0, 0);
    sem_init(&signaler, 0, 0);
}

void create_sandwich(bool veggie) {
    pthread_mutex_lock(&MUTEX);
    if(veggie) {
        veg++;
        if(veg == vegThreads) {
            sem_post(&signaler);
        }
        pthread_mutex_unlock(&MUTEX);
        sem_wait(&vegetarian_wait);
    }
    else {
        car++;
        if(car == carThreads) {
            sem_post(&signaler);
        }
        pthread_mutex_unlock(&MUTEX);
        sem_wait(&carnivore_wait);
    }    
}

void assemble_sandwich(char c) {

}

//Much like blocked, but testing that once the vegetarians are finished, then the carnivores go
int main() {
    init_kitchen();
    pthread_t carThreadArray[carThreads];
    pthread_t vegThreadArray[vegThreads];
    for(int i = 0; i < vegThreads; i++) {
        pthread_create(&vegThreadArray[i], NULL, (void*)vegetarian_thread, NULL);
    }
    //once all the threads are through and making their sandwiches, we will send the car threads
    sem_wait(&signaler);
    if(car != 0) return -1;//No car threads have gone yet, soooooo
    //now we are to be putting the car threads in
    for(int i = 0; i < carThreads; i++) {
        pthread_create(&carThreadArray[i], NULL, (void*)carnivore_thread, NULL);
    }
    //None of the car threads should have gone to create_sandwich, so car == 0 should be true
    if(car != 0) return -1;
    //However, they should have checked in so carnivore == carThreads should be true eventually
    while(carnivores != carThreads) {
        ;
    }
    //now that they are equal, we should see the state change
    if(status != TRANSITION_TO_CARNIVORE) return -1;
    //Now we can go ahead and let all the vegetarians go through
    for(int i = 0; i < vegThreads; i++) sem_post(&vegetarian_wait);
    //all the vegetarian threads should pass, so now the carnivore threads should start makign their sandwiches
    sem_wait(&signaler);
    //there should be no vegetarians left
    if(vegetarians != 0) return -1;
    //The status should have changed to carnivore
    if(status != CARNIVORE) return -1;
    return 0;
}