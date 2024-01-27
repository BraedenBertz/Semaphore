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

bool sandwich_test_incomplete_sando_bread() {
    reset();
    pthread_t BREAD_THREAD;
    pthread_t TOFU_THREAD;
    pthread_create(&BREAD_THREAD, NULL, (void*)bread_thread, NULL);
    pthread_create(&TOFU_THREAD, NULL, (void*)tofu_thread, NULL);
    sleep(1);
    if(TOFU != 0 || BREAD != 0 || MEAT != 0) return false;
    if(tofu != 1 || bread != 1 || meat != 0) return false;
    pthread_cancel(BREAD_THREAD);
    pthread_cancel(TOFU_THREAD);
    reset();
    pthread_t MEAT_THREAD;
    pthread_create(&BREAD_THREAD, NULL, (void*)bread_thread, NULL);
    pthread_create(&MEAT_THREAD, NULL, (void*)meat_thread, NULL);
    sleep(1);
    if(TOFU != 0 || BREAD != 0 || MEAT != 0) return false;
    if(tofu != 0 || bread != 1 || meat != 1) return false;
    pthread_cancel(BREAD_THREAD);
    pthread_cancel(MEAT_THREAD);
    return true;
}

bool sandwich_test_incomplete_sando() {
    reset();
    pthread_t BREAD_THREAD;
    pthread_t BREAD_THREAD2;
    pthread_create(&BREAD_THREAD, NULL, (void*)bread_thread, NULL);
    pthread_create(&BREAD_THREAD2, NULL, (void*)bread_thread, NULL);
    sleep(1);
    if(TOFU != 0 || BREAD != 0 || MEAT != 0) return false;
    if(tofu != 0 || bread != 2 || meat != 0) return false;
    pthread_cancel(BREAD_THREAD);
    pthread_cancel(BREAD_THREAD2);
    return true;
}

int main() {
    init_kitchen();
    if(!sandwich_test_incomplete_sando() || !sandwich_test_incomplete_sando_bread()) return -1;
    return 0;
}