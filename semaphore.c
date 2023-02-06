/********* semaphore.c ***********/
// #include "headers.h"

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

void sem_wait(semaphore *sem) {
  // lock before changing value
  printf("wait\n");
  printf("pthread id? %ld\n", pthread_self());
  mutex_lock(sem->mutex);
  //sem->value --;
 
  //printf("sem value? %d \n", sem->value);
  printf("sem counter? %d \n", sem->counter);
  //if (sem->value < 0) {
	do {
  	int n = pthread_cond_wait(sem->cond, sem->mutex);
  	if (n != 0) {
    	perror("pthread_cond_wait failed");
    	exit(-1);
  	}
	// } while (sem->counter == 0);
	} while(sem->counter < 1);   
	sem->counter --;
  //}
  mutex_unlock(sem->mutex);
}

void sem_signal(semaphore *sem) {
  // lock before changing value
  printf("signal\n");
  printf("pthread id? %ld\n", pthread_self());
  mutex_lock(sem->mutex);
  //sem->value ++;
 
  //printf("sem value? %d \n", sem->value);
  printf("sem counter? %d \n", sem->counter);
  //if (sem->value <= 0) {
	sem->counter ++;
	int n = pthread_cond_signal(sem->cond);
	if (n != 0) {
  	perror("pthread_cond_signal failed");
  	exit(-1);
	}
  //}
  mutex_unlock(sem->mutex);
}

semaphore *initialize(){
	semaphore *sem = (semaphore *) malloc(sizeof(semaphore));
	sem->value = 0;
	sem->counter = 0;
	mutex *sem_mutex = (mutex *) malloc(sizeof(mutex));
	int make_mutex = pthread_mutex_init(sem_mutex, NULL);
	if (make_mutex != 0) {
    	perror("make_lock failed");  
    	exit(-1);
	}
	sem->mutex = sem_mutex;
	cond *sem_cond = (cond *) malloc(sizeof(cond));
	int make_cond = pthread_cond_init(sem_cond, NULL);
	if (make_cond != 0) {
    	perror("make_cond failed");  
    	exit(-1);
	}
	sem->cond = sem_cond;
    
	return sem;
}