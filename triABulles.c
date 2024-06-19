#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
/*******************************************************************
*
* @brief programme de TRI A BULLES sur un tableau de 100 000 entiers
* initialis� avec des valeurs al�atoires
*
*******************************************************************/
#define TAILLE 100000

typedef int typeTab[TAILLE];

void triABulles(typeTab tab);
void remplirTableau(typeTab tab);

int main () {
    typeTab leTableau;
    srand(time(NULL));
    remplirTableau(leTableau);
    //time_t horaire = time(NULL);
    clock_t horaire = clock();
    triABulles(leTableau);
    //time_t horaire2 = time(NULL);
    clock_t horaire2 = clock();
    //printf("Le temps du trie est de %lf\n",difftime(horaire2,horaire));
    double  tmpsCPU = (horaire2 - horaire)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps CPU = %.3f secondes\n",tmpsCPU);
    return EXIT_SUCCESS;
}

void triABulles(typeTab t){
    int i, j;
    int tmp;
    
    for(i=0;i<TAILLE-1;i++){
        for(j=0;j<TAILLE-i-1;j++){
            if (t[j]>t[j+1]){
                tmp = t[j];
                t[j] = t[j+1];
                t[j+1] = tmp;
            }
        }
    }
}

void remplirTableau(typeTab tab){
    int i;
    for (i=0 ; i<TAILLE ; i++){
        tab[i] = rand();
    }
}
