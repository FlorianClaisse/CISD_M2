#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MASTER 0
#define TAG 1000

int main(int argc, char *argv[]) {
    time_t value = MPI_Wtime();
    int rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER) {
        time(&value);
        printf("Get time = %ld\n", value);
    }
    if (rank > 0) 
        MPI_Recv(&value, 1, MPI_DOUBLE, rank - 1, TAG, MPI_COMM_WORLD, NULL);
    if (rank < size - 1)
        MPI_Send(&value, 1, MPI_DOUBLE, rank + 1, TAG, MPI_COMM_WORLD);
    if (rank == size - 1)
        printf("time = %ld\n", value);

    MPI_Finalize();
    return 0;
}
