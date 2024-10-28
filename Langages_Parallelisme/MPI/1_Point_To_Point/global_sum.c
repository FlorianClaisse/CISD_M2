#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv){
    int rank, size, i=0, tag=1, count;
    int sum=0, tmp=0, sumtot=0;
    MPI_Request req[4];
    MPI_Status sta[4];

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size > 20){
        printf("This test is supposed to be executed with less than 21 processes\n ");
        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
    }
    
    sum=rank;
    tmp=0;

    // all processes send their ID to all other processes 
    for(i=0; i<size; i++){
        if(i!=rank){
            MPI_Isend(&rank, 1, MPI_INT, i, i, MPI_COMM_WORLD, &req[i]);
            MPI_Recv(&tmp, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &sta[i]);
            sum+=tmp;
        }
    }
    MPI_Waitall(size, req, sta);

    printf("P%d has sum=%d\n", rank, sum);

    MPI_Finalize();
  return 0;
}
