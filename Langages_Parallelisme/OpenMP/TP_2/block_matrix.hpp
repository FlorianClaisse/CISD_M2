#include <assert.h>
#include <stdbool.h>
////////////////////////////////////////////////////
typedef struct block {
  int shape[2]; // the shape of the block
  value_type *coeff; // the coefficients
} block;

typedef struct tiled_matrix {
  int block_size;
  int global_shape[2] ;
  int shape[2];            //  the shape of the block matrix
  struct block *mat_block; //  a pointer on the block structure
} tiled_matrix;

// get the number of cols of the matrix
int nb_cols(tiled_matrix *Ablock) {
  int N_col = 0;
  for (int j = 0; j < Ablock->shape[1]; ++j) {
    N_col += Ablock->mat_block[j].shape[1];
  }
  return N_col;
};
// get the number of rows of the matrix
int nb_rows(tiled_matrix *Ablock) {
  int N_rows = 0;
  for (int j = 0; j < Ablock->shape[0]; ++j) {
    N_rows += Ablock->mat_block[Ablock->shape[0] * j].shape[1];
  }
  return N_rows;
};
void init(tiled_matrix *Ablock, int N, int BS) {

  int nb_block = N / BS;
  if (N % BS != 0) {
    printf(" NS doit diviser N !");
    exit(EXIT_FAILURE);
    // nb_block += 1;
  }
  // printf("nb_block: %d\n", nb_block);
  if (Ablock == NULL) {
    Ablock = malloc(sizeof(struct tiled_matrix));
  }
  Ablock->block_size = BS ;
  Ablock->global_shape[0] =  N;
  Ablock->global_shape[1] =  N;

  Ablock->shape[0] = nb_block;
  Ablock->shape[1] = nb_block;
  Ablock->mat_block = malloc(sizeof(block) * nb_block * nb_block);

  for (int i = 0; i < nb_block; ++i) {
    int size_block_i = BS;
    for (int j = 0; j < nb_block; ++j) {
      // printf("\n block %d  pos (%d, %d) \n", i * Ablock->shape[0] + j, pos_row,
      //        pos_col);
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      int size_block_j = BS;
      c_block->shape[0] = size_block_i;
      c_block->shape[1] = size_block_j;
      c_block->coeff = malloc(sizeof(value_type) * size_block_i * size_block_j);

    }
  }
}
// Convert a dense matrix to a tiled matrix
void to_tiled(int N, value_type *A, tiled_matrix *Ablock, int BS) {
  // printf(" N= %d BS= %d\n", N, BS);
  // printMat(N, A);
  int nb_block = N / BS;
  if (N % BS != 0) {
    printf(" NS doit diviser N ! res = ",N % BS);
    exit(EXIT_FAILURE);
  }
  // printf("nb_block: %d\n", nb_block);
  if (Ablock == NULL) {
    Ablock = malloc(sizeof(tiled_matrix));
  }
  Ablock->shape[0] = nb_block;
  Ablock->shape[1] = nb_block;
  Ablock->mat_block = malloc(sizeof(block) * nb_block * nb_block);

  int pos_row = 0;
  int pos_col = 0;
  for (int i = 0; i < nb_block; ++i) {
    int size_block_i = BS;
    for (int j = 0; j < nb_block; ++j) {
      // printf("\n bloc %d  pos (%d, %d) \n", i * Ablock->shape[0] + j, pos_row,
      //        pos_col);
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      int size_block_j = BS;

      c_block->shape[0] = size_block_i;
      c_block->shape[1] = size_block_j;
      c_block->coeff = malloc(sizeof(value_type) * size_block_i * size_block_j);
      // fill the block
      for (int ii = 0; ii < size_block_i; ++ii) {
        for (int jj = 0; jj < size_block_j; ++jj) {
          c_block->coeff[jj + ii * size_block_j] =
              A[(pos_row + ii) * N + pos_col + jj];
        }
      }
      pos_col += BS;
    }
    pos_col = 0;
    pos_row += BS;
  }
  // printf("Ablock: %x\n", Ablock);

  // printf("end to_tiled\n");
}
// void print_struct(struct tiled_matrix *Ablock) {
//   printf("Block matrix %d x %d  ptr %d \n", Ablock->shape[0], Ablock->shape[1],
//          Ablock->mat_block);
//   if (Ablock->mat_block == NULL) {
//     return;
//   }
//   // Ablock = malloc(sizeof(struct tiled_matrix));
//   int nb_block = 0;
//   for (int i = 0; i < Ablock->shape[0]; ++i) {
//     for (int j = 0; j < Ablock->shape[1]; ++j) {
//       struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);

//       printf("block %d  pos (%d, %d) size %d x %d ptr %x \n", nb_block, i, j,
//              c_block->shape[0], c_block->shape[1], c_block->coeff);
//       ++nb_block;
//     }
//   }
// }
void to_dense(struct tiled_matrix *Ablock, int N, int M, value_type *A) {
  // printf("inside dense_A: %x  size %d x %d\n", A, N,M);

  int nb_block = 0;
  int pos_row = 0;
  int size_block_i;
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    int pos_col = 0;
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      struct block *c_block = &Ablock->mat_block[i * Ablock->shape[1] + j];
      size_block_i = c_block->shape[0];
      int size_block_j = c_block->shape[1];
      // fill the block
      // printf("block %d loc (%d,%d) \n", nb_block, pos_row, pos_col);
      for (int ii = 0; ii < size_block_i; ++ii) {
        for (int jj = 0; jj < size_block_j; ++jj) {
          A[(pos_row + ii) * M + pos_col + jj] =
              c_block->coeff[jj + ii * size_block_j];
        }
      }
      pos_col += size_block_j;
      ++nb_block;
    }
    pos_row += size_block_i;
  }
  // printf("end to_dense\n");
}
void reset_tilted_matrix(struct tiled_matrix *Ablock) {
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
        c_block->coeff[k] = 0.0;
      }
    }
  }
}

// Compare deux matrices denses 
value_type compareMat(int N, value_type *A, value_type *B) {
  value_type err = 0.0;
  for (int i = 0; i < N * N; ++i) {
    err = fmax(fabsf(A[i] - B[i]), err);
  }
  return err;
}
// Compare deux ùatrices blocs
bool compare(struct tiled_matrix *Ablock, struct tiled_matrix *Bblock) {
  value_type tmp, err;
  err=0.0;
  int block_size = Ablock->block_size;
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      struct block *A_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      struct block *B_block = &(Bblock->mat_block[i * Bblock->shape[0] + j]);

      for (int k = 0; k < A_block->shape[0] * A_block->shape[1]; ++k) {
        tmp =A_block->coeff[k] - B_block->coeff[k] ;
        err += tmp*tmp;
      }
    }
  }
  return (err < 1.e-10); 
}
// copy A in B
void copy(struct tiled_matrix *Ablock, struct tiled_matrix *Bblock) {
    int N = Ablock->global_shape[0];
    int BS = Ablock->block_size ;
    init(Bblock,  N,  BS) ;

  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      struct block *A_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      struct block *B_block = &(Bblock->mat_block[i * Bblock->shape[0] + j]);
      for (int k = 0; k < A_block->shape[0] * A_block->shape[1]; ++k) {
        B_block->coeff[k] = A_block->coeff[k] ;
      }
    }
  }
}

void random_tilted_matrix(struct tiled_matrix *Ablock) {
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      printf("i %d, j %d block %d \n", i,j,i * Ablock->shape[0] + j);
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
        // c_block->coeff[k] =  rand() / (value_type)RAND_MAX;
                c_block->coeff[k] =  i * Ablock->shape[0] + j; //rand() / (value_type)RAND_MAX;

      }
    }
  }
}
void random_sym_tilted_matrix(struct tiled_matrix *Ablock, bool dist) {
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < i; ++j) {
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      struct block *c_block_sym = &(Ablock->mat_block[j * Ablock->shape[0] + i]);
      int block_size = c_block->shape[0];
      for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
        c_block->coeff[k] =  rand() / (value_type)RAND_MAX;
      }
      for (int k = 0; k < c_block->shape[1]; ++k) {
        for (int l = 0; l < c_block->shape[0] ; ++l) {
          c_block_sym->coeff[k+block_size*l] = c_block->coeff[l+block_size*k];
        }
      }
    }
    struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + i]);
    int block_size = c_block->shape[0];
    for (int k = 0; k < c_block->shape[1]; ++k) {
      if(dist)
      {
         c_block->coeff[k+block_size*k] = 0.0 ;
      }
      else
      {
            c_block->coeff[k+block_size*k] =   rand() / (value_type)RAND_MAX;
}      
for (int l = 0; l < k ; ++l) {
        c_block->coeff[l+block_size*k] =  rand() / (value_type)RAND_MAX;
        c_block->coeff[k+block_size*l] = c_block->coeff[l+block_size*k];
      }
    }
  }
}
void printMatptr(int N, value_type *A) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      printf(" %e ", A[i * N + j]);
    }
    printf("\n ");
  }
}
void printVecptr(int N, value_type *A) {
    for (int j = 0; j < N; ++j) {
      printf(" %e ", A[j]);
    }
    printf("\n ");
  
}
void print_tilted_matrix(struct tiled_matrix *Ablock) {
  int BS = Ablock->block_size ;
  for (int i = 0; i < Ablock->shape[0]; ++i) {
    for (int j = 0; j < Ablock->shape[1]; ++j) {
      printf(" block  %d,%d \n", i * BS,j*BS);
      struct block *c_block = &(Ablock->mat_block[i * Ablock->shape[0] + j]);
      printMatptr(c_block->shape[1], c_block->coeff) ;
    }
  }
}
