#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define BUF_SIZE 10

int main(int argc, char *argv[]) {
    
    int rank,
        size;

    int *buffer = (int*) malloc(sizeof(int) * BUF_SIZE);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER) {
        for (int i = 0; i < BUF_SIZE; i++) { 
            buffer[i] = i * 2;
        }
    }

    MPI_Bcast(buffer, BUF_SIZE, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank != MASTER) {
        printf("rank: %d, array: [", rank);
        for (int i = 0; i < BUF_SIZE; i++) {
            printf("%d, ", buffer[i]);
        }
        printf("]\n");
    }

    MPI_Finalize();

    return 0;
}
