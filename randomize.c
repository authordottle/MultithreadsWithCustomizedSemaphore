/********* randomize.c ***********/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

// swap strings
void swap (char *a, char *b) {
	char temp = *a;
	*a = *b;
	*b = temp;
}

// generate a random permutation of arr[]
void randomize (char *arr[], int n) {   
	// create a different value from timestamp @ different time
	struct timeval current_time;
  	gettimeofday(&current_time, 0);
	srand(current_time.tv_sec);

	// start from the last element to swap
	for (int i = n - 1; i > 0; i --) {
		// pick a random index from 0 to i
		int j = rand() % (i + 1);

		// swap arr[i] with the element at random index
		swap(&arr[i], &arr[j]);
	}
}