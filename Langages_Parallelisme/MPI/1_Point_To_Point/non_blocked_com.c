#include <stdio.h>
#include "mpi.h"


int main(int argc, char** argv) {

    int rank, size, i=0, tag1=1, tag2=2;
    char msg1[100000], msg2[100000];
    MPI_Request req[2];
    MPI_Status stat[2]; 

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size < 2) {
        printf("ERROR: Use at least 2 MPI processes\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
    }

    sprintf(msg1, "Hello from P%d", rank);

    if(rank == 0) {
        // Send to 1
        MPI_Isend(&msg1, 100000, MPI_CHAR, 1, tag1, MPI_COMM_WORLD, &req[0]);
        // Recv from 1
        MPI_Irecv(&msg2, 100000, MPI_CHAR, 1, tag2, MPI_COMM_WORLD, &req[1]);
    }

    if(rank == 1) {
        // Send to 0 
        MPI_Isend(&msg1, 100000, MPI_CHAR, 0, tag2, MPI_COMM_WORLD, &req[0]);
        // Recv from 0
        MPI_Irecv(&msg2, 100000, MPI_CHAR, 0, tag1, MPI_COMM_WORLD, &req[1]);
    }

    MPI_Waitall(2, req, stat);
    printf("I, P%d, received %s\n",rank, msg2);

    MPI_Finalize();
    return 0;
}
