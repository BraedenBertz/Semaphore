#include "../StudentAttempt/shared_kitchen.h"

sem_t carnivore_wait;
sem_t signaler;
int car = 0;
int veg = 0;
int vegThreads = 4;
int carThreads = 3;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

void test_sem_init() {
    sem_init(&carnivore_wait, 0, 0);
    sem_init(&signaler, 0, 0);
}

void create_sandwich(bool veggie) {
    pthread_mutex_lock(&MUTEX);
    if(veggie) veg++;
    else car++;
    if(car == carThreads) {
        sem_post(&signaler);
    }
    pthread_mutex_unlock(&MUTEX);
    sem_wait(&carnivore_wait);
}

void assemble_sandwich(char c) {

}

int main() {
    //This is testing to see that once the vegetarians achieve majority over an existing carnivore,
    //that any additional carnivores don't check in.
    init_kitchen();
    test_sem_init();
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
    while(vegetarians != vegThreads || carnivores != carThreads || status != TRANSITION_TO_VEGETARIAN) {
        ;
    }
    //if another carnivore tries to enter, it should be blocked
    pthread_t extra_carnivore;
    pthread_create(&extra_carnivore, NULL, (void*) carnivore_thread, NULL);
    //how to ensure that this thread is blocked...?
    //well, if it didn't block, it should have incremented the carnivores
    sleep(1);
    if(carnivores > carThreads) return -1;
    //seems that the blocking, worked... probably
    for(int i = 0; i < carThreads; i++) {
        pthread_cancel(carThreadArray[i]);
    }
    for(int i = 0; i < vegThreads; i++) {
        pthread_cancel(vegThreadArray[i]);
    }
    pthread_cancel(extra_carnivore);
    return 0;
}