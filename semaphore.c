/********* semaphore.c ***********/
// ! COMMENT THE INCLUDE LINE FOR COMPILING
// #include "headers.h"

mutex *initialize_mutex()
{
	mutex *sem_mutex = (mutex *) malloc(sizeof(mutex));
	int make_mutex = pthread_mutex_init(sem_mutex, NULL);
	if (make_mutex != 0)
	{
		perror("initialize_lock failed");
		exit(-1);
	}

	return sem_mutex;
}

cond *initialize_cond()
{
	cond *sem_cond = (cond *) malloc(sizeof(cond));
	int make_cond = pthread_cond_init(sem_cond, NULL);
	if (make_cond != 0)
	{
		perror("initialize_cond failed");
		exit(-1);
	}

	return sem_cond;
}

void cond_wait(cond *cond, mutex *mutex)
{
	int n = pthread_cond_wait(cond, mutex);
	if (n != 0)
	{
		perror("cond_wait failed");
		exit(-1);
	}
}

void cond_signal(cond *cond)
{
	int n = pthread_cond_signal(cond);
	if (n != 0)
	{
		perror("cond_signal failed");
		exit(-1);
	}
}

void cond_broadcast(cond *cond)
{
	int n = pthread_cond_broadcast(cond);
	if (n != 0)
	{
		perror("cond_broadcast failed");
		exit(-1);
	}
}

void mutex_lock(mutex *mutex)
{
	int n = pthread_mutex_lock(mutex);
	if (n != 0)
	{
		perror("lock failed");
		exit(-1);
	}
}

void mutex_unlock(mutex *mutex)
{
	int n = pthread_mutex_unlock(mutex);
	if (n != 0)
	{
		perror("unlock failed");
		exit(-1);
	}
}

semaphore *initialize_sem(int value)
{
	semaphore *sem = (semaphore *)malloc(sizeof(semaphore));
	sem->value = value;
	sem->counter = 0;
	sem->mutex = initialize_mutex();
	sem->cond = initialize_cond();

	return sem;
}

// void sem_wait(semaphore *sem) {
// 	// lock before changing value
// 	// printf("wait call on pthread id? %ld\n", pthread_self());
// 	mutex_lock(sem->mutex);
// 	sem->value --;

// 	// negative value means threads are waiting so that while no pending signals busy waiting condition
// 	if (sem->value < 0) {
// 		do {
// 			int n = pthread_cond_timedwait(sem->cond, sem->mutex, 100);
// 			if (n != 0) {
// 				perror("pthread_cond_timedwait failed");
// 				exit(-1);
// 			}
// 		} while(sem->counter < 1);
// 		sem->counter --;
// 	}
// 	mutex_unlock(sem->mutex);
// }

// void sem_signal(semaphore *sem) {
//   // lock before changing value
//   // printf("signal call on pthread id? %ld\n", pthread_self());
//   mutex_lock(sem->mutex);
//   sem->value ++;

//   // negative value means threads are waiting so that increase # of pending signals and signals condition
//   if (sem->value <= 0) {
// 	sem->counter ++;
// 	int n = pthread_cond_signal(sem->cond);
// 	if (n != 0) {
// 		perror("pthread_cond_signal failed");
// 		exit(-1);
// 	}
//   }
//   mutex_unlock(sem->mutex);
// }