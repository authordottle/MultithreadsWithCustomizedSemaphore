/********* consumer.c ***********/
#include "headers.h"

void *consumer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    semaphore *sem = ((pthread_args *) args)->sem;
    item elem;
    
    // Read data
    while (1) {
        // TODO
        // pthread_mutex_lock(&mutex);
        
        // get an item from buffer and print into consumer txt
        elem = ptr->buffer[ptr->out]; 
        fprintf(fd, "%s %d", elem.color, elem.timestamp);  
        ptr->out = (ptr->out + 1) % BUFFER_SIZE;

        // printf("%c\t%d", elem.color, elem.timestamp); 

        // TODO: BUFFER
        // while (ptr->out == 0) {
        //     pthread_cond_wait(&condc, &mutex);    
        // }  

        // pthread_cond_signal(&condp);
        // pthread_mutex_unlock(&mutex);        
    }
}
