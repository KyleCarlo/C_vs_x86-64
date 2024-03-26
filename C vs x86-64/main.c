#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SAXPY_c.h"

extern void saxpy_asm(size_t n, float A, float* X, float* Y, float* Z);

int main() {
	//int n = {};
	
	const size_t n = 1 << 30; // array size
	const size_t ARRAY_BYTES = n * sizeof(float); // size in bytes
	clock_t start, end; // timer variables
	size_t error_count = 0; // sanity variable
	const int loop = 30; // loop count
	double time_taken = 0;   // time taken per run in milliseconds
	double ave_time = 0; // average time taken across 30 runs in milliseconds

	// allocate 
	float A, * X, * Y, * Z_c, * Z_asm;
	X = (float*)malloc(ARRAY_BYTES);
	Y = (float*)malloc(ARRAY_BYTES);
	Z_c = (float*)malloc(ARRAY_BYTES);
	Z_asm = (float*)malloc(ARRAY_BYTES);

	// declare scalar data
	A = 2.0f;
	// declare array data
	for (size_t i = 0; i < n; i++) {
		X[i] = (float)(i);
		Y[i] = (float)(i);
	}

	/*** ----------- C kernel call ---------- ***/

	// fill cache
	saxpy_c(n, A, X, Y, Z_c);
	ave_time = 0;

	for (int i = 0; i < loop; i++){
		start = clock();
		saxpy_c(n, A, X, Y, Z_c);
		end = clock();

		time_taken = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
		ave_time += time_taken;
	}
	
	ave_time /= loop;
	
	printf("C function took an average time of %f milliseconds across %d runs for array size %llu\n", ave_time, loop, n);

	/* CORRECTNESS CHECK 
	for (size_t i = 0; i < 10; i++) {
		printf("C Output#%llu: %f\n", i + 1, Z_c[i]);
	} */
	
	/*** ---------- x86-64 kernel call ---------- ***/

	// fill cache
	saxpy_asm(n, A, X, Y, Z_asm);
	ave_time = 0;
	
	for(int i = 0; i < loop; i++) {
		start = clock();
		saxpy_asm(n, A, X, Y, Z_asm);
		end = clock();

		time_taken = ((double)(end - start)) * 1e3 / CLOCKS_PER_SEC;
		ave_time += time_taken;
	}

	ave_time /= loop;

	printf("x86-64 function took an average time of %f milliseconds across %d runs for array size %llu\n", ave_time, loop, n);

	/* CORRECTNESS CHECK
	for (size_t i = 0; i < 10; i++) {
		printf("x86-64 Output#%llu: %f\n", i + 1, Z_asm[i]);
	} */

	/*** ---------- SANITY CHECK ---------- ***/

	for (size_t i = 0; i < n; i++) {
		if (Z_c[i] != Z_asm[i]) {
			error_count++;
		}
	}

	if(error_count == 0){
		printf("No errors found\n");
	} else {
		printf("%llu errors found\n", error_count);
	}

	return 0;
}