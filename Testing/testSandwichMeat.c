#include "../StudentAttempt/shared_kitchen.h"
int TOFU = 0;
int BREAD = 0;
int MEAT = 0;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;


void create_sandwich(bool veggie) {
    
}

void assemble_sandwich(char c) {
    pthread_mutex_lock(&MUTEX);
    switch(c) {
        case 'T':
            TOFU++;
            break;
        case 'B': 
            BREAD++;
            break;
        case 'M':
            MEAT++;
            break;
    }
    pthread_mutex_unlock(&MUTEX);
}

void reset() {
    TOFU = 0;
    BREAD = 0;
    MEAT = 0;
    bread = 0;
    tofu = 0;
    meat = 0;
}

bool sandwich_test_single() {
    reset();
    pthread_t BREAD_THREAD1;
    pthread_t BREAD_THREAD2;
    pthread_t MEAT_THREAD;
    pthread_create(&BREAD_THREAD1, NULL, (void*)bread_thread, NULL);
    pthread_create(&BREAD_THREAD2, NULL, (void*)bread_thread, NULL);
    pthread_create(&MEAT_THREAD, NULL, (void*)meat_thread, NULL);
    void* value;
    pthread_join(BREAD_THREAD1, &value);
    pthread_join(BREAD_THREAD2, &value);
    pthread_join(MEAT_THREAD, &value);
    if(TOFU != 0 || BREAD != 2 || MEAT != 1) return false;
    if(meat != 0 || bread != 0 || tofu != 0) return false;
    return true;
}

bool sandwich_test_multiple() {
    reset();
    int meatNum = 10;
    int breadNum = 2*meatNum;
    pthread_t meatThreads[meatNum];
    pthread_t breadThreads[breadNum];
    for(int i = 0; i < meatNum; i++) pthread_create(&meatThreads[i], 0, (void*)tofu_thread, 0);
    for(int i = 0; i < breadNum; i++) pthread_create(&breadThreads[i], 0, (void*)bread_thread, 0);
    void* value;
    for(int i = 0; i < meatNum; i++) pthread_join(meatThreads[i], &value);
    for(int i = 0; i < breadNum; i++) pthread_join(breadThreads[i], &value);
    if(TOFU != meatNum || BREAD != breadNum || MEAT != 0) return false;
    if(meat != 0 || bread != 0 || tofu != 0) return false;
    return true;
}

int main() {
    init_kitchen();
    if(!sandwich_test_single() || !sandwich_test_multiple()) return -1;
    return 0;
}