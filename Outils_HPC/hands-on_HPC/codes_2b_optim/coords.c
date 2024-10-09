#include <stdlib.h>
#include <string.h>

#define n 50000000
//#define n 100000000

void point_allocate( double **mycoord, int i, int size ) {
  mycoord[i] = (double*) malloc( size * sizeof(double) );
}

void assign( double **mycoord, int i ) {
  mycoord[i][0] = (double)i;
  mycoord[i][1] = (double)(2*i);
  mycoord[i][2] = (double)(3*i);
}

int main( int argc, char* argv[] ) {
  double **coords;
  int i;

  /* Allocate pointers to coordinates */
  coords = (double**) malloc( n * sizeof(double*) );

  for ( i = 0; i < n; i++ ) {
    /* For each point, allocate an array of 3 coordinates */
    point_allocate( coords, i, 3 );
    /* ... Dummy assignement of point coordinates... */
    assign( coords, i );
  }

  /* ... Imagine there is a computational mechanics code here ... */

  /* Free memory */
  for ( i = 0; i < n; i++ ) {
    free( coords[i] );
  }
  free( coords );

  return EXIT_SUCCESS;
}
