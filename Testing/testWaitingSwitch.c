#include "../StudentAttempt/shared_kitchen.h"
/*
carnivore checks in, goes to create a sandwich and stops
veg checks in, waits in queue
carnivore checks out, 
if the studnet didn't check to see if any waiting vegetarians, then there could be a problem
else {
            status = NEUTRAL;//wrong implementation
            for(int i = 0; i < carnivores; i++) {
                sem_post(&CarnivoreQueue);
            }
        }
this code leads to a race condition because
lets say another car thread spins up, checks in, it sees the neutral status 
and goes to create a sandwich, but that waiting vegetarian is now up so boom, bad stuff
how to test that tho besides just fuzzing a lot
*/
sem_t wait;
sem_t signaler;

void test_sem_init() {
    sem_init(&wait, 0, 0);
    sem_init(&signaler, 0, 0);
}

void create_sandwich(bool veggie) {
    sem_post(&signaler);
    sem_wait(&wait);
}

void assemble_sandwich(char c) {

}

int main() {
    pthread_t veg;
    pthread_t car;
    init_kitchen();
    test_sem_init();
    pthread_create(&veg, 0, (void*)vegetarian_thread, 0);
    sem_wait(&signaler);
    pthread_create(&car, 0, (void*)carnivore_thread, 0);
    while(1 != carnivores) {;}
    usleep(10);
    sem_post(&wait);
    //vegetarian should exit
    while(0 != vegetarians) {;}
    //carnivore should have awoke
    sem_wait(&signaler);
    void* value;
    pthread_join(veg, &value);
    sem_post(&wait);
    pthread_join(car, &value);
    return 0;
}