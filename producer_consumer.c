/********* producer_consumer.c ***********/
// ! COMMENT THE INCLUDE LINE FOR COMPILING
// #include "headers.h"

shared_struct *update_shared_struct(item elem, shared_struct *ptr)
{
	ptr->buffer[ptr->in] = elem;
	ptr->in = (ptr->in + 1) % BUFFER_SIZE;
	ptr->occupied_slots++;
	ptr->available_slots--;

	return ptr;
}

void store_item(item elem, FILE *fd) 
{
	fprintf(fd, "%s %d\n", elem.color, elem.timestamp);
}

item produce_item(char *color)
{
	item elem;
	struct timeval current_time;
	gettimeofday(&current_time, 0);
	int timestamp = current_time.tv_usec;
	elem.timestamp = timestamp;
	elem.color = color;
	
	printf("producer_consumer.c: Producer %s generates %s %d. \n", color, elem.color, elem.timestamp);
	
	return elem;
}

void *producer(void *args)
{
	shared_struct *ptr = ((pthread_args *)args)->ptr;
	FILE *fd = ((pthread_args *)args)->fd;
	char *color = ((pthread_args *)args)->color;

	printf("producer_consumer.c: Producer %ld %s is here. \n", pthread_self(), color);
	for (int i = 1; i <= ITEM_NUM; i++)
	{
		mutex_lock(sem->mutex);
		while (ptr->occupied_slots == BUFFER_SIZE && !ptr->prod_state_ready)
		{
			cond_wait(sem->cond, sem->mutex);
			// printf("Producer %ld is waiting. \n", pthread_self());
		}
		// printf("Producer %ld %s is on. \n", pthread_self(), color);

		// critical region
		// put an item in buffer and producer file
		item elem = produce_item(color);
		store_item(elem, fd);
		ptr = update_shared_struct(elem, ptr);

		// if there is no more space in buffer
		// wake up consumer and release the lock to access buffer
		if ((ptr->occupied_slots) == BUFFER_SIZE && (ptr->available_slots) == 0)
		{
			ptr->prod_state_ready = 0;
			ptr->cons_state_ready = 1;
			// printf("Producer %ld will let consumer go. \n", pthread_self());
		}

		cond_broadcast(sem->cond);
		mutex_unlock(sem->mutex);
	}

	printf("Producer %ld is done. \n", pthread_self());
	pthread_exit(NULL);
}

void *consumer(void *args)
{
	shared_struct *ptr = ((pthread_args *)args)->ptr;
	FILE *fd = ((pthread_args *)args)->fd;
	item elem;

	printf("producer_consumer.c: Consumer %ld is here.\n", pthread_self());

	while (ptr->countdown != 0)
	{
		// buffer is empty
		mutex_lock(sem->mutex);
		while (ptr->available_slots == BUFFER_SIZE && !ptr->cons_state_ready)
		{
			cond_wait(sem->cond, sem->mutex);
			// printf("Consumer %ld is waiting. \n", pthread_self());
		}
		// printf("Cons %ld is on.\n", pthread_self());

		// critical region
		// get an item from buffer and print into consumer txt
		elem = ptr->buffer[ptr->out];
		printf("producer_consumer.c: Consumer outputs %s %d\n", elem.color, elem.timestamp);
		fprintf(fd, "%s %d\n", elem.color, elem.timestamp);
		ptr->out = (ptr->out + 1) % BUFFER_SIZE;
		ptr->occupied_slots--;
		ptr->available_slots++;
		ptr->countdown--;

		// if there is no more data to be read from buffer
		// wake up producer and release the lock to access buffer
		if ((ptr->available_slots) == BUFFER_SIZE)
		{
			ptr->cons_state_ready = 0;
			ptr->prod_state_ready = 1;
			printf("Consumer %ld will let one producer go. \n", pthread_self());
		}
		
		cond_broadcast(sem->cond);
		mutex_unlock(sem->mutex);
	}

	printf("Consumer %ld is done. \n", pthread_self());
	pthread_exit(NULL);
}
