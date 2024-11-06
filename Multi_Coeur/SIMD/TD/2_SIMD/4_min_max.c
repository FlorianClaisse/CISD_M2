#include <immintrin.h>

#define VEC_TYPE            __m256i
#define VEC_BYTES           sizeof(__m256i)
#define VEC_ELEMENTS_TYPE   int32_t
#define VEC_NB_ELEMENTS     VEC_BYTES / sizeof(VEC_ELEMENTS_TYPE)

void minmax(VEC_TYPE x, VEC_TYPE y, VEC_TYPE *zmin, VEC_TYPE *zmax) {
	*zmax = _mm256_max_epi32(x, y);
	*zmin = _mm256_min_epi32(x, y);
}

void minmax2(VEC_TYPE x, VEC_TYPE y, VEC_TYPE *zmin, VEC_TYPE *zmax) {
    __m256i c = _mm256_cmpgt_epi32(x, y);

	*zmax = _mm256_or_si256(_mm256_and_si256   (c, x), _mm256_andnot_si256(c, y));
	*zmin = _mm256_or_si256(_mm256_andnot_si256(c, x), _mm256_and_si256   (c, y));
}

int main(int argc, char const *argv[]) {

    {
		__m256i x = _mm256_set_epi32(2,  15,  -7,   7, -12,   4,  -1,  -3);
		__m256i y = _mm256_set_epi32(-7,  18,   3,   4,  -1, -19,  11, -10);;
		__m256i zmin;
		__m256i zmax;

		minmax(x, y, &zmin, &zmax);
	}

	{
		__m256i x = _mm256_set_epi32(2,  15,  -7,   7, -12,   4,  -1,  -3);
		__m256i y = _mm256_set_epi32(-7,  18,   3,   4,  -1, -19,  11, -10);;
		__m256i zmin;
		__m256i zmax;

		minmax2(x, y, &zmin, &zmax);
	}
    
    return 0;
}
