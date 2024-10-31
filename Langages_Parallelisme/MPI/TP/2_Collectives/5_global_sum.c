#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

int main(int argc, char *argv[]) {
    int size, me;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    {
        printf("------------ V1 ----------------\n");
        int sum = 0;

        // Process 0 does the sum of all IDs
        MPI_Reduce(&me, &sum, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
        printf("P: %d has sum=%d\n", me, sum);
        
        // and broadcast the result
        MPI_Bcast(&sum, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        printf("P: %d has sum=%d\n", me, sum);
    }

    {
        printf("------------ V2 ----------------\n");
        int sum = 0; 

        // All processes do the sum of IDs
        MPI_Allreduce(&me, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        printf("P: %d has sum=%d\n", me,sum);
    }

    return MPI_Finalize();
}
