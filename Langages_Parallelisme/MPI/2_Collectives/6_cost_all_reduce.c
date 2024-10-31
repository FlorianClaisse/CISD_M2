#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define NUMBER_OF_TESTS 10
#define MASTER 0

int main(int argc, char** argv) {

    int size, me;
    double t1, t2, tmin;
    double d_in, d_out;
    int nloop;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (me == MASTER && size == 1) {
	    printf("Kind\t\tnp\ttime (sec)\n");
    }

    nloop = 1000;
    tmin  = 1000;
    for (int k = 0; k < NUMBER_OF_TESTS; k++) {
	    MPI_Barrier(MPI_COMM_WORLD);
        d_in = 1.0;
        t1 = MPI_Wtime();
        for (int j = 0; j < nloop; j++) {
            MPI_Allreduce(&d_in, &d_out, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        }
        t2 = (MPI_Wtime() - t1) / nloop;
        if (t2 < tmin) tmin = t2;
    }

    if (me == MASTER) {
	    printf( "Allreduce\t%d\t%f\n", size, tmin );
    }
    
    return MPI_Finalize();
}
