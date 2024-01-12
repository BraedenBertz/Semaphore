#include "global.h"
#include "bread.h"
#include "tofu.h"
#include "meat.h"

void run_carnivores(int NUM_BREAD_THREADS, int NUM_MEAT_THREADS) {
    sem_init(&meatQueue, 0, 0);
    sem_init(&breadQueue, 0, 0);
    sem_init(&mutex_sandwich, 0, 1);
    init_barrier(&barrier, 3);

    pthread_t meatThreads[NUM_MEAT_THREADS];
    pthread_t breadThreads[NUM_BREAD_THREADS];
    for(int i = 0; i < NUM_BREAD_THREADS; i++) {
        pthread_create(&breadThreads[i], NULL, (void*)bread_thread, NULL);
    }
    for(int i = 0; i < NUM_MEAT_THREADS; i++) {
        pthread_create(&meatThreads[i], NULL, (void*)meat_thread, NULL);
    }

    //wait for it to end;
    void * value;
    for(int i = 0; i < NUM_BREAD_THREADS; i++) {
        pthread_join(breadThreads[i], &value);
    }
    for(int i = 0; i < NUM_MEAT_THREADS; i++) {
        pthread_join(meatThreads[i], &value);
    }
}

void run_vegetarians(int NUM_BREAD_THREADS, int NUM_TOFU_THREADS) {
    sem_init(&meatQueue, 0, 0);
    sem_init(&breadQueue, 0, 0);
    sem_init(&mutex_sandwich, 0, 1);
    init_barrier(&barrier, 3);//this is for h2o

    pthread_t meatThreads[NUM_TOFU_THREADS];
    pthread_t breadThreads[NUM_BREAD_THREADS];
    for(int i = 0; i < NUM_BREAD_THREADS; i++) {
        pthread_create(&breadThreads[i], NULL, (void*)bread_thread, NULL);
    }
    for(int i = 0; i < NUM_TOFU_THREADS; i++) {
        pthread_create(&meatThreads[i], NULL, (void*)tofu_thread, NULL);
    }

    //wait for it to end;
    void * value;
    for(int i = 0; i < NUM_BREAD_THREADS; i++) {
        pthread_join(breadThreads[i], &value);
    }
    for(int i = 0; i < NUM_TOFU_THREADS; i++) {
        pthread_join(meatThreads[i], &value);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 3) return -1;
    int num_tofu = atoi(argv[1]);
    int num_bread = atoi(argv[2]);
    if(num_tofu*2 != num_bread) return -2;
    run_vegetarians(num_bread, num_tofu);
    run_carnivores(num_bread, num_tofu);
    return 0;
}