#include <stdlib.h>
#include <stdio.h>

void saxpy_c(size_t n, float A, float* X, float* Y, float* Z) {
	for (size_t i = 0; i < n; i++) {
		Z[i] = (A * X[i]) + Y[i];
	}
}