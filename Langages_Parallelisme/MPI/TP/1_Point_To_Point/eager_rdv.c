#include <stdio.h>
#include <string.h>
#include "mpi.h"


int main(int argc, char** argv){

    int rank, size, tag = 1;
    char msg[1000]; // use a size=1000 to see a deadlock
    char buf[1000];
    MPI_Status stat;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(size<2){
        printf("ERROR: Use at least 2 MPI processes\n");
        MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
    }

    sprintf(msg, "Hello from P%d", rank);



    // Process 0 sends a message to Process 1
    if(rank == 0) {
        // Send to 1 (change strlen(msg) into 1000 to see a deadlock)
        MPI_Send(&msg, 1000, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
        // Recv from 1
        MPI_Recv(&buf, 1000, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &stat);
    }

    // Process 1 sends a message to Process 0
    if(rank == 1) {
        // Send to 0 (change strlen(msg) into 1000 to see a deadlock)
        MPI_Send(&msg, 1000, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        // Recv from 0
        MPI_Recv(&buf, 1000, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &stat);
    }

    if(rank==0 || rank==1)
        printf("P%d received: %s\n",rank,buf);

    MPI_Finalize();
    return 0;
}
