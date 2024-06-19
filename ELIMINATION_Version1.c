#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Constantes
#define N 3
#define TAILLE N*N

typedef struct tCase1{ 
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

// Procédure et fonctions
int chargerGrille(tGrille grille, char *nom_fichier);
int initialiserCandidats(tGrille grille);
bool estCandidat(tCase1 laCase, int val);
int nbCandidat(tCase1 laCase) ;
void ajouterCandidat(tCase1 *laCase, int val);
void retirerCandidat(tCase1 *laCase, int val);
void afficheStats(int nbCaseRemplies, int nbCaseVidesInitiales, int nbCandidatElimine, int nbCandidatInitial, char *nomFichier );
bool possible(tGrille grille, int numLigne, int numColonne, int val);
void singletonNu(tGrille grille, int *nbCasesVides, int *nbCandidatElimine, bool *progression, int *nbCandidatInitial);

// Programme principal
int main(){
    tGrille grille;
    bool progression;
    int nbCasesVides;
    char nom_fichier[30];
    int nbCaseVidesInitiales, nbCaseRemplies, nbCandidatInitial;
    int nbCandidatElimine = 0;
    printf("Nom du fichier ? \n");
    scanf("%s", nom_fichier);
    nbCasesVides = chargerGrille(grille, nom_fichier);
    nbCaseVidesInitiales = nbCasesVides;
    nbCandidatInitial = initialiserCandidats(grille);
    clock_t horaire = clock();
    progression = true;
    while((nbCasesVides != 0) && (progression == true)){  
        progression = false;
        singletonNu(grille, &nbCasesVides, &nbCandidatElimine, &progression, &nbCandidatInitial);
    }

    nbCaseRemplies = nbCaseVidesInitiales - nbCasesVides;
    afficheStats(nbCaseRemplies, nbCaseVidesInitiales, nbCandidatElimine, nbCandidatInitial , nom_fichier);
    clock_t horaire2 = clock();
    double  tmpsCPU = (horaire2 - horaire)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps CPU = %.7f secondes\n",tmpsCPU);
    return EXIT_SUCCESS;
}


int chargerGrille(tGrille grille, char *nom_fichier){
    int nbCasesVides = 0;
    int i;
    int j;
    FILE * f ;
    f = fopen(nom_fichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nom_fichier);
    }
    for (i=0; i < TAILLE; i++){
        for (j=0; j<TAILLE; j++){
            fread(&grille[i][j].valeur, sizeof(int), 1, f);
            if (grille[i][j].valeur == 0){
                nbCasesVides ++;
            }
        }
    }
    fclose(f);
    return nbCasesVides;
}

int initialiserCandidats(tGrille grille){
    int i;
    int j;
    int val ;
    int nbCandidatTotal = 0;
    for(i = 0;i<TAILLE;i++){
        for(j = 0; j<TAILLE;j++){
            if(grille[i][j].valeur == 0){
                grille[i][j].nbCandidats = 0;
                for(val=1;val<=TAILLE;val++){
                    if(possible(grille,i,j, val) == true){
                        ajouterCandidat(&grille[i][j], val);
                        nbCandidatTotal ++;
                    }   
                }
            }
        }
    }
    return nbCandidatTotal; 
}

bool possible(tGrille grille, int numLigne, int numColonne, int val){
    bool resultat = true;
    int i,j;
    for(i=0;i<TAILLE;i++){
        if(val == grille[numLigne][i].valeur){
            resultat = false;
        }
    }
    for(j=0;j<TAILLE;j++){
        if (val==grille[j][numColonne].valeur){
            resultat = false;
        }
    }
    int x=(numLigne/N) * N;
    int y=(numColonne/N) * N;
    i=x;
    while (resultat && i<x+N){
        j=y;
        while (!resultat && j<y+N){
            if (grille[i][j].valeur==val){
                resultat = false;
            } else {
                j++;
            }
        }
        i++;
    }    
    return resultat;
}

void retirerCandidat(tCase1 *laCase, int val){
    bool valSuppr = false ;
    int compt =0;
    while((valSuppr!=true) && (compt<laCase->nbCandidats)){  
        if (val == laCase->candidats[compt]){
            laCase->candidats[compt] = laCase->candidats[laCase->nbCandidats -1];
            laCase->nbCandidats = laCase->nbCandidats - 1 ;  
        }  
    }
    valSuppr = true;
}

bool estCandidat(tCase1 laCase, int val){
    bool resultat;
    bool valEstCandidat;
    int compt;
    int limiteCase;
    resultat = false;
    valEstCandidat = false;
    compt = 0;
    limiteCase = nbCandidat(laCase);
    while((compt< limiteCase)&& (valEstCandidat != true)){
        if(val == laCase.candidats[compt]){
            resultat = true;
        }
        else{
            compt = compt+1;
        }
    }
    return resultat;
}

void ajouterCandidat(tCase1 *laCase, int val){
    if(nbCandidat(*laCase) < TAILLE){
        laCase->candidats[laCase->nbCandidats] = val;
        laCase->nbCandidats = laCase->nbCandidats + 1 ;
    }
}

int nbCandidat(tCase1 laCase){
    return laCase.nbCandidats ;
}

void afficheStats(int nbCaseRemplies, int nbCaseVidesInitiales, int nbCandidatElimine, int nbCandidatInitial, char *nomFichier ){
    float taux_remplissage;
    float taux_elimination;
    taux_remplissage = (nbCaseRemplies/nbCaseVidesInitiales)*100;
    taux_elimination = (nbCandidatElimine/nbCandidatInitial)*100;
    printf("******  RESULTATS POUR %s  ****** \n", nomFichier);
    printf("Nombres de cases remplies  = %d sur %d\t",nbCaseRemplies, nbCaseVidesInitiales);
    printf("Taux de remplissage = %.3f\n",taux_remplissage); 
    printf("Nombre total de candidat éliminé : %d\t",nbCandidatElimine);
    printf("Pourcentage d'élimination : %.3f\n",taux_elimination);   
}

void singletonNu(tGrille grille, int *nbCasesVides, int *nbCandidatElimine, bool *progression, int *nbCandidatInitial){
    int j,i;
    for(i=0;i<TAILLE;i++){
        for(j=0;j<TAILLE;j++){
            if(grille[i][j].valeur == 0){
                if(nbCandidat(grille[i][j]) == 1){
                    grille[i][j].valeur = grille[i][j].candidats[0];
                    // retirerCandidat(grille[i][j], grille[i][j].candidats[0]);
                    *nbCasesVides = *nbCasesVides -1 ;
                    *nbCandidatElimine = *nbCandidatInitial - initialiserCandidats(grille);
                    *progression = true;
                }
            }
        }
    }
}