#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

//sums up to the number specified
void SumUpto(int number){
	int i;
	int sum = 0;
	for(i=0; i<number; i++){
		sum += i;
	}
}

//prints the time taken to sum to a spceifed number
int main(int argc, char* argv[]){
	timing_start();//starts timing

	//makes sure correc tnumber of arguments is given
	if(argc < 2){
		printf("%s: Usage: %s <number to sum to> [loop count]\n", argv[0], argv[0]);
		exit(0);
	}

	int p;//loops to make
	int i;//counter
	int number = atoi(argv[1]);//converts the number given by the user from string to int

	//sets number of loops to make to the number sepecified by the user
	//if no number specifed deafults to 2
	if(argc == 3){
		p = atoi(argv[2]);
	}
	else{
		p = 2;
	}

	//calss the sumto loop p times
	for(i=0; i<p; i++){
		SumUpto(number);
	}

	timing_stop();//stops timing
	print_timing();//pritns timing
	return 0;
}