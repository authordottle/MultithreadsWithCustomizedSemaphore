/********* semaphore.c ***********/
#include "headers.h"

void sem_wait(semaphore *sem) {
  int mutex_lock = pthread_mutex_lock(sem->mutex);
  if (mutex_lock != 0) {
    perror("lock failed");
    exit(-1);
  }
  sem->value --;

  if (sem->value < 0) {
    do {
      int n = pthread_cond_wait(sem->cond, sem->mutex);
      if (n != 0) {
        perror("pthread_cond_wait failed");
        exit(-1);
      }
    } while(sem->counter < 1);   
    sem->counter --;
  }
  int mutex_unlock = pthread_mutex_unlock(sem->mutex);
  if (mutex_unlock != 0) {
    perror("unlock failed");
    exit(-1);
  }
}

void sem_signal(semaphore *sem) {
  int mutex_lock = pthread_mutex_lock(sem->mutex);
  if (mutex_lock != 0) {
    perror("lock failed");
    exit(-1);
  }
  sem->value ++;

  if (sem->value <= 0) {
    sem->counter ++;
    int n = pthread_cond_signal(sem->cond);
    if (n != 0) {
      perror("pthread_cond_signal failed");
      exit(-1);
    }
  }
  int mutex_unlock = pthread_mutex_unlock(sem->mutex);
  if (mutex_unlock != 0) {
    perror("unlock failed");
    exit(-1);
  }
}
