#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc, char* argv[] ) {
  int nproc;          /* MPI communicator size */
  int rank;           /* MPI process rank */

  /* Initialize MPI */
  MPI_Init( &argc, &argv );

  /* Get process information */
  MPI_Comm_size( MPI_COMM_WORLD, &nproc );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank  );

  /* Each proc says hello */
  printf("Hello! I am proc %d among %d procs.\n", rank, nproc);

  /* Finalize MPI */
  MPI_Finalize();
}
