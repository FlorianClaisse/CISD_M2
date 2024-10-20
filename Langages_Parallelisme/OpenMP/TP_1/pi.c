#include <math.h>
#include <stdio.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define START 1
#define END 100000000
#define PI25DT 3.141592653589793238462643

void defaultt() {
    double x, pi, sum = 0.0;

    double step = 1.0 / (double) END;
    
    double start = omp_get_wtime();
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    double end = omp_get_wtime();
    
    pi = step * sum; 
    printf("default: pi := %.16e  %.e, time = %f\n", pi, fabs(pi - PI25DT), (end - start));
}

void thread() {
    double x, pi = 0.0;
    const int nb_threads = omp_get_max_threads();
    double sum[nb_threads];
    double global_sum = 0;

    double step = 1.0 / (double) END;

    for (int i = 0; i < nb_threads; i++)
        sum[i] = 0;
    
    double start = omp_get_wtime();
    #pragma omp parallel shared(step, sum)
    {
        int id = omp_get_thread_num();
        #pragma omp for private(x)
        for (int i = START; i <= END; i++) {
            x = (i - 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }
    double end = omp_get_wtime();

    for (int i = 0; i < nb_threads; i++) 
        global_sum += sum[i];

    pi = step * global_sum;
    printf("thread: pi := %.16e  %.e, time = %f\n", pi, fabs(pi - PI25DT), (end - start));
}

void atomic() {
    double x, pi = 0.0;
    double sumtmp, sum = 0;
    double step = 1.0 / (double) END;
    
    double start = omp_get_wtime();
    #pragma omp parallel default(none) private(sumtmp) shared(step, sum)
    {
        sumtmp = 0;
        #pragma omp for private(x) nowait
        for (int i = START; i <= END; i++) {
            x = (i - 0.5) * step;
            sumtmp += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic update
        sum += sumtmp;
    }
    double end = omp_get_wtime();

    pi = step * sum;
    printf("atomic: pi := %.16e  %.e, time = %f\n", pi, fabs(pi - PI25DT), (end - start));
}

void redution() {
    double x, pi = 0.0;
    double sum = 0;
    double step = 1.0 / (double) END;
    
    double start = omp_get_wtime();
    #pragma omp parallel for default(none) private(x) shared(step) reduction(+: sum)
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    start = omp_get_wtime() - start;

    pi = step * sum;
    printf("reduction: pi := %.16e  %.e, time = %f", pi, fabs(pi - PI25DT), start);
}

void task() {
    double x, pi = 0.0;
    double sum = 0;
    double step = 1.0 / (double) END;
    
    double start = omp_get_wtime();
    #pragma omp parallel
    #pragma omp single
    #pragma omp parallel for shared(step) firstprivate(x) reduction(+: sum)
    for (int i = START; i <= END; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    start = omp_get_wtime() - start;

    pi = step * sum;
    printf("task: pi := %.16e  %.e, time = %f", pi, fabs(pi - PI25DT), start);
}


int main () {	
    defaultt();    
    thread();
    atomic();
    redution();
    return 0 ;
}
