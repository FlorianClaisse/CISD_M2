#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int size, me;
    MPI_Comm dup_world, pair_world;
    int dup_size, dup_me,
        pair_size, pair_me;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    MPI_Comm_dup(MPI_COMM_WORLD, &dup_world);

    MPI_Comm_size(dup_world, &dup_size);
    MPI_Comm_rank(dup_world, &dup_me);

    printf("Original size=%d, rank=%d, new size=%d, rank=%d\n", size, me, dup_size, dup_me);

    MPI_Comm_split(dup_world, dup_me % 2, me, &pair_world);

    MPI_Comm_size(pair_world, &pair_size);
    MPI_Comm_rank(pair_world, &pair_me);

    printf("Pair size=%d, rank=%d, original rank=%d\n", pair_size, pair_me, dup_me);
    
    return MPI_Finalize();
}
