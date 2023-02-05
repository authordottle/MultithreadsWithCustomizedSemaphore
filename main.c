#include "headers.h"

int main() {
    // prepare and open txt files
    FILE *producer_red = fopen("Producer_RED.txt", "w"); 
    FILE *producer_blk = fopen("Producer_BLACK.txt", "w"); 
    FILE *producer_white = fopen("Producer_WHITE.txt", "w"); 
    FILE *consumer_colors = fopen("Consumer_COLORS.txt", "w"); 
    if (producer_red == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    if (producer_blk == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    if (producer_white == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }
    if (consumer_colors == NULL) {
        printf("fopen failed\n");
        exit(-1);
    }

    // initialize semaphores
    semaphore *sem = (semaphore *) malloc(sizeof(semaphore));
    sem->value = BUFFER_SIZE;
    sem->counter = 0;
    mutex *sem_mutex = (mutex *) malloc(sizeof(mutex));
    int make_mutex = pthread_mutex_init(sem_mutex, NULL);
    if (make_mutex != 0) {
        perror("make_lock failed");  
        exit(-1);
    }
    sem->mutex = sem_mutex;
    cond *sem_cond = (cond *) malloc(sizeof(cond));
    int make_cond = pthread_cond_init(sem_cond, NULL);
    if (make_cond != 0) {
        perror("make_cond failed");  
        exit(-1);
    }
    sem->cond = sem_cond;

    // initialize bounded buffer
    shared_struct *ptr = (shared_struct *) malloc(sizeof(shared_struct));
    ptr->in = 0;
    ptr->out = 0;

    // initialize pthread args
    pthread_args *p_r_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_r_args->ptr = ptr;
    p_r_args->fd = producer_red;
    p_r_args->color = *COLOR_RED;
    p_r_args->sem = sem;
    pthread_args *p_b_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_b_args->ptr = ptr;
    p_b_args->fd = producer_blk;
    p_b_args->color = *COLOR_BLK;
    p_b_args->sem = sem;
    pthread_args *p_w_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_w_args->ptr = ptr;
    p_w_args->fd = producer_white;
    p_w_args->color = *COLOR_WHITE;
    p_w_args->sem = sem;
    pthread_args *c_args = (pthread_args *) malloc(sizeof(pthread_args));
    c_args->ptr = ptr;
    c_args->fd = consumer_colors;
    c_args->sem = sem;
    // c_args->color = NULL;

    // producer and consumer thread identifiers
    pthread_t producer_t[PRODUCER_NUM]; 
    pthread_t consumer_t[CONSUMER_NUM];

    // producers assigned color randomly
    char *color_arr[] = {COLOR_RED, COLOR_BLK, COLOR_WHITE};
    for (int i = 0; i < 2; i++) {
        struct timeval current_time;
        gettimeofday(&current_time, 0);
        int timestamp = current_time.tv_usec;
        // use current time as seed for random generator
        srand(timestamp); 
        int j = rand() % 3;
        char *t = color_arr[j];
        color_arr[j] = color_arr[i];
        color_arr[i] = t;
    }

    pthread_attr_t attr;  

    // create producer and consumer threads 
    for (int i = 0; i < PRODUCER_NUM; i++) {
        if (strcmp(color_arr[i], COLOR_RED) == 0) {
            pthread_create(&(producer_t[i]), &attr, producer, p_r_args);
            printf("Producer RED is created.\n");
        } else if (strcmp(color_arr[i], COLOR_BLK) == 0) {
            pthread_create(&(producer_t[i]), &attr, producer, p_b_args);
            printf("Producer BLK is created.\n");
        } else if (strcmp(color_arr[i], COLOR_WHITE) == 0) {
            pthread_create(&(producer_t[i]), &attr, producer, p_w_args);
            printf("Producer WHITE is created.\n");
        }
    } 
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_create(&(consumer_t[i]), &attr, consumer, c_args);
        printf("Consumer is created.\n");
    } 

    // wait for working threads
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_join(producer_t[i], NULL);
        printf("Producer %i joins now.\n", i);
    } 
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_join(consumer_t[i], NULL);
        printf("Consumer %i joins now.\n", i);
    } 

    // close open files
    fclose(producer_red);
    fclose(producer_blk);
    fclose(producer_white);
    fclose(consumer_colors);

    // free
    free(sem);
    free(sem_cond);
    pthread_cond_destroy(sem_cond);
    free(sem_mutex);
    pthread_mutex_destroy(sem_mutex);
    free(p_r_args);
    free(p_b_args);
    free(p_w_args);
    free(c_args);
    free(ptr);
    
    exit(0);
}
