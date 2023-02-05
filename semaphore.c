/********* semaphore.c ***********/
#include "headers.h"

void mutex_lock(mutex *mutex) {
  int n = pthread_mutex_lock(mutex);
  if (n != 0) {
    perror("lock failed");
    exit(-1);
  }
}

void mutex_unlock(mutex *mutex) {
  int n = pthread_mutex_unlock(mutex);
  if (n != 0) {
    perror("unlock failed");
    exit(-1);
  }
}

void _wait(semaphore *sem) {
  mutex_lock(sem->mutex);
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
  mutex_unlock(sem->mutex);
}

void _signal(semaphore *sem) {
  mutex_lock(sem->mutex);
  sem->value ++;

  if (sem->value <= 0) {
    sem->counter ++;
    int n = pthread_cond_signal(sem->cond);
    if (n != 0) {
      perror("pthread_cond_signal failed");
      exit(-1);
    }
  }
  mutex_unlock(sem->mutex);
}
