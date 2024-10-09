#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define n 50000000
//#define n 100000000

void assign( double *mycoord, int i ) {
  mycoord[3*i]   = (double)i;
  mycoord[3*i+1] = (double)(2*i);
  mycoord[3*i+2] = (double)(3*i);
}

int check( double *mycoord ) {
  int i,j;

  for ( i = 0; i < n; i++ ) {
    for ( j = 0; j < 3; j++ ) {
      if ( (int)mycoord[3*i+j] != (j+1)*i )
        return 1;
    }
  }

  return 0;
}

int main( int argc, char* argv[] ) {
  double *coords;
  int i;

  /* Allocate pointers to coordinates */
  coords = (double*) malloc( 3 * n * sizeof(double) );

  for ( i = 0; i < n; i++ ) {
    /* ... Dummy assignement of point coordinates... */
    assign( coords, i );
  }

  /* ... Imagine there is a computational mechanics code here ... */

//  /* Check results */
//  if ( check( coords ) )
//    return EXIT_FAILURE;

  /* Free memory */
  free( coords );

  return EXIT_SUCCESS;
}
