#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0

int main(int argc, char *argv[]) {
    int size, me;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    float send = 1.1 * me;
    float *recv;

    if (me == MASTER) {
        recv = malloc(size * sizeof(float));
    } else {
        recv = NULL;
    }

    MPI_Gather(&send, 1, MPI_FLOAT, recv, 1, MPI_FLOAT, MASTER, MPI_COMM_WORLD);

    if (me == MASTER) {
        for (int i = 00; i < size; i++) {
            printf("%f, ", recv[i]);
        }
    }

    return MPI_Finalize();
}
