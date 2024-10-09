#include <stdlib.h>
#include <string.h>

#define n 2048
typedef double Type;

/**
 * Compute the average of two matrices: C = 0.5* ( A + B )
 */
void average( Type* A, Type* B, Type* C ) {
  int i, j;
  int *s1, *s2;

  /* Define the order of the three subscripts */
  s1 = &i;
  s2 = &j;

  /* Perform the averaging */
  for ( *s1 = 0; *s1 < n; (*s1)++ ) {
    for ( *s2 = 0; *s2 < n; (*s2)++ ) {
      C[i*n+j] = ( A[i*n+j] + B[i*n+j] ) * 0.5;
    }
  }
}

/**
 * Multiply two matrices: C = A * B
 */
void multiply( Type* A, Type* B, Type* C ) {
  int i, j, k;
  int *s1, *s2, *s3;

  /* Initialize the result */
  memset( C, 0, sizeof(Type) * n * n );

  /* Define the order of the three subscripts */
  s1 = &i;
  s2 = &k;
  s3 = &j;

  /* Perform the multiplication */
  for ( *s1 = 0; *s1 < n; (*s1)++ ) {
    for ( *s2 = 0; *s2 < n; (*s2)++ ) {
      for ( *s3 = 0; *s3 < n; (*s3)++ ) {
        C[i*n+j] = C[i*n+j] + A[i*n+k] * B[k*n+j];
      }
    }
  }
}

/**
 * Small program to test matrix operations.
 */
int main( int argc, char* argv[] ) {
  Type* A;
  Type* B;
  Type* C;

  /* Allocate matrices */
  A = (Type*) malloc( n * n * sizeof(Type) );
  B = (Type*) malloc( n * n * sizeof(Type) );
  C = (Type*) malloc( n * n * sizeof(Type) );

  /* ... matrices are not initialized, bu we are not interested in real results
   * in this test... */


  /* Perform matrix product C = A * C */
  multiply( A, B, C);

  /* Perform matrix average C = (A + B)/2 */
  /* ... (many times, cyclically) ...*/
  int i;
  for ( i = 0; i < 100; i++ ) {
    average( A, B, C );
    average( B, C, A );
    average( C, A, B );
  }

  /* Free memory */
  free( A );
  free( B );
  free( C );

  return EXIT_SUCCESS;
}
