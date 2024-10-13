#include <math.h>
#include <stdio.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define THREADS 8
#define START 1
#define END 100000000

void defaultt() {
    double x, pi, sum = 0.0;
    double PI25DT = 3.141592653589793238462643;

    double step = 1.0 / (double) END;
    
    #pragma omp simd
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    
    pi = step * sum; 
    printf("default: pi := %.16e  %.e\n", pi, fabs(pi - PI25DT));
}

static inline int index() {
    return omp_get_thread_num() % THREADS;
}

void thread() {
    double x, pi = 0.0;
    double PI25DT = 3.141592653589793238462643;
    double sum[THREADS];
    double global_sum = 0;

    double step = 1.0 / (double) END;

    for (int i = 0; i < THREADS; i++)
        sum[i] = 0;
    
    #pragma omp parallel for schedule(static) shared(step, sum) firstprivate(x) num_threads(THREADS)
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum[index()] = sum[index()] + 4.0 / (1.0 + x * x);
    }
    
    for (int i = 0; i < THREADS; i++) 
        global_sum += sum[i];

    pi = step * global_sum;
    printf("thread: pi := %.16e  %.e\n", pi, fabs(pi - PI25DT));
}

void atomic() {
    double x, pi = 0.0;
    double PI25DT = 3.141592653589793238462643;
    double sum = 0;
    double step = 1.0 / (double) END;
    
    #pragma omp parallel for schedule(static) shared(sum, step) firstprivate(x) num_threads(THREADS)
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        #pragma omp atomic update
        sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    printf("thread: pi := %.16e  %.e\n", pi, fabs(pi - PI25DT));
}

void redution() {
    double x, pi = 0.0;
    double PI25DT = 3.141592653589793238462643;
    double sum = 0;
    double step = 1.0 / (double) END;
    
    #pragma omp parallel for schedule(static) shared(step) firstprivate(x) reduction(+: sum) num_threads(THREADS)
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    printf("thread: pi := %.16e  %.e\n", pi, fabs(pi - PI25DT));
}


int main () {	
    defaultt();
    //thread();
    //atomic();
    //redution();
    return 0 ;
}
