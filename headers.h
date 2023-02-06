/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h> // forgettimeofday()
// #include <semaphore.h>

#define BUFFER_SIZE 2
#define PRODUCER_NUM 3
#define CONSUMER_NUM 1
#define COLOR_RED "RED"
#define COLOR_BLK "BLACK"
#define COLOR_WHITE "WHITE"
#define ITEM_NUM 1000

// item printed in buffer and producer and consumer files
typedef struct
{
	char *color;
	int timestamp;
} item;

// buffer with shared variable in for producer and out for consumer
typedef struct
{
	int in;
	int out;
	item buffer[BUFFER_SIZE];
	int occupied_slots;
	int available_slots;
	int countdown;
	int done;
	int prod_state_ready;
	int cons_state_ready;
} shared_struct;

// thread arguments
typedef struct
{
	shared_struct *ptr; // shared by producer threads and consumer threads
	FILE *fd;			// file descriptor of output file
	char *color;
} pthread_args;

// semaphore
typedef pthread_mutex_t mutex;
typedef pthread_cond_t cond;

typedef struct
{
	int value;	  // value for semaphore
	int counter;  // # of pending signals
	mutex *mutex; // lock and unlock, controls access to critical region
	cond *cond;	  //
} semaphore;

// variables shared by all threads; mutable
static semaphore *sem;

// functions
item produce_item(char *color); // create a specific colored item and timestamp
void *producer(void *args); // producer thread
void *consumer(void *args); // consumer thread
semaphore *initialize_sem(int value);
void cond_wait(cond *cond, mutex *mutex);
void cond_signal(cond *cond);
void cond_broadcast(cond *cond);
void mutex_lock(mutex *mutex);
void mutex_unlock(mutex *mutex);
// void sem_wait(semaphore *sem);
// void sem_signal(semaphore *sem);