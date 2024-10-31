#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define NUMBER_OF_TESTS 10
#define MASTER 0

int main(int argc, char** argv) {
    int size, me;
    double t1, t2, tmin;
    int nloop;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (me == 0 && size == 1) {
	    printf("Kind\tnp\ttime (sec)\n");
    }

    nloop = 1000;
    tmin  = 1000;
    for (int k = 0; k < NUMBER_OF_TESTS; k++) {
        MPI_Barrier(MPI_COMM_WORLD);
        t1 = MPI_Wtime();
        for (int j = 0; j < nloop; j++) {
            MPI_Barrier(MPI_COMM_WORLD);
        }
        t2 = (MPI_Wtime() - t1) / nloop;
        if (t2 < tmin) tmin = t2;
    }

    if (me == MASTER) {
	    printf("Barrier\t%d\t%f\n", size, tmin);
    }

    MPI_Finalize();
    return 0;
}
