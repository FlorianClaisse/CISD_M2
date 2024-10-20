#include <stdio.h>
#include <stdlib.h>
#include<mpi.h>

#define TAG 1000
#define SIZE 1
#define TYPE MPI_INT
#define MASTER 0

int main(int argc, char *argv[]) {
    int size,
        rank;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int token = 1234567;
        for (int i = 1; i < size; i++)
            MPI_Send(&token, SIZE, TYPE, i, TAG ,MPI_COMM_WORLD);
    } else {
        int token;
        MPI_Recv(&token, SIZE, TYPE, MASTER, TAG, MPI_COMM_WORLD, NULL);
        printf("rank: %d, value: %d\n", rank, token);
    }

    return MPI_Finalize();
}
