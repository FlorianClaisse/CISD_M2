#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, total_size;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &total_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank % 2 == 0)
        printf("Je suis le processus %d et je suis pair\n", rank);
    else
        printf("Je suis le processus %d et je suis impair\n", rank);
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}