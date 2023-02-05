/******* producer.c *******/
#include "headers.h"

void *producer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    char color = ((pthread_args *) args)->color;
    semaphore *sem = ((pthread_args *) args)->sem;
    item elem;
    
    printf("Producer %s is here.\n", color);
    
    int id  = 0;
    int end = 0;

    for (int i = 1; i <= ITEM_NUM; i ++) {  
        // TODO
        int unlock_mutex = pthread_mutex_unlock(sem->mutex);
        if (unlock_mutex != 0) {
            perror("unlock failed");
            exit(-1);
        }

        // create a specific colored item and timestamp
        struct timeval current_time;
        gettimeofday(&current_time, 0);
        int timestamp = current_time.tv_usec;
        elem.timestamp = timestamp;
        elem.color = color;

        // put an item in buffer and producer file
        ptr->buffer[ptr->in] = elem;
        fprintf(fd, "%s %d", elem.color, elem.timestamp);  
        ptr->in = (ptr->in + 1) % BUFFER_SIZE;

        // TODO:
        // while (ptr->in != 0) {
        //     pthread_cond_wait(&condp, &mutex);    
        // }

        // if there is no more data to be read from the 
        // wake up consumer and release it to access buffer
        // TODO
        // pthread_cond_signal(&condc);
        // pthread_mutex_unlock(&mutex);
    }     

    pthread_exit(0);
}
