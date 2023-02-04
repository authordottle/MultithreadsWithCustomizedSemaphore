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

    // initialize bounded buffer
    shared_struct *ptr = (shared_struct *) malloc(sizeof(shared_struct));
    ptr->in = 0;
    ptr->out = 0;

    // initialize pthread args
    pthread_args *p_r_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_r_args->ptr = ptr;
    p_r_args->fd = producer_red;
    p_r_args->color = COLOR_RED;
    pthread_args *p_b_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_b_args->ptr = ptr;
    p_b_args->fd = producer_blk;
    p_b_args->color = COLOR_BLK;
    pthread_args *p_w_args = (pthread_args *) malloc(sizeof(pthread_args));
    p_w_args->ptr = ptr;
    p_w_args->fd = producer_white;
    p_w_args->color = COLOR_WHITE;
    pthread_args *c_args = (pthread_args *) malloc(sizeof(pthread_args));
    c_args->ptr = ptr;
    c_args->fd = consumer_colors;
    c_args->color = NULL;

    // producer and consumer thread identifiers
    pthread_t producer_t[PRODUCER_NUM]; 
    pthread_t consumer_t[CONSUMER_NUM];

    // producers assigned any of 3 color randomly
    char *color_arr[3];
    color_arr[0] = COLOR_RED;
    color_arr[1] = COLOR_BLK;
    color_arr[2] = COLOR_WHITE;
    int n = sizeof(color_arr) / sizeof(color_arr[0]);
    randomize(color_arr, n);
    
    // create producer and consumer threads 
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_args *p_args;
        if (strcmp(color_arr[i], COLOR_RED) == 0) {
            p_args = p_r_args;
        } else if (strcmp(color_arr[i], COLOR_BLK) == 0) {
            p_args = p_b_args;
        } else if (strcmp(color_arr[i], COLOR_WHITE) == 0) {
            p_args = p_w_args;
        }
        pthread_create(&(producer_t[i]), &attr, producer, p_args);
    } 
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_create(&(consumer_t[i]), &attr, consumer, c_args);
    } 

    // wait for working threads
    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_join(producer_t[i], NULL);
    } 
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_join(consumer_t[i], NULL);
    } 
    
    // initialize semaphores
    // TODO
    semaphore *semaphore = initialize(BUFFER_SIZE);

    // if (sem_init(&mutex, 0, 1) == -1) {
    //     printf("Initilize semaphore failed\n");
    //     exit(-1);
    // }
    // if (sem_init(&empty, 0, BUFFER_SIZE) == -1) {
    //     printf("Initilize semaphore failed\n");
    //     exit(-1);
    // }
    // if (sem_init(&full, 0, 0) == -1) {
    //     printf("Initilize semaphore failed\n");
    //     exit(-1);
    // }
    // sem_init(&empty, 0, BUFFER_SIZE); 
    // sem_init(&full, 0, 0);
    // sem_init(&mutex, 0, 1);

    // get the default attributes, mutex, condp, condc
    pthread_attr_init(&attr);       
    pthread_mutex_init(&mutex, &attr);       
    pthread_cond_init(&condp, &attr);
    pthread_cond_init(&condp, &attr);
   
    // destroy mutex and cond
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condp);
    pthread_cond_destroy(&condc);
    
    // close open files
    fclose(producer_red);
    fclose(producer_blk);
    fclose(producer_white);
    fclose(consumer_colors);

    // free
    free(p_r_args);
    free(p_b_args);
    free(p_w_args);
    free(c_args);
    free(ptr);

    exit(0);
}
