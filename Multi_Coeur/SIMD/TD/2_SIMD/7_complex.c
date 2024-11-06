#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <immintrin.h>

#define MAX_DISP_ARRAY 10

#define VEC_TYPE            __m256i
#define VEC_BYTES           sizeof(__m256i)
#define VEC_ELEMENTS_TYPE   int32_t
#define VEC_NB_ELEMENTS     VEC_BYTES / sizeof(int32_t)

__attribute__((noinline)) 
void cmplx_sum(VEC_TYPE x, VEC_TYPE y, VEC_TYPE *z) {
	*z = _mm256_add_epi32(x, y);
}

__attribute__((noinline)) 
void cmplx_prod(VEC_TYPE x, VEC_TYPE y, VEC_TYPE *z) {

	VEC_TYPE factor = _mm256_set_epi32(-1, 1, -1, 1, -1, 1, -1, 1);
	VEC_TYPE real_mask = _mm256_set_epi32(0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff);

	VEC_TYPE real = _mm256_mullo_epi32(x, y);
	real = _mm256_mullo_epi32(factor, real);

	VEC_TYPE real_shuf = _mm256_shuffle_epi32(real, _MM_SHUFFLE(2, 3, 0, 1));
	real = _mm256_add_epi32(real, real_shuf);
	real = _mm256_and_si256(real_mask, real);

	VEC_TYPE y_shuf =  _mm256_shuffle_epi32(y, _MM_SHUFFLE(2, 3, 0, 1));
	VEC_TYPE imag = _mm256_mullo_epi32(x, y_shuf);
	VEC_TYPE imag_shuf =  _mm256_shuffle_epi32(imag, _MM_SHUFFLE(2, 3, 0, 1));

	imag = _mm256_add_epi32(imag, imag_shuf);
	imag = _mm256_andnot_si256(real_mask, imag);

	*z = _mm256_or_si256(real, imag);
}


int main(int argc, char *argv[]) {
	{
		__m256i x = _mm256_set_epi32(3, 8, 3, 2, 6, 4, 1, 3);
		__m256i y = _mm256_set_epi32(3, 8, 3, 2, 6, 4, 1, 3);
		__m256i z = _mm256_set1_epi32(0);

		cmplx_sum(x, y, &z);
	}

	{
		__m256i x = _mm256_set_epi32(3, 8, 3, 2, 6, 4, 1, 3);
		__m256i y = _mm256_set_epi32(3, 8, 3, 2, 6, 4, 1, 3);
		__m256i z = _mm256_set1_epi32(0);

		cmplx_prod(x, y, &z);
    }

	return 0;
}

