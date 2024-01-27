#include "../StudentAttempt/shared_kitchen.h"

sem_t vegetarian_wait;
sem_t signaler;
int car = 0;
int veg = 0;
int vegThreads = 3;
int carThreads = 4;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

void test_sem_init() {
    sem_init(&vegetarian_wait, 0, 0);
    sem_init(&signaler, 0, 0);
}

void create_sandwich(bool veggie) {
    pthread_mutex_lock(&MUTEX);
    if(veggie) veg++;
    else car++;
    if(veg == vegThreads) {
        sem_post(&signaler);
    }
    pthread_mutex_unlock(&MUTEX);
    sem_wait(&vegetarian_wait);
}

void assemble_sandwich(char c) {

}

int main() {
    //This is testing to see that once the carnivores achieve majority over an existing vegetarian,
    //that any additional vegetarians don't check in.
    init_kitchen();
    test_sem_init();
    pthread_t carThreadArray[carThreads];
    pthread_t vegThreadArray[vegThreads];
    for(int i = 0; i < vegThreads; i++) {
        pthread_create(&vegThreadArray[i], NULL, (void*)vegetarian_thread, NULL);
    }
    //once all the threads are through and making their sandwiches, we will send the car threads
    sem_wait(&signaler);
    if(car != 0) {
        printf("D");
        return -1;}//No car threads have gone yet, soooooo
    //now we are to be putting the car threads in
    for(int i = 0; i < carThreads; i++) {
        pthread_create(&carThreadArray[i], NULL, (void*)carnivore_thread, NULL);
    }
    //None of the car threads should have gone to create_sandwich, so car == 0 should be true
    if(car != 0) {
        printf("C");
        return -1;
    }//However, they should have checked in so vegetarian == vegThreads should be true eventually
    while(vegetarians != vegThreads || carnivores != carThreads || status != TRANSITION_TO_CARNIVORE) {
        ;
    }
    //if another veg tries to enter, it should be blocked
    pthread_t extra_veg;
    pthread_create(&extra_veg, NULL, (void*) vegetarian_thread, NULL);
    //how to ensure that this thread is blocked...?
    //well, if it didn't block, it should have incremented the carnivores
    sleep(1);
    if(vegetarians > vegThreads) {
        printf("A");
        return -1;
        }
    //seems that the blocking, worked... probably
    for(int i = 0; i < carThreads; i++) {
        pthread_cancel(carThreadArray[i]);
    }
    for(int i = 0; i < vegThreads; i++) {
        pthread_cancel(vegThreadArray[i]);
    }
    pthread_cancel(extra_veg);
    
    return 0;
}