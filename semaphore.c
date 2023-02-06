/********* semaphore.c ***********/
// ! COMMENT THE INCLUDE LINE FOR COMPILING
// #include "headers.h"

mutex *initialize_mutex()
{
	mutex *sem_mutex = (mutex *)malloc(sizeof(mutex));
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
	cond *sem_cond = (cond *)malloc(sizeof(cond));
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

void sem_wait(semaphore *sem, shared_struct *ptr, char *client)
{
	mutex_lock(sem->mutex);
	
	if (client == PRODUCER)
	{
		while (ptr->occupied_slots == BUFFER_SIZE && !ptr->prod_state_ready)
		{
			cond_wait(sem->cond, sem->mutex);
		}
	}
	else if (client == CONSUMER)
	{
		while (ptr->available_slots == BUFFER_SIZE && !ptr->cons_state_ready)
		{
			cond_wait(sem->cond, sem->mutex);
		}
	}
}

void sem_signal(semaphore *sem, shared_struct *ptr, char *client)
{
	if (client == PRODUCER)
	{
		// if there is no more space in buffer
		// wake up consumer and release the lock to access buffer
		if ((ptr->occupied_slots) == BUFFER_SIZE && (ptr->available_slots) == 0)
		{
			ptr->prod_state_ready = 0;
			ptr->cons_state_ready = 1;
			// printf("Producer %ld will let consumer go. \n", pthread_self());
		}
	}
	else if (client == CONSUMER)
	{
		// if there is no more data to be read from buffer
		// wake up producer and release the lock to access buffer
		if ((ptr->available_slots) == BUFFER_SIZE && (ptr->occupied_slots) == 0)
		{
			ptr->cons_state_ready = 0;
			ptr->prod_state_ready = 1;
			// printf("Consumer %ld will let one producer go. \n", pthread_self());
		}
	}

	cond_broadcast(sem->cond);
	mutex_unlock(sem->mutex);
}