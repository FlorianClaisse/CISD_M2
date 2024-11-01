#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

// row-major layout
//
//  Pour compiler : gcc -fopenmp block_mds.c -o block_mds
// clang  -fopenmp -o block_mds block_mds.c
//  BS divise N
//
//////////////////////////////////////
typedef double value_type;

#include "block_matrix.hpp"

void pretraitement_MDS_seq(tiled_matrix *distance) {
    int N, BS;
    BS = distance->block_size;
    N = distance->global_shape[0];
    value_type *mean_row;
    mean_row = calloc(N, sizeof(value_type));

    //////////////////////////////////////////////////////////////////
    // Etape 1 : distance := distance .* distance
    for (int i = 0; i < distance->shape[0]; ++i) {
        for (int j = 0; j < distance->shape[1]; ++j) {
            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
        
            for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
                c_block->coeff[k] = c_block->coeff[k] * c_block->coeff[k];
            }
        }
    }

    //////////////////////////////////////////////////////////////////
    // Etape 2 : calcule des moyennes
    int row, col;
    row = 0;
    col = 0;
    for (int i = 0; i < distance->shape[0]; ++i) {
        for (int j = 0; j < distance->shape[1]; ++j) {

            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
            BS = c_block->shape[0];
            col = j * BS;
            row = i * BS;
            for (int k = 0; k < c_block->shape[1]; ++k) {
                for (int l = 0; l < c_block->shape[0]; ++l) {
                    mean_row[row + k] += c_block->coeff[k + BS * l];
                    // mean_col[col+l] += c_block->coeff[k+BS*l];
                }
            }
        }
    }
    
    //   printf("\n");
    // printVecptr(N,mean_row);
    value_type d = 0;
    for (int i = 0; i < N; ++i) {
        d += mean_row[i];
        mean_row[i] /= (value_type)N;
    }
    d /= (value_type)(N * N);

    //////////////////////////////////////////////////////////////////
    // Etape 3 : calcul la matrice de Gram par Double centrage
    for (int i = 0; i < distance->shape[0]; ++i) {
        row = i * BS;
        for (int j = 0; j < distance->shape[1]; ++j) {
            col = j * BS;
            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
            for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
                c_block->coeff[k] = c_block->coeff[k];
            }

            for (int k = 0; k < c_block->shape[1]; ++k) {
                for (int l = 0; l < c_block->shape[0]; ++l) {
                    c_block->coeff[k + BS * l] = -0.5 * (c_block->coeff[k + BS * l] + d - mean_row[row + k] - mean_row[col + l]);
                }
            }
        }
    }
}


void pretraitement_MDS_task(tiled_matrix *distance) {
    //
    // A paralléliser avec omp task
    //
    int N, BS;
    BS = distance->block_size;
    N = distance->global_shape[0];
    value_type *mean_row;
    mean_row = calloc(N, sizeof(value_type));

    //////////////////////////////////////////////////////////////////
    // Etape 1 : distance := distance .* distance
    for (int i = 0; i < distance->shape[0]; ++i) {
        for (int j = 0; j < distance->shape[1]; ++j) {
            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
            for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
                c_block->coeff[k] = c_block->coeff[k] * c_block->coeff[k];
            }
        }
    }

    //////////////////////////////////////////////////////////////////
    // Etape 2 : calcule des moyennes
    int row, col;
    row = 0;
    col = 0;
    for (int i = 0; i < distance->shape[0]; ++i) {
        for (int j = 0; j < distance->shape[1]; ++j) {

            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
            BS = c_block->shape[0];
            col = j * BS;
            row = i * BS;
            for (int k = 0; k < c_block->shape[1]; ++k) {
                for (int l = 0; l < c_block->shape[0]; ++l) {
                    mean_row[row + k] += c_block->coeff[k + BS * l];
                    // mean_col[col+l] += c_block->coeff[k+BS*l];
                }
            }
        }
    }

    //   printf("\n");
    // printVecptr(N,mean_row);
    value_type d = 0;
    for (int i = 0; i < N; ++i) {
        d += mean_row[i];
        mean_row[i] /= (value_type)N;
    }
    d /= (value_type)(N * N);

    //////////////////////////////////////////////////////////////////
    // Etape 3 : calcul la matrice de Gram par Double centrage
    for (int i = 0; i < distance->shape[0]; ++i) {
        row = i * BS;
        for (int j = 0; j < distance->shape[1]; ++j) {
            col = j * BS;
            struct block *c_block = &(distance->mat_block[i * distance->shape[0] + j]);
            for (int k = 0; k < c_block->shape[0] * c_block->shape[1]; ++k) {
                c_block->coeff[k] = c_block->coeff[k];
            }

            for (int k = 0; k < c_block->shape[1]; ++k) {
                for (int l = 0; l < c_block->shape[0]; ++l) {
                    c_block->coeff[k + BS * l] = -0.5 * (c_block->coeff[k + BS * l] + d - mean_row[row + k] - mean_row[col + l]);
                }
            }
        }
    }
}

int main() {
    const int N = 10;
    const int BS = 5;
    const bool dist = true;
    ///
    tiled_matrix distance = {0, 0, 0, 0, 0, NULL};
    tiled_matrix dist_par = {0, 0, 0, 0, 0, NULL};
    // initialisation
    init(&distance, N, BS);
    // remplissage de la matrice de distance
    random_sym_tilted_matrix(&distance, 1);
    copy(&distance, &dist_par);

    //
    clock_t start, stop;
    double elapsed, dstop, dstart;
    //

    start = clock();
    pretraitement_MDS_seq(&distance);
    stop = clock();
    elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("(block) Seq Time elapsed in ms: %f\n", elapsed);

    start = clock();
    pretraitement_MDS_task(&dist_par);
    stop = clock();
    elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("(block) par Time elapsed in ms: %f\n", elapsed);

    printf("Result: %s\n", compare(&distance, &dist_par) ? "true" : "false");
}
