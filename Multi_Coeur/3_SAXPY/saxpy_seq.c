/*
 * Application saxpy: y = A.x + y
 * Calcul sur le CPU
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define A 3.

///////////////////////////////////////////////////////////
//                    Fonction y=ax + y                  //
///////////////////////////////////////////////////////////
void saxpy(float *tabX, float *tabY, int SIZE) {
    int k;
    for (k = 0; k < SIZE; k++)   
        tabY[k]= A * tabX[k] + tabY[k];
}
///////////////////////////////////////////////////////////
//               Initialisation des vecteurs             //
///////////////////////////////////////////////////////////
void initab(float *tab, float val, int SIZE) {
    int k;
    for (k = 0; k < SIZE; k++)     
        tab[k]= val;
}
///////////////////////////////////////////////////////////
//                  Affichage des vecteurs               //
///////////////////////////////////////////////////////////
void affitab(char *chaine,float * ab, int SIZE) {
    int k;
    printf("\nLes 16 premiers de %s: \n",chaine);
    for (k = 0; k < 16; k++) 
        printf("%.2f ",tab[k]);

    printf("\nLes 16 derniers: \n");

    for (k = SIZE - 16; k < SIZE; k++) 
        printf("%.2f ",tab[k]);
    
    printf("\n");
}
//////////////////////////////////////////////////////////////////
//                      Program main                            //
//////////////////////////////////////////////////////////////////
int main( int argc, char** argv)  {
    int nbelements;
    float *tablox;
    float *tabloy;

    if (argc < 2) {
        fprintf(stderr, "usage:  %s nbelements \n", argv[0]);
       return EXIT_FAILURE;
    }
    
    sscanf(argv[1],"%d",&nbelements);
    printf("Debut du programme avec %d elements \n",nbelements);

    /***************************************/
    tablox=(float *) malloc(sizeof(float) * nbelements);
    initab(tablox, 5., nbelements);

    tabloy=(float *) malloc(sizeof(float) * nbelements);
    initab(tabloy, 7., nbelements);
    
    affitab("tabloy avant",tabloy, nbelements);

    /*****    Appel de la fonction    *****/
    saxpy(tablox, tabloy, nbelements);
    /**************************************/
    affitab("tabloy apres",tabloy, nbelements);  
    
    free(tablox);
    free(tabloy);
    
    printf("Fin du programme\n");
    
    return EXIT_SUCCESS;
}


