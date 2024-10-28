#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 100000
#define MASTER 0
#define CHILD 1
#define TAG 1000

static inline void array_print(const int *array, const int size) {
    printf("[");
    for (int i = 0; i < size - 1; i++) 
        printf("%d, ", array[i]);

    printf("%d]\n", array[size - 1]);
}

int main(int argc, char *argv[]) {
    int rank, size;
    int tab[N];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == MASTER) {
        for (int i = 0; i < N; i++)
            tab[i] = i * 2;

        double time = MPI_Wtime();
        MPI_Send(tab, N, MPI_INT, CHILD, TAG, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(tab, N, MPI_INT, CHILD, TAG, MPI_COMM_WORLD, &status);
        time = MPI_Wtime() - time;

        array_print(tab, N);
        printf("%d - Communication time = %f\n", rank, time);
    } else if (rank == CHILD) {
        MPI_Status status;

        double time = MPI_Wtime();
        MPI_Recv(tab, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, &status);
        time = MPI_Wtime() - time;
        
        for (int i = 0; i < N; i++)
            tab[i] *= 2;

        time += MPI_Wtime();
        MPI_Send(tab, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
        time += MPI_Wtime() - time;
        printf("%d Communication time %f\n", rank, time);
    }
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}