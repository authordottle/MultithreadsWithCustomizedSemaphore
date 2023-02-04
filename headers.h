/***** headers.h ******/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
// One of the objectives of this assignment is to implement counting semaphores using POSIX mutex and condition variables. 
// Hence, NO use semaphore.h (which is part of the 3RT library) or any other counting semaphore library.
// #include <semaphore.h>

#define BUFFER_SIZE 2
#define PRODUCER_NUM 3
#define CONSUMER_NUM 1
#define COLOR_RED "RED"
#define COLOR_BLK "BLACK"
#define COLOR_WHITE "WHITE"

typedef struct {
    char color;                     // item color
    int timestamp;                  // item timestamp
} item;

typedef struct {
    int in;                         // shared variable in
    int out;                        // shared variable out
    item buffer[BUFFER_SIZE];       // bounded buffer with size BUFFER_SIZE
} shared_struct;

typedef struct {
    shared_struct *ptr;             // shared by producer threads and consumer threads
    FILE *fd;                       // file descriptor of output file
    char color;
} pthread_args;

void *randomize(char *array, int n);        // shuffle function using Fisher–Yates shuffle Algorithm
void *producer(void *args);         // producer thread
void *consumer(void *args);         // consumer thread

// sem_t empty;
// sem_t full;
// sem_t mutex;

