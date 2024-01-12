#include "shared_kitchen.h"

#define NUM_CARNIVORES 3
#define NUM_VEGETARIANS 3

int main(int argc, char* argv[]) {
    init_factory();
    pthread_t heathenThreads[NUM_CARNIVORES];
    pthread_t prudeThreads[NUM_VEGETARIANS];
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_create(&heathenThreads[i], NULL, (void*)Carnivore, NULL);
        
    }
    //sleep for a second and then let the next round go
    usleep(1*100);
    for(int i = 0; i < NUM_VEGETARIANS; i++) {
        pthread_create(&prudeThreads[i], NULL, (void*)Vegetarian, NULL);
    }

    //sleep for a second and then let the next round go
    usleep(10000);
    pthread_t HThreads2[NUM_CARNIVORES];
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_create(&HThreads2[i], NULL, (void*)Carnivore, NULL);
        
    }

    //wait for it to end;
    void * value;
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_join(heathenThreads[i], &value);
    }
    for(int i = 0; i < NUM_VEGETARIANS; i++) {
        pthread_join(prudeThreads[i], &value);
    }
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_join(HThreads2[i], &value);
        
    }

    return 0;
}