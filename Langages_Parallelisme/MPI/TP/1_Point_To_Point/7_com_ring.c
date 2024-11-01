#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAG 1000
#define SIZE 1
#define TYPE MPI_INT
#define MASTER 0

int main(int argc, char *argv[]) {
    int size,
        rank,
        token = 10;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER) {
        MPI_Send(&token, SIZE, TYPE, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(&token, SIZE, TYPE, size - 1, TAG, MPI_COMM_WORLD, NULL);
        printf("token : %d", token);
    }
    if (rank > 0) {
        MPI_Recv(&token, SIZE, TYPE, rank - 1, TAG, MPI_COMM_WORLD, NULL);
        token *= 2;
    }
    if (rank < size - 1) {
        MPI_Send(&token, SIZE, TYPE, rank + 1, TAG, MPI_COMM_WORLD);
    }
    if (rank == size - 1)
        MPI_Send(&token, SIZE, TYPE, MASTER, TAG, MPI_COMM_WORLD);

    return MPI_Finalize();
}
