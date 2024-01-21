#include "shared_kitchen.h"

#define NUM_CARNIVORES 25
#define NUM_VEGETARIANS 17
pthread_t carnivoreThreads[NUM_CARNIVORES];
pthread_t carnivoreThreads2[NUM_CARNIVORES];
pthread_t vegetarianThreads[NUM_VEGETARIANS];
pthread_t vegetarianThreads2[NUM_VEGETARIANS];


void* captureThread(void* arg) {
    while (true) {
        // Capture the state of mutex and mutex_sandwich
        //sem_wait(&mutex);
        //printf("have mutex\n");
        //sem_wait(&mutex_sandwich);
        sleep(5);
        printStatus("", "");
        printf("bread %d tofu %d meat %d\n", bread, tofu, meat);
        //sem_post(&mutex_sandwich);
        //sem_post(&mutex);  
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    init_factory();
    pthread_t watcher;
    pthread_create(&watcher, NULL, (void*)captureThread, NULL);
    
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_create(&carnivoreThreads[i], NULL, (void*)Carnivore, NULL);
    }
    
    for(int i = 0; i < NUM_VEGETARIANS; i++) {
        pthread_create(&vegetarianThreads[i], NULL, (void*)Vegetarian, NULL);
    }

    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_create(&carnivoreThreads2[i], NULL, (void*)Carnivore, NULL);
    }

    for(int i = 0; i < NUM_VEGETARIANS; i++) {
        pthread_create(&vegetarianThreads2[i], NULL, (void*)Vegetarian, NULL);
    }
    //wait for it to end;
    void * value;
    for(int i = 0; i < NUM_CARNIVORES; i++) {
        pthread_join(carnivoreThreads[i], &value);
        pthread_join(carnivoreThreads2[i], &value);
    }
    for(int i = 0; i < NUM_VEGETARIANS; i++) {
        pthread_join(vegetarianThreads[i], &value);
        pthread_join(vegetarianThreads2[i], &value);
    }

    pthread_cancel(watcher);
    return 0;
}