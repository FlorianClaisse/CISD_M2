/**
 * \author Luca Cirrottola (INRIA)
 * \date October 2022
 *
 * \warning "DO NOT SHOOT THE PIANIST!" -- This program is purposely bugged --
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Integer matrix datatype */
typedef struct {
  int nrow;
  int ncol;
  int** val;
} matrix_t;

/** Allocate an integer matrix */
void create( int nrow, int ncol, matrix_t* pmat ) {
  pmat->nrow = nrow;
  pmat->ncol = ncol; /* XXX bug: the number of columns was wrongly initialized (found with gdb) */
  pmat->val = (int**) malloc( pmat->nrow*sizeof(int*) );
  pmat->val[0] = (int*) calloc( pmat->nrow*pmat->ncol, sizeof(int) );
  for ( int i = 1; i < pmat->nrow; i++ )
    pmat->val[i] = pmat->val[i-1] + pmat->ncol;
}

/** Free an integer matrix */
void delete( matrix_t* pmat ) {
  free( pmat->val[0] ); /* XXX bug: the matrix was not freed (found with valgrind) */
  free( pmat->val );
}

/** Multiply two integer matrices as C = A*B */
void matmul( matrix_t* pA, matrix_t* pB, matrix_t* pC ) {
  for ( int r = 0; r < pC->nrow; r++ ) {
    for ( int c = 0; c < pC->ncol; c++ ) {
      for ( int k = 0; k < pA->ncol; k++ ) {
        pC->val[r][c] += pA->val[r][k] * pB->val[k][c];
      }
    }
  }
}

/** Print an integer matrix */
void print( matrix_t* pmat ) {
  printf("Matrix:\n");
  for ( int r = 0; r < pmat->nrow; r++ ) {
    for ( int c = 0; c < pmat->ncol; c++ ) {
      printf(" %3d", pmat->val[r][c] );
    }
    printf("\n");
  }
}

/**
 *  Small program to compute the product of two integer matrices.
 *  It is tested on the product of a row matrix
 *    A = [ 0 1 2 3 ... ]
 *
 *  and a column matrix
 *    B = [ 0 1 2 3 ... ]^T
 *
 *  so the result A = B*C is the multiplication table:
 *    C = [ 0 0 0 0 ...
 *          0 1 2 3 ...
 *          ...     ... ]
 */
int main( int argc, char* argv[] ) {
  matrix_t A, B, C;

  /* Allocate matrices */
  create( 10, 1, &A );
  create( 1,  5, &B );
  create( 10, 5, &C );

  /* Fill matrices */
  for ( int r = 0; r < A.nrow; r++ )
    for ( int c = 0; c < A.ncol; c++ )
      A.val[r][c] = r;

  for ( int r = 0; r < B.nrow; r++ )
    for ( int c = 0; c < B.ncol; c++ )
      B.val[r][c] = c;

  /* Multiply matrices */
  matmul( &A, &B, &C );

  /* Print */
  print( &C );

  /* Free matrices */
  delete( &A );
  delete( &B );
  delete( &C );

  return EXIT_SUCCESS;
}
