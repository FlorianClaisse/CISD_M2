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

/**
 * Small program to create an integer array, fill it and print it.
 */
int main( int argc, char* argv[] ) {
  const int mysize = 100;
  int* array = NULL;

  /* XXX First bug: memory was not allocated (found with gdb: segmentation fault) */
  array = (int*) malloc( mysize*sizeof(int) );

  /* Fill array */
  /* XXX Second bug: index should be less than mysize (found with valgrind: invalid write) */
  for ( int i = 0; i < mysize; i++ )
    array[i] = i;

  /* Print values */
  printf("Values:");
  for ( int i = 0; i < mysize; i++ )
    printf(" %d", array[i]);
  printf("\n");

  /* XXX Possible third bug: memory should be freed (found with valgrind: definitely lost bytes) */
  free( array );

  return EXIT_SUCCESS;
}
