#include <stdlib.h>
#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <helper_cuda.h>

#define THREAD_PER_BLOCK 256

/********************** kernel **************************/
__global__ void saxpy(int n, float a, float *x, float *y) {
    /* TODO : Calcul de l'indice i */
    //int i_vect_line = blockIdx.x;
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    /* Calcul de saxpy */
    if (i < n) 
        y[i] = a * x[i] + y[i];
}

/********************** main **************************/
int main(void) {
    int N = 1 << 20;
    unsigned long vec_size = sizeof(float) * N;
    float *x, *y, *gpu_x, *gpu_y;
    
    x = (float*) malloc(vec_size);
    y = (float*) malloc(vec_size);

    /* Allocation de l'espace pour gpu_x et gpu_y qui vont recevoir x et y sur le GPU */
    cudaMalloc((void **) &gpu_x, vec_size);
    cudaMalloc((void **) &gpu_y, vec_size);
    
    /* Initialisation de x et y */
    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    /* Copie de x et y sur le GPU dans gpu_x et gpu_y respectivement */
    cudaMemcpy(gpu_x, x, vec_size, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_y, y, vec_size, cudaMemcpyHostToDevice);
    
    /* Appel au kernel saxpy sur les N éléments avec a = 2.0f */
    saxpy<<<N/THREAD_PER_BLOCK, THREAD_PER_BLOCK>>>(N, 2.0, gpu_x, gpu_y);
    
    /* Copie du résultat dans y */
    cudaMemcpy(y, gpu_y, vec_size, cudaMemcpyDeviceToHost);

    float maxError = 0.0f;
    for (int i = 0; i < N; i++) {
        if(y[i] != 4.0f)
            printf("not equal %d %f %f\n", i, y[i], x[i]);

        maxError = max(maxError, abs(y[i]-4.0f));
    }
    printf("Max error: %f\n", maxError);

    /* Libération de la mémoire sur le GPU */
    cudaFree(gpu_x);
    cudaFree(gpu_x);

    free(x);
    free(y);
}
