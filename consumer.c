/********* consumer.c ***********/
#include "headers.h"

void *consumer(void *args) {
    shared_struct *ptr = ((pthread_args *) args)->ptr;
    FILE *fd = ((pthread_args *) args)->fd;
    item elem;
    
    // Read data
    while (1) {
        // sem_wait(&full);
        // sem_wait(&mutex);

        // get an item from buffer
        elem = ptr->buffer[ptr->out];

        // printf("%c\t%d", elem.color, elem.timestamp);  
        fprintf(fd, "%s %s", elem.color, elem.timestamp);      
        // sem_post(&mutex);
        // sem_post(&empty);
        
        // if (elem.id == -1) break;

        ptr->out = (ptr->out + 1) % BUFFER_SIZE;
    }
}
