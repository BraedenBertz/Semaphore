#include "shared_kitchen.h"

void init_testing() {
    //testing syncs
    sem_init(&CCheckin, 0, 0);
    sem_init(&VCheckin, 0, 0);
    sem_init(&Sando, 0, 0);
    sem_init(&CCheckout, 0, 0);
    sem_init(&VCheckout, 0, 0);
}

void create_sandwich(bool veggie) {
    sem_wait(&TestLock);
    if(veggie) {
        vegInKitchen++;
        if(carInKitchen != 0)
            printf("ingredient mixing %d; %d\n", carInKitchen, vegInKitchen);
    }
    else {
        carInKitchen++;
        if(vegInKitchen!=0) {
            printf("ingredient mixing 2 %d; %d\n", carInKitchen, vegInKitchen);
        }
    }
    sem_post(&TestLock);
    sem_wait(&Sando);
    sem_wait(&TestLock);
    if(veggie) {
        vegInKitchen--;
        if(carInKitchen != 0)
            printf("Ingredient mixing %d; %d\n", carInKitchen, vegInKitchen);
    }
    else {
        carInKitchen--;
        if(vegInKitchen != 0) {
            printf("Ingredient mixing 2 %d; %d\n", carInKitchen, vegInKitchen);
        }
    }
    sem_post(&TestLock);
}

void init_kitchen() {
    sem_init(&TestLock, 0, 1);
    sem_init(&mutex, 0, 1);//This is a lock
    sem_init(&Carnivore_turn, 0, 1);//This is a turnstile
    sem_init(&Vegetarian_turn, 0, 1);//This is a turnstile
    sem_init(&CarnivoreQueue, 0, 0);//This is a queue
    sem_init(&VegetarianQueue, 0, 0);//This is a queue
}

int testcase(char* ordering, int* vars, int size) {
    for(int i = 0; i < size; i++) {
        switch(ordering[i]) {
            case 'V':
                switch(ordering[i+1]) {
                    case 'I':
                    sem_post(&VCheckin);
                    break;
                    case 'O':
                    sem_post(&VCheckout);
                    break;
                }
                i++;
                break;
            case 'S':
                sem_post(&Sando);
                break;
            case 'C':
            switch(ordering[i+1]) {
                    case 'I':
                        sem_post(&CCheckin);
                    break;
                    case 'O':
                        sem_post(&CCheckout);
                    break;
                }
                i++;
                break;
        }
        printStatus();
        usleep(10000);
    }
}

int main(int argc, char* argv[]) {
    init_factory();
    init_testing();
    char* seq = "CIVIVISCOSSVOVO";
    int * sdfjlksdf;
    int size = 15;
    pthread_t threads[3];
    pthread_create(&threads[0],NULL, (void*) Carnivore, NULL);
    pthread_create(&threads[1],NULL, (void*) Vegetarian, NULL);
    pthread_create(&threads[2],NULL, (void*) Vegetarian, NULL);
    testcase(seq, sdfjlksdf, size);
}