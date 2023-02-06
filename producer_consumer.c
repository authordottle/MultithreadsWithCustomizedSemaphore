/********* producer_consumer.c ***********/
// #include "headers.h"

item produce_item(char *color) {
    item elem;
    struct timeval current_time;
    gettimeofday(&current_time, 0);
    int timestamp = current_time.tv_usec;
    elem.timestamp = timestamp;
    elem.color = color;
   	 
    return elem;
}

// variables shared by all threads
static int producer_awake = 1;
static int consumer_awake = 0;
    
void *producer(void *args) {
	shared_struct *ptr = ((pthread_args *) args)->ptr;
	FILE *fd = ((pthread_args *) args)->fd;
	char *color = ((pthread_args *) args)->color;
    
	printf("producer_consumer.c: Producer %ld %s is here. \n", pthread_self(), color);
	for (int i = 1; i <= ITEM_NUM; i ++) {
   	    // printf("producer_consumer.c: producer ptr->in before wait is %d. \n", ptr->in);
    	// printf("%d %d", full, empty);
    	while(full == BUFFER_SIZE && producer_awake) {
        	sem_wait(global_sem);
        	printf("producer wait\n");
    	}
    	producer_awake = 1;
    	printf("producer awake\n");
   	 
    	// put an item in buffer and producer file
    	item elem = produce_item(color);
    	printf("producer_consumer.c: Producer %s generates %s %d. \n", color, elem.color, elem.timestamp);
    	fprintf(fd, "%s %d\n", elem.color, elem.timestamp);  
    	ptr->buffer[ptr->in] = elem;
    	ptr->in = (ptr->in + 1) % BUFFER_SIZE;
    	full ++;
    	empty --;
   	 
    	// printf("producer_consumer.c: producer ptr->in after saving into file is %d. \n", ptr->in);

    	// if there is no more space in buffer
    	// wake up consumer and release the lock to access buffer
    	printf("full? %d\n",full);
    	printf("empty? %d\n", empty);
    	if (full == BUFFER_SIZE && empty == 0) {
      	sem_signal(global_sem);
      	consumer_awake = 1;
      	printf("consumer go\n");
    	}
	}
}

void *consumer(void *args) {
	countdown = ITEM_NUM * PRODUCER_NUM;
	shared_struct *ptr = ((pthread_args *) args)->ptr;
	FILE *fd = ((pthread_args *) args)->fd;
	item elem;
    
	printf("producer_consumer.c: Consumer %ld is here.\n", pthread_self());
    // printf("producer_consumer.c: con ptr->out before loop is %d. \n", ptr->out);

	// global variable assigned value
	end = 0;

	// Read data
	while (1) {
    	// buffer is empty
    	while (empty == BUFFER_SIZE && consumer_awake) {
        	sem_wait(global_sem);   
        	printf("cons wait\n");
    	}  
    	consumer_awake = 1;
    	printf("cons awake\n");
   	 
    	// get an item from buffer and print into consumer txt
    	elem = ptr->buffer[ptr->out];
    	printf("producer_consumer.c: Consumer outputs %s %d\n", elem.color, elem.timestamp);
    	fprintf(fd, "%s %d\n", elem.color, elem.timestamp);  
    	countdown --;
    	if (countdown == 0) {
      	end = 1;
    	}
    	ptr->out = (ptr->out + 1) % BUFFER_SIZE;
    	full --;
    	empty ++;
   	 
        // printf("producer_consumer.c: con ptr->out after printing into file is %d. \n", ptr->out);
 
    	// if there is no more data to be read from buffer
    	// wake up producer and release the lock to access buffer
    	if (empty == BUFFER_SIZE) {
      	sem_signal(global_sem);
      	producer_awake = 1;
      	printf("producer go\n");
    	}
 
    	if (end) {
    	break;
    	}
	}
}
