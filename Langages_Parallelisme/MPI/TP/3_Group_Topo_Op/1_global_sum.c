#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <limits.h>

#define MASTER 0
#define TAG 1000

void my_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *type) {
    int *src = (int *) invec;
    int *dst = (int *) inoutvec;
    
    *dst += *src;
}

void my_max(void *invec, void *inoutvec, int *len, MPI_Datatype *type) {
    int *src = (int *) invec;
    int *dst = (int *) inoutvec;
    
    if (*src > *dst)
        *dst = *src;

}

int main(int argc, char *argv[]) {
    int size, me;
    int result;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    MPI_Op sum, max;

    MPI_Op_create((MPI_User_function *) my_sum, 1, &sum);
    MPI_Op_create((MPI_User_function *) my_max, 0, &max);

    /* Reduction puis diffusion */
    {
        result = 0;
        MPI_Reduce(&me, &result, 1, MPI_INT, sum, MASTER, MPI_COMM_WORLD);
        MPI_Bcast(&result, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        printf("Process: %d, has sum=%d\n", me, result);
        result = 0;
        MPI_Reduce(&me, &result, 1, MPI_INT, max, MASTER, MPI_COMM_WORLD);
        MPI_Bcast(&result, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        printf("Process: %d, has max=%d\n", me, result);
    }

    /* Reduction global */
    {
        result = 0;
        MPI_Allreduce(&me, &result, 1, MPI_INT, sum, MPI_COMM_WORLD);
        printf("Process: %d, has sum=%d\n", me, result);
        result = 0;
        MPI_Allreduce(&me, &result, 1, MPI_INT, max, MPI_COMM_WORLD);
        printf("Process: %d, has max=%d\n", me, result);
    }

    printf("Process: %d, has sum=%d\n", me, result);
    
    return MPI_Finalize();
}
