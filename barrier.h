#ifndef BARRIER_H
#define BARRIER_H
#include <semaphore.h>

struct resuseable_barrier {
  int n;
  int waiting;
  sem_t mutex;
  sem_t turnstile_1;
  sem_t turnstile_2;  
};

void init_barrier(struct resuseable_barrier *b, int n) {
    b->n = n;
    sem_init(&b->mutex, 0, 1);
    sem_init(&b->turnstile_1, 0, 0);
    sem_init(&b->turnstile_2, 0, 0);
}

void phase1_barrier(struct resuseable_barrier *barrier)
{
  sem_wait(&barrier->mutex);
  if (++barrier->waiting == barrier->n) {
    int i;
    for (i = 0; i < barrier->n; i++) {
      sem_post(&barrier->turnstile_1);
    }
  }
  sem_post(&barrier->mutex);
  sem_wait(&barrier->turnstile_1);
}

void phase2_barrier(struct resuseable_barrier *barrier)
{
  sem_wait(&barrier->mutex);
  if (--barrier->waiting == 0) {
    int i;
    for (i = 0; i < barrier->n; i++) {
      sem_post(&barrier->turnstile_2);
    }
  }
  sem_post(&barrier->mutex);
  sem_wait(&barrier->turnstile_2);
}

void wait_barrier(struct resuseable_barrier *barrier)
{
  phase1_barrier(barrier);
  phase2_barrier(barrier);
}
#endif