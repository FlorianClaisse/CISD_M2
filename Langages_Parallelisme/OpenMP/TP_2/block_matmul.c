#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
//
//  Pour compiler : gcc -o block_matmult block_matmult.c  -fopenmp -O3
//  BS divise N
//
//////////////////////////////////////
typedef double value_type;
#include "block_matrix.hpp"


void block_matmul_seq(tiled_matrix *Abloc, tiled_matrix *Bbloc,
                      tiled_matrix *Cbloc) {

  int nb_row_block = Abloc->shape[0];
  int nb_col_block = Abloc->shape[1];

  for (int i = 0; i < nb_row_block; ++i) {
    for (int j = 0; j < nb_col_block; ++j) {
      struct block *c_block = &Cbloc->mat_block[i * Cbloc->shape[1] + j];

      for (int k = 0; k < nb_col_block; ++k) {
        struct block *a_block = &Abloc->mat_block[i * Abloc->shape[1] + k];
        struct block *b_block = &Bbloc->mat_block[k * Bbloc->shape[1] + j];

        for (int ii = 0; ii < c_block->shape[0]; ++ii) {
          for (int jj = 0; jj < c_block->shape[1]; ++jj) {
            value_type tmp = 0.0;
            // 
            for (int kk = 0; kk < a_block->shape[1]; ++kk) {
              tmp += a_block->coeff[ii * a_block->shape[1] + kk] *
                     b_block->coeff[kk * b_block->shape[1] + jj];
            }
            c_block->coeff[ii * c_block->shape[1] + jj] += tmp;
          }
        }
      }
    }
  }
}
void block_matmul_FJ(tiled_matrix *Abloc, tiled_matrix *Bbloc,
                     tiled_matrix *Cbloc) {

  int nb_row_block = Abloc->shape[0];
  int nb_col_block = Abloc->shape[1];
  printf("Todo block_matmul_FJ\n");

}


void bloc_matmul_depend(tiled_matrix *Abloc, tiled_matrix *Bbloc,
                        tiled_matrix *Cbloc) {
  int nb_row_block = Abloc->shape[0];
  int nb_col_block = Abloc->shape[1];
  printf("Todo bloc_matmul_depend\n");
}



int main() {
  const int N = 1000;
  const int BS = 100;
      printf("N= %d\nBS= %d\n\n", N, BS);
  /// 
  value_type *A, *B, *C;
  A = malloc(sizeof(value_type) * N * N);
  B = malloc(sizeof(value_type) * N * N);
  C = malloc(sizeof(value_type) * N * N);

  srand(time(NULL));
  for (int i = 0; i < N * N; ++i) {
    A[i] = rand() / (double)RAND_MAX;
    B[i] = rand() / (double)RAND_MAX;
    C[i] = 0.0;
  }
  tiled_matrix Abloc = {0, 0, 0, 0, 0, NULL};;
  tiled_matrix Bbloc = {0, 0, 0, 0, 0, NULL};;
  tiled_matrix Cbloc = {0, 0, 0, 0, 0, NULL};;
  tiled_matrix Dbloc = {0, 0, 0, 0, 0, NULL};;
  to_tiled(N, A, &Abloc, BS);
  to_tiled(N, B, &Bbloc, BS);
  to_tiled(N, C, &Cbloc, BS);
  to_tiled(N, C, &Dbloc, BS);

  //
  clock_t start, stop;
  double elapsed, dstop, dstart;
  //
  // for cache
  block_matmul_seq(&Abloc, &Bbloc, &Cbloc);
  reset_tilted_matrix(&Cbloc);
  // Start computation
  start = clock();
  block_matmul_seq(&Abloc, &Bbloc, &Cbloc);
  stop = clock();
  elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("(bloc) Seq Time elapsed in ms: %f\n", elapsed);

  value_type *dense_C = malloc(sizeof(value_type) * N * N);
  to_dense(&Cbloc, N, N, dense_C); //

  // printMatptr( N, dense_C);
  // printf("dense \n");

  //  printMatptr( N, C);
  //  for (int i =0; i < N*N; ++i){
  //   D[i] = dense_C[i] - C[i];
  //  }
  //  printMatptr( N, D);

  //
  // reset_tilted_matrix(&Cbloc);

  /////////////////////////////////////
  // Fork join
  start = clock();
  block_matmul_FJ(&Abloc, &Bbloc, &Dbloc);
  stop = clock();
  elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("FJ Time elapsed in ms: %f\n", elapsed);
  to_dense(&Dbloc, N, N, C); //

  value_type err = compareMat(N, dense_C, C);
  printf("Error: %f \n", err);

  reset_tilted_matrix(&Cbloc);
  start = clock();
  bloc_matmul_depend(&Abloc, &Bbloc, &Cbloc); 
  stop = clock();
  elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
  printf("tasks depend Time elapsed in ms: %f\n", elapsed);
  to_dense(&Dbloc, N, N, C); 
  // 
  
  err = compareMat(N, dense_C, C);
  printf("Error: %f \n", err);
}
