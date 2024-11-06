#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>

#define VEC_TYPE            __m256
#define VEC_BYTES           sizeof(__m256)
#define VEC_ELEMENTS_TYPE   float
#define VEC_NB_ELEMENTS     VEC_BYTES / sizeof(float)

__attribute__((noinline)) 
float dot_product (const float *x, const float *y, const int vector_size) {
	assert(vector_size % VEC_NB_ELEMENTS == 0);
	
    float result = 0;
	if (vector_size > 0) {
		VEC_TYPE reg_acc = _mm256_setzero_ps();

		for (int i = 0; i < vector_size; i += VEC_NB_ELEMENTS) {
			VEC_TYPE reg_x;
			VEC_TYPE reg_y;

			reg_x = _mm256_load_ps(x + i);
			reg_y = _mm256_load_ps(y + i);

			reg_acc = _mm256_fmadd_ps(reg_x, reg_y, reg_acc);
		}

		reg_acc = _mm256_add_ps(reg_acc, _mm256_permute2f128_ps(reg_acc, reg_acc, _MM_SHUFFLE(0,0,0,1)));
		reg_acc = _mm256_add_ps(reg_acc, _mm256_permute_ps(reg_acc, _MM_SHUFFLE(1,0,3,2)));
		reg_acc = _mm256_add_ps(reg_acc, _mm256_permute_ps(reg_acc, _MM_SHUFFLE(2,3,0,1)));

		result = reg_acc[0];
	}

	return result;
}

int main(int argc, char *argv[]) {
	int nb_loops = 10;
	int nx = 16;
	int verbose = 0;
	int header = 0;

	float *x = aligned_alloc(VEC_BYTES, nx * sizeof(float));
	assert(x != NULL);

	float *y = aligned_alloc(VEC_BYTES, nx * sizeof(float));
	assert(y != NULL);

    for (int i = 0; i < nx; i++) {
        x[i] = i * 2;
        y[i] = i * 2;
    }

	float dp = dot_product(x, y, nx);

	free(x);
	free(y);
	return 0;
}

