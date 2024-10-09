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

/** Allocate an array */
void create( int size, int** arr ) {
  *arr = (int*) malloc( size*sizeof(int) );
}

/** Copy an old array into a new one */
void copy( int size, int* old, int** new ) {
  int* tmp;
  create( size, new );
  memcpy( *new, old, size*sizeof(int) );
  free(old);
}

/**
 * Small program to copy an old array into a new one.
 * Print their difference entry by entry.
 */
int main( int argc, char* argv[] ) {
  const int mysize = 100;
  int* oldarray;
  int* newarray;

  /* Allocate first array */
  create( mysize, &oldarray );

  /* Fill array */
  /* XXX bug: i should be less than mysize (found with valgrind, invalid write) */
  for ( int i = 0; i < mysize; i++ )
    oldarray[i] = i;

  /* Copy array */
  /* XXX bug: the copy was not made on newarray (which was not allocated) but on
   * a temporary array inside the function (found with gdb or valgrind) */
  copy( mysize, oldarray, &newarray );

  /* Print difference between the two arrays */
  printf("Difference:");
  /* XXX bug: i should be less than mysize (found with valgrind, invalid write) */
  for ( int i = 0; i < mysize; i++ ) {
    int err = newarray[i];
    err -= oldarray[i];
    printf(" %d", err);
  }
  printf("\n");

  /* XXX possible bug: newarray needs to be freed (found with valgrind, definitely lost bytes) */
  free( newarray );

  return EXIT_SUCCESS;
}
