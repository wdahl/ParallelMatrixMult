#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include <pthread.h>

//sums to the number provided
void *SumUpto(void *arg){
	int i;
	int sum = 0;
	int *number = arg;//sets arguments to an int type

	for(i=0; i<number[0]; i++){
		sum += i;
	}
	pthread_exit(NULL);//exits thread
}

//parallized code to sum to a specified number a certian number of times
int main(int argc, char* argv[]){
	timing_start(); // starts timing

	//checks arguments
	if(argc < 2){
		printf("%s: Usage: %s <number to sum to> [loop count]\n", argv[0], argv[0]);
		exit(1);
	}

	int p; // numebr fo threads
	int i;
	int *number = (int *)malloc(sizeof(int));
	number[0] = atoi(argv[1]); // converts argument to int

	//sets number fo threads to perform
	//defaults to 2
	if(argc == 3){
		p = atoi(argv[2]);
	}
	else{
		p = 2;
	}

	pthread_t threads[p]; // array of threads
	int rc; // return value for thread create and join to check for error

	//creates p threads
	for(i=0; i<p; i++){
		rc = pthread_create(&threads[i], NULL, SumUpto, (void *)number);

		//checks for error when creating thread
		if(rc){
			printf("ERROR on create: %d\n", rc);
			exit(1);
		}
	}

	for(i=0; i<p; i++){
		rc = pthread_join(threads[i], NULL);

		//checks for error on join
		if(rc){
			printf("ERROR on join: %d\n", rc);
			exit(1);
		}
	}
	
	timing_stop(); // stios timing
	print_timing();//prints timing
	pthread_exit(NULL);//exits main thread
}