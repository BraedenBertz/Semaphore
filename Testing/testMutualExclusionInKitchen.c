#include "../StudentAttempt/shared_kitchen.h"
#include <stdlib.h>
int CAR = 0;
int VEG = 0;
bool ok = true;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;

void create_sandwich(bool veggie) {
    pthread_mutex_lock(&MUTEX);
    if(veggie && CAR != 0) {
        printf("A, C: %d; V: %d\n", carnivores, vegetarians);
         printf("Status: %d, CAR: %d\n", status, CAR);
        ok = false;
    }
    else if(!veggie && VEG != 0){
        printf("B, C: %d; V: %d\n", carnivores, vegetarians);
         printf("Status: %d, VEG: %d\n", status, VEG);
        ok = false;
    }
    if(veggie) VEG++;
    else CAR++;
    
    pthread_mutex_unlock(&MUTEX);
    pthread_t BREAD_THREAD;
    pthread_t BREAD_THREAD2;
    pthread_t TOFU_THREAD;
    pthread_create(&BREAD_THREAD, NULL, (void*)bread_thread, NULL);
    pthread_create(&BREAD_THREAD2, NULL, (void*)bread_thread, NULL);
    pthread_create(&TOFU_THREAD, NULL, (void*)(veggie ? tofu_thread : meat_thread), NULL);
    void* value;
    pthread_join(BREAD_THREAD, &value);
    pthread_join(BREAD_THREAD2, &value);
    pthread_join(TOFU_THREAD, &value);
    pthread_mutex_lock(&MUTEX);
    if(veggie) VEG--;
    else CAR--;
    if(veggie && CAR != 0) {
        printf("C, C: %d; V: %d\n", carnivores, vegetarians);
        printf("Status: %d, CAR: %d\n", status, CAR);
        ok = false;
    }
    else if(!veggie && VEG != 0) {
        printf("D, C: %d; V: %d\n", carnivores, vegetarians);
         printf("Status: %d, VEG: %d\n", status, VEG);
        ok = false;
    }
    pthread_mutex_unlock(&MUTEX);
}

void assemble_sandwich(char c) {
}

void fuzz() {
    int num_car_threads = random() % 19;
    int num_veg_threads = rand() % 19;
    num_car_threads++;
    num_veg_threads++;
    pthread_t vegThreads_t[num_veg_threads];
    pthread_t carThreads_t[num_car_threads];
    int max = num_car_threads > num_veg_threads ? num_car_threads : num_veg_threads;
    for(int i = 0; i < max; i++) {
        if(i < num_car_threads) {
            pthread_create(&carThreads_t[i], 0, (void*)carnivore_thread, 0);
        }
        if(i < num_veg_threads) {
            pthread_create(&vegThreads_t[i], 0, (void*)vegetarian_thread, 0);            
        }
    }
    void* value;
    for(int i = 0; i < max; i++) {
        if(i < num_car_threads) {
            pthread_join(carThreads_t[i], &value);
        }
        if(i < num_veg_threads) {
            pthread_join(vegThreads_t[i], &value);          
        }
    }
}

int main() {
    init_kitchen();
    int NUM_TRIALS = 10000;
    for(int i = 0; i < NUM_TRIALS && ok; i++) {
        if(i % 500 == 0) printf("%d\n", i);
        fuzz();
    }
    if(ok) return 0;
    else return -1;
}