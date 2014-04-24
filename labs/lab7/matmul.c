/*
 * Matrix Multiply.
 *
 * This is a simple matrix multiply program which will compute the product
 *
 *                C  = A * B
 *
 * A ,B and C are both square matrix. They are statically allocated and
 * initialized with constant number, so we can focuse on the parallelism.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define ORDER 1000   // the order of the matrix
#define AVAL  3.0    // initial value of A
#define BVAL  5.0    // initial value of B
#define TOL   0.001  // tolerance used to check the result

#define N ORDER
#define P ORDER
#define M ORDER

double A[N][P];
double B[P][M];
double C[N][M];

// Initialize the matrices (uniform values to make an easier check)
void matrix_init(void) {
	int i, j;

	// A[N][P] -- Matrix A
	for (i=0; i<N; i++) {
		for (j=0; j<P; j++) {
			A[i][j] = AVAL;
		}
	}

	// B[P][M] -- Matrix B
	for (i=0; i<P; i++) {
		for (j=0; j<M; j++) {
			B[i][j] = BVAL;
		}
	}

	// C[N][M] -- result matrix for AB
	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			C[i][j] = 0.0;
		}
	}
}

// The actual mulitplication function, totally naive
double matrix_multiply(void) {
	int i, j, k;
	double start, end;

	// timer for the start of the computation
	// Reorganize the data but do not start multiplying elements before 
	// the timer value is captured.
	start = omp_get_wtime(); 

	for (i=0; i<N; i++){
		for (j=0; j<M; j++){
			for(k=0; k<P; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	// timer for the end of the computation
	end = omp_get_wtime();
	// return the amount of high resolution time spent
	return end - start;
}

// Function to check the result, relies on all values in each initial
// matrix being the same
int check_result(void) {
	int i, j;

	double e  = 0.0;
	double ee = 0.0;
	double v  = AVAL * BVAL * ORDER;

	for (i=0; i<N; i++) {
		for (j=0; j<M; j++) {
			e = C[i][j] - v;
			ee += e * e;
		}
	}

	if (ee > TOL) {
		return 0;
	} else {
		return 1;
	}
}

// main function
int main(int argc, char **argv) {
	int correct;
	double run_time;
	double mflops;

	// initialize the matrices
	matrix_init();
	// multiply and capture the runtime
	run_time = matrix_multiply();
	// verify that the result is sensible
	correct  = check_result();

	// Compute the number of mega flops
	mflops = (2.0 * N * P * M) / (1000000.0 * run_time);
	printf("Order %d multiplication in %f seconds \n", ORDER, run_time);
	printf("Order %d multiplication at %f mflops\n", ORDER, mflops);

	// Display check results
	if (correct) {
		printf("\n Hey, it worked");
	} else {
		printf("\n Errors in multiplication");
	}
	printf("\n all done \n");

	return 0;
}
