//
#include <omp.h>
#include <stdio.h>

long comp_fib_numbers(int n) {
    long  fnm1, fnm2, fn;
    
    if (n == 0 || n == 1) 
        return 1 ;

    #pragma omp task shared(fnm1, n)
    fnm1 = comp_fib_numbers(n - 1);

    #pragma omp task shared(fnm2, n)
    fnm2 = comp_fib_numbers(n - 2);

    #pragma omp taskwait
    fn = fnm1 + fnm2; 

    return fn;
}

int main() {
    long result;
    int n = 25;
    int nthreads;

    #pragma omp parallel
    #pragma omp single
    result = comp_fib_numbers(n);

    printf("Result for n = %d: %ld \n", n, result);
}
