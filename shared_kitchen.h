#include "global.h"

void create_sandwich(char* team) {
    pid_t tid = syscall(SYS_gettid);
    printf("Worker %d is walking the field. Part of the %s\n", tid, team);
    fflush(stdout);
    usleep(50*1000);
    printf("Worker %d finished. Part of the %s\n", tid, team);
    fflush(stdout);
}

void init_factory() {
    sem_init(&mutex, 0, 1);//This is a lock
    sem_init(&Carnivore_turn, 0, 1);//This is a turnstile
    sem_init(&Vegetarian_turn, 0, 1);//This is a turnstile
    sem_init(&CarnivoreQueue, 0, 0);//This is a queue
    sem_init(&VegetarianQueue, 0, 0);//This is a queue
}

void Carnivore() {
    pid_t tid = syscall(SYS_gettid);
    printf("Carnivore enter %d\n", tid);
    //Wait to see if we can enter the store and checkin
    sem_wait(&Carnivore_turn);
    sem_post(&Carnivore_turn);
    sem_wait(&mutex);
    //A worker can check us in, update the scoreboard
    carnivores++;

    //since we are checking in as a carnivore, we have to update the state of the kitchen
    switch (status) {
    case NEUTRAL: //No one is occupying the kitchen, so we carnivores take over
        status = CARNIVORE;//adjust the state of the kitchen
        printStatus(CarnivoreStart, "Carnivore CHECKEDIN1");
        sem_post(&mutex);//let other threads be able to update the scoreboard
        //crucially, since carnivores are in control, we can start making the sandwhich
        break;
    case VEGETARIAN: //The vegetarians are currently operating the kitchen
        if(carnivores > vegetarians) {
            //There are now more carnivore customers checked in than vegetarian customers
            //By majority rule, we are going to transition to carnivore kitchen
            status = TRANSITION_TO_CARNIVORE;
            //lock the Vegetarian_turnstile so that Vegetarians cannot checkin
            //ensures that no starvation occurs
            sem_wait(&Vegetarian_turn);
        }
        printStatus(CarnivoreStart, "Carnivore CHECKEDIN2");
        //give up the mutex so vegetarian threads can checkout
        sem_post(&mutex);
        //wait until the last Vegetarian exits and signals us to go
        sem_wait(&CarnivoreQueue);
        break;
    case TRANSITION_TO_CARNIVORE:
        //We are already transitioning to carnivore, so just join the queue
        printStatus(CarnivoreStart, "Carnivore CHECKEDIN3");
        //give up the mutex so vegetarian threads can checkout
        sem_post(&mutex);
        sem_wait(&CarnivoreQueue);
        break;
    default: 
        //implicitly we are transitioning to vegetarian, but this thread already entered the store and thus can checkin
        //kinda weird, but oh well
        printStatus(CarnivoreStart, "Carnivore CHECKEDIN4");
        sem_post(&mutex);
    }
    //start making the sandwhich order
    create_sandwich("\033[31;1mCarnivore\033[00m");
    //sandwhiches aren't required to hold the mutex because the mutex is for the scoreboard
    sem_wait(&mutex);
    //checkout this thread
    carnivores--;
    printStatus(CarnivoreStart, "carnivores FINISHED");
    
    //if the last carnivore is leaving, and we want to transition to vegetarian, then we
    //have to signal all those waiting vegetarian customers
    //and open the turnstile for vegetarians to enter the store again
    if(carnivores == 0) {
        if(status == TRANSITION_TO_VEGETARIAN) {
            int value;
            sem_getvalue(&Vegetarian_turn, &value);
            if(value == 0) {
                printStatus(CarnivoreStart, "OPENING Vegetarian TURNSTILE");
                sem_post(&Vegetarian_turn);//open the Vegetarian turnstile
            }
        }
        if(vegetarians) {
            for(int i = 0; i < vegetarians; i++) {
                //let the waiting Vegetarians through
                sem_post(&VegetarianQueue);
            }
            //Change the state of the kitchen
            status = VEGETARIAN;
            int value;
            //We may have been blocked them from checking in so 
            //now we let them check in again and start building up force majure
            //stops deadlock, but I don't think its necessary
            //perhaps somewhere else the solution is incorrect
            sem_getvalue(&Carnivore_turn, &value);
            if(value == 0) {
                sem_post(&Carnivore_turn);
            }
            printStatus(CarnivoreStart, "SIGNALING WAITING Vegetarians");
        } else {
            //there are no waiting vegetarians, so we can just make the kitchen ready
            //for either sandwich shop
            status = NEUTRAL;
            int value;
            //stops deadlock, but I don't think its necessary
            //perhaps somewhere else the solution is incorrect
            sem_getvalue(&Vegetarian_turn, &value);
            if(value == 0) {
                sem_post(&Vegetarian_turn);
            }
        }
    }

    if(status == CARNIVORE) {
        //check if this carnivore that is leaving will tip the scale
        if(vegetarians > carnivores) {
            //there are now more carnivores than vegetarians, so we 
            //will transition to vegetarians
            status = TRANSITION_TO_VEGETARIAN;
            printStatus(CarnivoreStart, "LOCKING Carnivore TURNSTILE");
            //block the Carnivore turnstile, not allowing carnviores to enter the store
            sem_wait(&Carnivore_turn);
        }
    }
    printStatus(exitStart, "Carnivore EXITED");
    //release the scoreboard mutex so other threads can proceed
    sem_post(&mutex);
}

void Vegetarian() {
     pid_t tid = syscall(SYS_gettid);
    printf("Vegetarian enter %d\n", tid);
    sem_wait(&Vegetarian_turn);//see if we are blocked from checking in
    sem_post(&Vegetarian_turn);//open the turnstile again
    sem_wait(&mutex);//acquire the lock
    vegetarians++;
    //printStatus(Vegetarianstart, "Vegetarians ARRIVED");
    switch (status) {
    case NEUTRAL:
        status = VEGETARIAN;
        printStatus(VegetarianStart, "Vegetarian CHECKEDIN1");
        sem_post(&mutex);//release the mutex
        break;
    case CARNIVORE:
        if(vegetarians > carnivores) {
            status = TRANSITION_TO_VEGETARIAN;
            sem_wait(&Carnivore_turn);//lock carnivores from entering the turnstile
        }
        printStatus(VegetarianStart, "Vegetarian CHECKEDIN2");
        sem_post(&mutex);//release the mutex
        sem_wait(&VegetarianQueue);//wait for this queue
        break;
    case TRANSITION_TO_VEGETARIAN: 
        printStatus(VegetarianStart, "Vegetarian CHECKEDIN3");
        sem_post(&mutex);//release the mutex
        sem_wait(&VegetarianQueue);//wait in the queue
        break;
    default: 
        printStatus(VegetarianStart, "Vegetarian CHECKEDIN4");
        sem_post(&mutex);//release the mutex
        break;
    }

    create_sandwich("\033[32;1mVegetarian\033[00m");
    sem_wait(&mutex);
    vegetarians--;
    //cross the fieldint value;
    printStatus(VegetarianStart, "Vegetarians FINISHED");
    
    if(vegetarians == 0) {
        if(status == TRANSITION_TO_CARNIVORE) {
            int value;
            sem_getvalue(&Carnivore_turn, &value);
            if(value == 0) {
                printStatus(VegetarianStart, "OPENING Carnivore TURNSTILE");
                sem_post(&Carnivore_turn);//open the Carnivore turnstile
            }
        }
        if(carnivores) {
            status = CARNIVORE;
            for(int i = 0; i < carnivores; i++)
                sem_post(&CarnivoreQueue);
            int value;
            //blocked them from checking in so now we let them check in again and start building up force majure
            sem_getvalue(&Vegetarian_turn, &value);
            if(value == 0) {
                sem_post(&Carnivore_turn);
            }
            printStatus(VegetarianStart, "SIGNALING WAITING carnivores");
        } else {
            status = NEUTRAL;
            int value;
            sem_getvalue(&Carnivore_turn, &value);
            if(value == 0) {
                sem_post(&Carnivore_turn);
            }
        }
    }

    if(status == VEGETARIAN) {
        if(carnivores > vegetarians) {
            status = TRANSITION_TO_CARNIVORE;
            sem_wait(&Vegetarian_turn);//lock Vegetarian turnstile
        }
    }
    printStatus(exitStart, "Vegetarians EXITED");
    sem_post(&mutex);
}