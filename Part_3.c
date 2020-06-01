#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include <pthread.h>

//global variables to hold the matrix size, thread count, and size of blocks made
int N, P, block_size;

//generates a matrix that is N by N
int **generateMatrix(){
	int i, j;
	int **matrix = (int **)malloc(N * sizeof(int *)); // matirx pointer

  	for(i=0; i<N; i++){
  		matrix[i] = (int *)malloc(N * sizeof(int)); // intilzes rows for matrix

  		for(j=0; j<N; j++){
  			matrix[i][j] = rand() % 10 + 1; // sets value for element in matrix
  		}
  	}

  	return matrix; 
}

//prints array fro testing
void print_arr(int *arr){
	int i;
	for(i=0; i<10; i++){
		printf("%d, ", arr[i]);
	}
	printf("\n");
}

//prints a matrix fir testing
void print_mat(int **m){
	int i,j;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("%d,", m[i][j]);
		}
		printf("\n");
	}
}

//counts the distrobution for each number in a matrix from 1 - 10
void *getDistribution(void *arg){
	int **block = arg; //casts the arguments to an int matrix
	int i, j;
	int *freq = (int *)malloc(10 * sizeof(int)); // holds the frequencies fro the numbers from 1- 10
	
	//loops through the matrix
	for(i=0; i<block_size; i++){

		//matrix is padded with 0's at end to tell if at the end of the block 
		if(block[i][0] == 0){
			break;
		}

		//counts the frequency of each number
		for(j=0; j<N; j++){
			freq[block[i][j]-1] += 1;
		}
	}

	pthread_exit((void *)freq);//exits the thread and returns the frequency of the numbers for this block
}


//calculates the distrobution of the numebrs from 1 - 10 and times the exectution time
//takes in size of matrix and numebr of threads to perform
int main(int argc, char *argv[]){

	//checks for arguments
	if(argc != 3){
		printf("%s: Usage: %s <size of matrix> <number of threads>\n", argv[0], argv[0]);
		exit(1);
	}

	// counters for matrix row, block row, and threads respectivly
	int i=0, j=0, k=0; 
	N = atoi(argv[1]);//size of matrix
	P = atoi(argv[2]);//number of threads

	//checks if the size of the matrix can be devided evenly by the number of threads
	if(N%P == 0){
		block_size = N/P; // block size is even
	}
	else{
		block_size = (N/P)+1; 
	}

	int **matrix = generateMatrix(); //matrix to calculate distrobution of numbers for
	int **blocks[P];//array of blocks made for each thread to calculate the distrobution for
	int freq[10] = {0}; // total frequency of numbers
	pthread_t threads[P]; // thread array
	int rc;//return code from threads

	// allocates memory for each block 
	for(i=0; i<P; i++){
		blocks[i] = (int **)malloc(block_size * sizeof(int *));
	}

	i = 0;

	timing_start(); // start timing

	//loops through scource matrix
	while(i < N){

		//if on last thread fill current block till end of scource matrix
		if(k == P-1){
			blocks[k][j] = matrix[i];
			j++;
			i++;
			continue;
		}

		//if on last row of block 
		else if(j == block_size-1){

			//if block sizes are even or block sizes should be rounded up
			//set last row in current block to next row in scource matrix
			if(N%P == 0 || 2*(N%P) >= P){
				blocks[k][j] = matrix[i];
				j++;
				i++;
			}
			//else pad last row in block with 0's
			else{
				blocks[k][j] = (int *)malloc(N * sizeof(int));
				j++;
			}

			//create thread with current block of scource matrix
			rc = pthread_create(&threads[k], NULL, getDistribution, (void *)blocks[k]);
			k++;//next thread
			j = 0;//new block

			//check for erro on creation of thread
			if(rc){
				printf("ERROR on create: %d\n", rc);
				exit(1);
			}
		}

		//else set current row in block to the currnt row of the scource matrix
		else{
			blocks[k][j] = matrix[i];
			j++;
			i++;
		}
	}

	//if blocks are not of even sizes
	if(N%P != 0){
		//if last block should be smaller than all other blocks
		//pad last row with 0's
		if(2*(N%P) >= P){
			blocks[k][j] = (int *)malloc(N * sizeof(int));
		}
	}

	//create last thread
	rc = pthread_create(&threads[k], NULL, getDistribution, (void *)blocks[k]);

	if(rc){
		printf("ERROR on create: %d\n", rc);
		exit(1);
	}

	void *status; //return value from threads
	int *returned_freqs = (int *)malloc(10 * sizeof(int));//for cast from status to an array of ints

	//join returning threads
	for(i=0; i<P; i++){
		rc = pthread_join(threads[i], &status);

		//check for error on join
		if(rc){
			printf("ERROR on join: %d\n", rc);
			exit(1);
		}

		//combine returned frequencies
		returned_freqs = status;
		for(j=0; j<10; j++){
			freq[j] += returned_freqs[j];
		}
	}

	//print total distrobution of numbers between 1 - 10 in scource matrix
	for(i=0; i<10; i++){
		printf("%d: %d times\n", i+1, freq[i]);
	}

	timing_stop(); // stop timing
	print_timing(); // print timinh
	pthread_exit(NULL); // exit main thread
}