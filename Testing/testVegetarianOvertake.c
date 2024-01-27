#include "../StudentAttempt/shared_kitchen.h"

sem_t vegetarian_wait;
sem_t carnivore_wait;
sem_t signaler;
int car = 0;
int veg = 0;
int vegThreads = 4;
int carThreads = 3;
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

//Much like blocked, but testing that once the carnivores are finished, then the vegetarians go
int main() {
    init_kitchen();
    pthread_t carThreadArray[carThreads];
    pthread_t vegThreadArray[vegThreads];
    for(int i = 0; i < carThreads; i++) {
        pthread_create(&carThreadArray[i], NULL, (void*)carnivore_thread, NULL);
    }
    //once all the threads are through and making their sandwiches, we will send the veg threads
    sem_wait(&signaler);
    if(veg != 0) return -1;//No veg threads have gone yet, soooooo
    //now we are to be putting the veg threads in
    for(int i = 0; i < vegThreads; i++) {
        pthread_create(&vegThreadArray[i], NULL, (void*)vegetarian_thread, NULL);
    }
    //None of the veg threads should have gone to create_sandwich, so veg == 0 should be true
    if(veg != 0) return -1;
    //However, they should have checked in so vegetarian == vegThreads should be true eventually
    while(vegetarians != vegThreads) {
        ;
    }
    //now that they are equal, we should see the state change
    if(status != TRANSITION_TO_VEGETARIAN) return -1;
    //Now we can go ahead and let all the carnivores go through
    for(int i = 0; i < carThreads; i++) sem_post(&carnivore_wait);
    //all the carnivore threads should pass, so now the vegetarian threads should start makign their sandwiches
    sem_wait(&signaler);
    //there should be no carnivores left
    if(carnivores != 0) return -1;
    //The status should have changed to vegetarian
    if(status != VEGETARIAN) return -1;
    return 0;
}