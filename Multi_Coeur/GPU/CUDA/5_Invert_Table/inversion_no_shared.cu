#include <stdlib.h>
#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <helper_cuda.h>

#define NB_THREADS 256

/********************** kernel **************************/
__global__ void inversion(int n, int *x, int *y) {
    int origin = threadIdx.x + blockIdx.x * blockDim.x;
    int dest = n - 1 - origin;
    y[dest] = x[origin];
}

/********************** main **************************/
int main(void) {
    int N = NB_THREADS * 1024;
    int *x = (int *) malloc(N * sizeof(int)), 
        *y = (int *) malloc(N * sizeof(int)), 
        *gpu_x = cudaMalloc((void **) &x, sizeof(int) * N), 
        *gpu_y = codaMalloc((void **) &y, sizeof(int) * N);

    for (int i = 0; i < N; i++) {
        x[i] = i;
    }

    cudaMemcpy(gpu_x, x, sizeof(int) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_y, y, sizeof(int) * N, cudaMemcpyHostToDevice);

    inversion<<<1024, NB_THREADS>>>(N, gpu_x, gpu_y);

    cudaMemcpy(x, gpu_x, sizeof(int) * N, cudaMemcpyDeviceToHost);
    cudaMemcpy(y, gpu_y, sizeof(int) * N, cudaMemcpyDeviceToHost);

    for (i = N - 12; i < N; i++)
        printf("%d\n", x[i]);
        
    for (i = 0; i < min(12, N); i++)
        printf("%d\n", y[i]);

    cudaFree(gpu_x);
    cudaFree(gpu_y);
    free(x);
    free(y);
}
