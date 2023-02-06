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

void store_item(item elem, shared_struct *ptr, FILE *fd)
{
	struct shmseg *buffer_pointer = &ptr->shm_pointer;
	// char str[50];
	// sprintf(str, "%s %d", elem.color, elem.timestamp);
	// memset(buffer_pointer->buf, str, 50);
	// printf("segment contains : \n\"%s\"\n", str);
	// printf("segment contains : \n\"%s\"\n", buffer_pointer->buf);
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
		sem_wait(sem, ptr, PRODUCER);

		// critical region
		// put an item in buffer and producer file
		item elem = produce_item(color);
		ptr = update_shared_struct(elem, ptr);
		store_item(elem, ptr, fd);

		sem_signal(sem, ptr, PRODUCER);
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
		sem_wait(sem, ptr, CONSUMER);

		// critical region
		// get an item from buffer and print into consumer txt
		elem = ptr->buffer[ptr->out];
		printf("producer_consumer.c: Consumer outputs %s %d\n", elem.color, elem.timestamp);
		fprintf(fd, "%s %d\n", elem.color, elem.timestamp);
		ptr->out = (ptr->out + 1) % BUFFER_SIZE;
		ptr->occupied_slots--;
		ptr->available_slots++;
		ptr->countdown--;

		sem_signal(sem, ptr, CONSUMER);
	}
	printf("Consumer %ld is done. \n", pthread_self());
	pthread_exit(NULL);
}
