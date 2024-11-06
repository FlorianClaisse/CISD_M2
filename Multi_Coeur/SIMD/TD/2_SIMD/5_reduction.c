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
void reduce_sum(const float *x, const int vector_size, VEC_TYPE *r) {
	assert(vector_size % VEC_NB_ELEMENTS == 0);

	VEC_TYPE a;
	if (vector_size > 0) {
		a = _mm256_load_ps(&x[0]);

		for (int i = VEC_NB_ELEMENTS; i < vector_size; i += VEC_NB_ELEMENTS) {
			VEC_TYPE v = _mm256_load_ps(&x[i]);
			a = _mm256_add_ps(a, v);
		}

		a = _mm256_add_ps(a, _mm256_permute2f128_ps(a, a, _MM_SHUFFLE(0,0,0,1)));
		a = _mm256_add_ps(a, _mm256_permute_ps(a, _MM_SHUFFLE(1,0,3,2)));
		a = _mm256_add_ps(a, _mm256_permute_ps(a, _MM_SHUFFLE(2,3,0,1)));
	} else {
		a = _mm256_setzero_ps();
	}

	*r = a;
}

int main(int argc, char *argv[]) {
	int nx = 16;
	
    float *x = aligned_alloc(VEC_BYTES, nx * sizeof(float));
	assert(x != NULL);

    for (int i = 0; i < nx; i++) {
        x[i] = i * 2;
    }

	VEC_TYPE result;
	
	reduce_sum(x, nx, &result);

	free(x);
	return 0;
}

