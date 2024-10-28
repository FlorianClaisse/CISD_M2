#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

static int BUF_SIZE;

int main(int argc, char *argv[]) {

    int size,
        rank;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    BUF_SIZE = size * 3;
    int *global_buffer = (int*) malloc(sizeof(int) * BUF_SIZE);
    int *my_buffer = (int*) malloc(sizeof(int) * (BUF_SIZE / size));

    if (rank == MASTER) {
        for (int i = 0; i < BUF_SIZE; i++) {
            global_buffer[i] = i * 2;
        } 
    }

    MPI_Scatter(global_buffer, BUF_SIZE / size, MPI_INT, my_buffer, BUF_SIZE / size, MPI_INT, MASTER, MPI_COMM_WORLD);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
