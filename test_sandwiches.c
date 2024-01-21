#include "global.h"
#include "bread.h"
#include "tofu.h"
#include "meat.h"

void init_sandy() {
    sem_init(&meatQueue, 0, 0);
    sem_init(&breadQueue, 0, 0);
    sem_init(&mutex_sandwich, 0, 1);
    pthread_barrier_init(&sldfjs, NULL, 3);
}

int testcase(char* ordering, int* vars, int size) {
    pthread_t threads[size];
    for(int i = 0; i < size; i++) {
        switch(ordering[i]) {
            case 'T':
            pthread_create(&threads[i], NULL, (void*)tofu_thread, NULL);
            while(vars[i] != tofu) {;}
            break;
            case 'B':
            pthread_create(&threads[i], NULL, (void*)bread_thread, NULL);
            while(vars[i] != bread) {;}
            break;
            case 'M':
            pthread_create(&threads[i], NULL, (void*)meat_thread, NULL);
            while(vars[i] != meat) {;}
            break;
        }
    }
    return 0;
}

void generateTestcase(char* ordering, int* vars, int size) {
    pthread_t threads[size];
    for(int i = 0; i < size; i++) {
        switch(ordering[i]) {
            case 'T':
            pthread_create(&threads[i], NULL, (void*)tofu_thread, NULL);
            sem_wait(&mutex_sandwich);
            vars[i] = tofu;
            sem_post(&mutex_sandwich);
            break;
            case 'B':
            pthread_create(&threads[i], NULL, (void*)bread_thread, NULL);
            sem_wait(&mutex_sandwich);
            vars[i] = bread;
            sem_post(&mutex_sandwich);
            break;
            case 'M':
            pthread_create(&threads[i], NULL, (void*)meat_thread, NULL);
            sem_wait(&mutex_sandwich);
            vars[i] = meat;
            sem_post(&mutex_sandwich);
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    char* sequence_of_threads = "BBTBTB";
    int variables[] = {1, 2, 0, 1, 1, 0};
    int size = 6;
    init_sandy();
    if(testcase(sequence_of_threads, variables, size)) {
        printf("fail\n");
    }
    sequence_of_threads = "BBBBBBTTM";
    int variables2[] = {1, 2, 3, 4, 5, 6, 0, 0, 0};
    size = 9;
    if(testcase(sequence_of_threads, variables2, size)) {
        printf("fail\n");
    }
    int variables3[size];
    generateTestcase(sequence_of_threads, variables3, size);
    for(int i = 0; i < size; i++) {
        printf("%d,", variables3[i]);
    }
    return 0;
}