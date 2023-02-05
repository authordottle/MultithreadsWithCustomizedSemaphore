/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
// #include <semaphore.h>

#define BUFFER_SIZE 2
#define PRODUCER_NUM 3
#define CONSUMER_NUM 1
#define COLOR_RED "RED"
#define COLOR_BLK "BLACK"
#define COLOR_WHITE "WHITE"
#define ITEM_NUM 1000

// item printed in buffer and producer and consumer files
typedef struct {
    char color;                     
    int timestamp;       
} item;

// buffer with shared variable in and out
typedef struct {
    int in;                        
    int out;                      
    item buffer[BUFFER_SIZE];    
} shared_struct;

// functions                            
void *producer(void *args);                                             // producer thread
void *consumer(void *args);                                             // consumer thread

// semaphore
typedef pthread_mutex_t mutex;
typedef pthread_cond_t cond;

typedef struct {
    int value;          // value for semaphore
    int counter;        // counter/wakeups             
    mutex *mutex;       // pthread_mutex variable; controls access to critical region
    cond *cond;         // pthread_condition variable
} semaphore;

// functions

// other variables
pthread_attr_t attr;  

// thread arguments 
typedef struct {
    shared_struct *ptr;             // shared by producer threads and consumer threads
    FILE *fd;                       // file descriptor of output file
    char color;
    semaphore *sem;
} pthread_args;

// TODO
// semaphore empty = BUFFER_SIZE;      // counts empty buffer slots
// semaphore full = 0;                 // counts full buffer slots