#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int rank,
        size;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("rank: %d, start\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);

    sleep(rank);
    printf("rank: %d, finish and wait\n", rank);
    
    MPI_Barrier(MPI_COMM_WORLD);
    printf("rank: %d, all finish\n", rank);

    MPI_Finalize();
    
    return 0;
}
