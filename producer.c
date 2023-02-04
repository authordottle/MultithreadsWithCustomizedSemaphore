/******* producer.c *******/
#include "headers.h"

void *producer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    char color = ((pthread_args *) args)->color;
    item elem;
    
    int id  = 0;
    int end = 0;

    while (1) {     
        pthread_mutex_lock(&mutex);

        // create a specific colored item and timestamp
        struct timeval current_time;
  	    gettimeofday(&current_time, 0);
	    int timestamp = current_time.tv_sec;
        elem.timestamp = timestamp;
        elem.color = color;

        // put an item in buffer
        ptr->buffer[ptr->in] = elem;
        ptr->in = (ptr->in + 1) % BUFFER_SIZE;

        // TODO:
        // while (ptr->in != 0) {
        //     pthread_cond_wait(&condp, &mutex);    
        // }

        // if there is no more data to be read from the 
        // wake up consumer and release it to access buffer
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);

        if (end) break;        
    }
}
