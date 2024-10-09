/**
 * \author Luca Cirrottola (INRIA)
 * \date October 2022
 *
 * \warning "DO NOT SHOOT THE PIANIST!" -- This program is an exercise and it
 * is not meant to be efficient, nor accurate, nor well written --
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <mpi.h>

/**
 *  Compute position of the DOF on the complete ring, given the offset
 *  of the current proc, the discretization interval, and the DOF index.
 */
double position( double xshift, double dx, int ix ) {
    return xshift + (ix-1) *dx;
}

/**
 *  Simple program to solve the 1D heat equation on a periodic domain, using an
 *  explicit Euler time stepping and a central spatial discretization scheme.
 *
 *  Physical and discretization parameters are assumed to be constant, only
 *  the global number of discretization points Nxtot (and thus the global
 *  domain length) are given as command line arguments.
 *
 *  Parallelization is performed through MPI with ghost nodes:
 *
 *  x----o____o____o____o____o----x
 *  0    1    2   ...       Nx   Nx+1
 *                           |    |
 *                           x----o____o____o____o____o----x
 *                           0    1    2   ...       Nx   Nx+1
 *                                                    |    |
 *                                                    x----o____o____o____o____o----x
 *                                                    0    1    2   ...       Nx   Nx+1
 *
 *  so internal nodes (o) on each procs have indices [ 1, Nx ], while ghost
 *  nodes (x) have indices 0 and Nx+1.
 */
int main( int argc, char* argv[] ) {
  int       nproc;                           /* MPI communicator size */
  int       rank;                            /* MPI process rank */
  const int root = 0;                        /* MPI root process */
  int       perr;                            /* MPI error */

  const double alpha = 3.352e-4;             /* Thermal diffusivity of a steel at 25 deg (m^2/s) */
  const double dx    = 1.e-2;                /* Space discretization interval (m) */
  const double dt    = 0.12;                 /* Time discretization interval (s) */
  const double A     = 20.0;                 /* Initial solution amplitude */
  const int    Nt    = 5000;                 /* Number of time steps */
  int          Nxtot;                        /* Total number of DOFs */
  int          Nx;                           /* Local number of DOFs */
  int*         allsizes;                     /* Nb. of DOFs on each proc */
  double       c     = alpha * dt / (dx*dx); /* It needs to be < 0.5 for stability */
  double       xshift;                       /* starting position of the local domain on the ring */

  int it, ix, iproc;                        /* Time step, discretization point, proc counter */


  /**
   *  Initializations
   */

  /* Command line arguments */
  if ( argc < 2 ) {
    printf("Error: missing <Nxtot> argument.\n Usage: <program> <Nxtot>\n");
    return EXIT_FAILURE;
  } else if ( argc > 2 ) {
    printf("Error: too many arguments.\n Usage: <program> <Nxtot>\n");
    return EXIT_FAILURE;
  } else {
    /* Get the total number of space intervals */
    Nxtot = atoi( argv[1] );
  }

  /* Initialize MPI */
  MPI_Init( &argc, &argv );

  /* Get process information */
  MPI_Comm_size( MPI_COMM_WORLD, &nproc );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank  );

  /* Start chronometer */
  double starttime = MPI_Wtime();

  /* Local number of DOFs */
  Nx = Nxtot / nproc;
  if ( rank < ( Nxtot % nproc ) )
    ++Nx;

  /* Ring circumference */
  double length = Nxtot*dx;

  /* Compute starting position on the ring */
  allsizes = (int*) malloc( nproc*sizeof(int) );
  perr = MPI_Allgather( &Nx, 1, MPI_INT,
                        allsizes, 1, MPI_INT,
                        MPI_COMM_WORLD );
  xshift = 0.0;
  for ( iproc = 0; iproc < rank; iproc++ )
    xshift += allsizes[iproc]*dx;

  /* Allocate the solution with one ghost node in each direction */
  double* sol     = (double*) malloc( (Nx+2)*sizeof(double) );
  double* solnext = (double*) malloc( (Nx+2)*sizeof(double) );

  /* Solution initialization on internal points */
  for ( ix = 1; ix <= Nx; ix++ ) {
    sol[ix] = A * sin( 2.0*M_PI/length * position( xshift, dx, ix ) );
  }


  /**
   *  Time iterations
   */
  for ( it = 1; it <= Nt; it++ ) {
    /**
     *  Communication of ghost node values (warning: without any check on the
     *  error or the status of the MPI communication...)
     */
    MPI_Status status;
    int left, right, tagsend, tagrecv, perr;
    left  = rank-1 >= 0    ? rank-1 : nproc-1;
    right = rank+1 < nproc ? rank+1 : 0;

    /* Send to left, receive from right */
    tagsend = left;
    tagrecv = rank;
    //printf(" Sending %d -> %d (%d) and receiving %d->%d (%d)\n",rank,left,tagsend,right,rank,tagrecv);
    perr = MPI_Sendrecv( &(sol[1]),    1, MPI_DOUBLE, left, tagsend,
                         &(sol[Nx+1]), 1, MPI_DOUBLE, right, tagrecv,
                         MPI_COMM_WORLD, &status );

    /* Send to right, recv from left */
    tagsend = nproc+right;
    tagrecv = nproc+rank;
    //printf(" Sending %d -> %d (%d) and receiving %d->%d (%d)\n",rank, right,tagsend,left,rank,tagrecv);
    perr = MPI_Sendrecv( &(sol[Nx]), 1, MPI_DOUBLE, right, tagsend,
                         &(sol[0]),  1, MPI_DOUBLE, left, tagrecv,
                         MPI_COMM_WORLD, &status );

    /**
     *  Scheme iteration: only on internal nodes
     */
    for ( ix = 1; ix <= Nx; ix++ ) {
      solnext[ix] = sol[ix] + c * ( sol[ix+1] - 2*sol[ix] + sol[ix-1] );
    }

    /**
     *  Exchange pointers: next solution becomes the current solution
     */
    double *tmp;
    tmp = sol;
    sol = solnext;
    solnext = tmp;
  }

  /* Stop chronometer */
  double endtime = MPI_Wtime();

  /* Compute and visualize the inf norm of the error */
  double maxval = 0.0;
  for ( ix = 1; ix <= Nx; ix++ ) {
    double sol0 = A * sin( 2.0*M_PI/length * position( xshift, dx, ix ) );
    double coeff = M_PI/length*alpha;
    double err = fabs( sol[ix] - sol0 * exp( -coeff*coeff * it*dt) );
    if ( err > maxval )
      maxval = err;
  }
  double norm;
  perr = MPI_Reduce( &maxval, &norm, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD );

  if ( rank == root ) {
    printf( "/nNb. of procs: %d\n", nproc );
    printf( "/nNb. of DOFs: %d\n", Nxtot );
    printf( "Error inf norm: %f\n", norm );
    printf( "Elapsed time: %f\n\n", endtime-starttime );
  }


//  /* Gather solution and store solution to file */
//  int* displs;
//  double* soltot;
//  if ( rank == root ) {
//    displs = (int*) malloc( (nproc+1)*sizeof(int) );
//    displs[0] = 0;
//    for ( iproc = 0; iproc < nproc; iproc++ ) {
//      displs[iproc+1] = displs[iproc] + allsizes[iproc];
//    }
//    assert( displs[ nproc ] == Nxtot );
//
//    soltot = (double*) malloc( Nxtot*sizeof(double) );
//  }
//  perr = MPI_Gatherv( &(sol[1]), Nx, MPI_DOUBLE,
//                      soltot, allsizes, displs, MPI_DOUBLE,
//                      root, MPI_COMM_WORLD );
//  if ( rank == root ) {
//    free( displs );
//    FILE* fid;
//    fid = fopen("solution","w");
//    for ( ix = 0; ix < Nxtot; ix++ ) {
//      double x = ix*dx;
//      double sol0 = A * sin( 2.0*M_PI/length * ix*dx );
//      double coeff = M_PI/length*alpha;
//      double exact = sol0 * exp( -coeff*coeff * it*dt);
//      fprintf( fid, "%f %f %f\n", x, soltot[ix], exact );
//    }
//    fclose(fid);
//    free( soltot );
//  }

  /* Free memory */
  free( sol );
  free( solnext );
  free( allsizes );

  /* Finalize MPI */
  MPI_Finalize();
}
