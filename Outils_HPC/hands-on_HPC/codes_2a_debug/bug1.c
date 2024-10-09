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

  /* Fill array */
  for ( int i = 0; i <= mysize; i++ )
    array[i] = i;

  /* Print values */
  printf("Values:");
  for ( int i = 0; i < mysize; i++ )
    printf(" %d", array[i]);
  printf("\n");

  return EXIT_SUCCESS;
}
