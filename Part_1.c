#include <stdio.h>
#include "timing.h"
#include <stdlib.h>

//prototype
int** matmul_naive(int **A, int **B, int n);
void matmul_optimum(int **A, int **B, int n);
int** sub(int **A, int **B, int n);
int** add(int **A, int **B, int n);

int n = 100;

//generates a matrix that is N by N
int **generateMatrix(){
	int i, j;
	int **matrix = (int **)malloc(n * sizeof(int *)); // matirx pointer

  	for(i=0; i<n; i++){
  		matrix[i] = (int *)malloc(n * sizeof(int)); // intilzes rows for matrix

  		for(j=0; j<n; j++){
  			matrix[i][j] = rand() % 10 + 1; // sets value for element in matrix
  		}
  	}

  	return matrix; 
}

//Reads matrix input file and creates a matrix from the data in the file
int main(){

 	//declares the matrix and allocates its memory
	int i;
  int **matrix = generateMatrix();

  	//performs matrix multiplication using the trival solution
	matmul_naive(matrix, matrix, n);
  printf("\nTime for naive implementation:\n");
	print_timing();//print time to perfomr matrix multiplication

  //performs multiplication using strassen's algorithm
  matmul_optimum(matrix, matrix, n);
  printf("\nTime for optimum implementation:\n");
  print_timing();

	//returns 0 on sucess
	return 0;
}

//perfomrs matrix multiplication on the matrices passed to the method
int** matmul_naive(int **A, int **B, int n){
	int i, j, k;//indices of the entries in the matrices

  //Allocates memory for result matrix
  int **C = (int **)malloc(n * sizeof(int *));	
  timing_start();//begin time keeping

	//loops through matrices and perfomrs matrix multiplication
	for(i=0; i<n; i++){
		C[i] = (int *)malloc(n * sizeof(int));
		for(j=0; j<n; j++){
			for(k=0; k<n; k++){
				C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
			}
		}
	}

  timing_stop();//end time keeping
	
  return C;//returns resulting matrix
}

//performs matrix multiplication using strassens algorithm.
void matmul_optimum(int **A, int** B, int n){
   int i, j, k;

  //Allocates the sub-matrices
  int **a = (int **)malloc(n/2 * sizeof(int *));
  int **b = (int **)malloc(n/2 * sizeof(int *));
  int **c = (int **)malloc(n/2 * sizeof(int *));
  int **d = (int **)malloc(n/2 * sizeof(int *));
  for(i=0; i<n; i++){
    a[i] = (int *)malloc(n/2 * sizeof(int));
    b[i] = (int *)malloc(n/2 * sizeof(int));
    c[i] = (int *)malloc(n/2 * sizeof(int));
    d[i] = (int *)malloc(n/2 * sizeof(int));
  }

 //intilazes the sub matrices
  for (i=0; i<n/2; i++){
    for (j=0; j<n/2; j++){
      a[i][j] = A[i][j];
    }
  }

  for (i=0; i<n/2; i++){
    for (j=n/2; j<n; j++){
      b[i][j-(n/2)] = A[i][j];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=0; j<n/2; j++){
      c[i-(n/2)][j] = A[i][j];
    }
  }

  for(i=n/2; i<n; i++){
    for(j=n/2; j<n; j++){
      d[i-(n/2)][j-(n/2)] = A[i][j];
    }
  }

  timing_start();

  //strassens algorithm
  int **p1 = matmul_naive(a, sub(b,d,n/2), n/2);
  int **p2 = matmul_naive(add(a,b,n/2), d, n/2);
  int **p3 = matmul_naive(add(c,d,n/2), a, n/2);
  int **p4 = matmul_naive(d, sub(c,a,n/2), n/2);
  int **p5 = matmul_naive(add(a,d,n/2), add(a,d,n/2), n/2);
  int **p6 = matmul_naive(sub(b,d,n/2), add(c,d,n/2), n/2);
  int **p7 = matmul_naive(sub(a,c,n/2), add(a,b,n/2), n/2);

  //makes the sub-matrices of the result matrix
  a = add(sub(add(p5,p4,n/2),p2,n/2),p6,n/2);
  b = add(p1,p2,n/2);
  c = add(p3,p4,n/2); 
  d = sub(sub(add(p1,p5,n/2),p3,n/2),p7,n/2);

  timing_stop();
}

//adds two matrices
int **add(int **A, int **B, int n){
  int i, j;
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      A[i][j] = A[i][j] + B[i][j];
    }
  }

  return A;
}

//subtacts two matricies
int **sub(int **A, int **B, int n){
  int i, j;
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      A[i][j] = A[i][j] - B[i][j];
    }
  }

  return A;
}