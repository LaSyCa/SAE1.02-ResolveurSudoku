#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> 

#define N 4
#define TAILLE (N*N) 

typedef struct {
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
} tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE];

typedef int tGrille [TAILLE][TAILLE];

void chargerGrille(tGrille grille);
void afficherGrille(tGrille grille);
bool possible(tGrille grille, int numLigne, int numColonne, int val);
bool backtracking(tGrille grille, int numeroCase);

void ajouterCandidat(tCase2 *lacase, int val, float *nbCandidatInitial);
void retirerCandidat(tCase2 *lacase, int val, float *nbCandidatElimine);
bool estCandidat(tCase2 lacase, int val);
int nbCandidat(tCase2 lacase);
void initialiserCandidat(tGrille grille, float *nbCandidatInitial);
void retirerLigneColCase(tGrille grille, int ligne, int colonne, int valeur, float *nbCandidatElimine);
void singletonnu(tGrille grille, bool *progression);
//void singletoncache(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies);
//void pairesnue(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine);

int main(){
    tGrille grille;
    bool progression = true;
    nbCaseVides = chargerGrille(grille, nomFichier);
    afficherGrille(grille);
    initialiserCandidat(grille, &nbCandidatInitial);
    afficherGrille(grille);
    clock_t debut = clock();
    singletonnu(grille, &progression);
    backtracking(grille, 0);
    clock_t fin = clock();
    afficherGrille(grille);
    double tmpsCPU = (fin - debut)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps résolution de grille : s = %.6f secondes\n",tmpsCPU);
    return EXIT_SUCCESS;
}


void chargerGrille(tGrille g){
 char nomFichier[30];
 FILE * f;
 printf("Nom du fichier ? ");
 scanf("%s", nomFichier);
 f = fopen(nomFichier, "rb");
 if (f==NULL){
    printf("\n ERREUR sur le fichier %s\n", nomFichier);
 } 
 else {
    fread(g, sizeof(int), TAILLE*TAILLE, f);
    fclose(f);
 }

}

void afficherGrille(tGrille grille){

    int i;
    int j;
    int compteur_chiffre = 0;
    int compteur_carre = 0;
    int compteur_ligne = 0;

    printf("+------------+------------+------------+------------+\n");

    for (i=0; i<TAILLE; i++){
        printf("|");
        compteur_ligne++;

        for (j=0; j<TAILLE; j++){

            if (grille[i][j] == 0){

                printf(" . ");
            } else {
                printf("%3d", grille[i][j]);
            }           
            compteur_chiffre++;
            if (compteur_chiffre == N){
                printf("|");
                compteur_chiffre = 0;
                compteur_carre++;
                if (compteur_carre == N){
                    printf("\n");
                    compteur_carre=0;
                }
            }
        }
        if (compteur_ligne == N){
            printf("+------------+------------+------------+------------+\n");
            compteur_ligne =0;
        }
    }

}


bool possible(tGrille grille, int numLigne, int numColonne, int val){

    /**  DÃ©clarations des variables */ 
    bool resultat = true;
    int i,j;

    /** Traitement des donnÃ©es */ 
    for(i=0;i<TAILLE;i++){
        if(val == grille[numLigne][i]){

            resultat = false;
            break;
        }
    }
    
    for(j=0;j<TAILLE;j++){
        if (val==grille[j][numColonne]){

            resultat = false;
            break;
        }
    }

    int x=(numLigne/N) * N;
    int y=(numColonne/N) * N;
    i=x;
    while (resultat && i<x+N){
        j=y;
        while (resultat && j<y+N){
            if (grille[i][j]==val){
                resultat = false;
            } else {
                j++;
            }
        }
        i++;
    }    
    return resultat;
}


bool backtracking(tGrille grille, int numeroCase){
    int ligne, colonne;
    bool resultat = false;
    if(numeroCase == TAILLE*TAILLE){
        resultat = true;
    }
    else {
        ligne = numeroCase / TAILLE ; 
        colonne = numeroCase % TAILLE ; 
        if((grille[ligne][colonne]) != 0){
            resultat = backtracking(grille, numeroCase+1);
        }
        else{
            for(int i = 1; i<TAILLE+1; i++){
                if(possible(grille, ligne, colonne, i)==true){
                    
                    grille[ligne][colonne] = i;
                    if(backtracking(grille,numeroCase+1)== true){
        
                        resultat = true;
                    }
                    else{
                        grille[ligne][colonne] = 0;
                    }
                }

            }
        }   
    }
    return resultat;
} 

void ajouterCandidat(tCase2 *lacase, int val, float *nbCandidatInitial){
    (*lacase).candidats[val] = true;
    (*lacase).nbCandidats++;
    (*nbCandidatInitial)++;
}

void retirerCandidat(tCase2 *lacase, int val, float *nbCandidatElimine){
    (*lacase).candidats[val] = false;
    (*lacase).nbCandidats--;
    (*nbCandidatElimine)++;
}

bool estCandidat(tCase2 lacase, int val){
    bool res = false;
    if (lacase.candidats[val] == true)
    {
        res = true;
    }
    return res;
}

int nbCandidat(tCase2 lacase){
    int res = lacase.nbCandidats;
    return res;
}

void initialiserCandidat(tGrille grille, float *nbCandidatInitial){
    int ligne, colonne, valeur;
    for (ligne = 0; ligne < TAILLE; ligne++)
    {
        for (colonne = 0; colonne < TAILLE; colonne++)
        {
            for (valeur = 1; valeur < TAILLE+1; valeur++) {
                grille[ligne][colonne].candidats[valeur] = false;
            }
            grille[ligne][colonne].nbCandidats = 0;

            if (grille[ligne][colonne].valeur == 0)
            {
                for (valeur = 1; valeur < TAILLE+1; valeur++)
                {
                    if (possible(grille, ligne, colonne, valeur) == true)
                    {
                        ajouterCandidat(&grille[ligne][colonne], valeur, nbCandidatInitial);
                    }
                    
                }
            }
        }
    }
}

void retirerLigneColCase(tGrille grille, int ligne, int colonne, int valeur, float *nbCandidatElimine){
    int i, j;
    int blocLigne = ligne / N * N;
    int blocColonne = colonne / N * N;

    for (i = 0; i < TAILLE; i++) {
        if (grille[ligne][i].candidats[valeur]) {
            retirerCandidat(&grille[ligne][i], valeur, nbCandidatElimine);
        }

        if (grille[i][colonne].candidats[valeur]) {
            retirerCandidat(&grille[i][colonne], valeur, nbCandidatElimine);
        }
    }

    for (i = blocLigne; i < blocLigne + N; i++) {
        for (j = blocColonne; j < blocColonne + N; j++) {
            if (grille[i][j].candidats[valeur] && (i != ligne || j != colonne)) {
                retirerCandidat(&grille[i][j], valeur, nbCandidatElimine);
            }
        }
    }
}

void singletonnu(tGrille grille, bool *prog){
    int ligne, colonne, val;
    for (ligne = 0; ligne < TAILLE; ligne++)
    {
        for (colonne = 0; colonne < TAILLE; colonne++)
        {
            if (grille[ligne][colonne].valeur == 0)
            {
                if (grille[ligne][colonne].nbCandidats == 1)
                {
                    for (val = 1; val  <= TAILLE; val++)
                    {
                        if (grille[ligne][colonne].candidats[val] == true)
                        {
                        grille[ligne][colonne].valeur = val;
                        retirerLigneColCase(grille, ligne, colonne, val, nbCandidatElimine);
                        *prog = true;
                        }
                    }
                }
            }
        } 
    }   
}

// void singletoncache(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies){
//     int ligne, colonne, val, compteurligne, compteurcol, compteurcase, i, j, blocLigne, blocColonne;
//     for (ligne = 0; ligne < TAILLE; ligne++) {
//         for (colonne = 0; colonne < TAILLE; colonne++) {
//             for (val = 1; val < TAILLE+1; val++) {
//                 if (grille[ligne][colonne].valeur == 0 && estCandidat(grille[ligne][colonne], val))
//                 {
//                     blocLigne = ligne / N * N;
//                     blocColonne = colonne / N * N;
//                     compteurligne = 0;
//                     compteurcol = 0;
//                     compteurcase = 0;

//                     for (i = 0; i < TAILLE; i++)
//                     {
//                         if (grille[ligne][i].candidats[val])
//                         {
//                             compteurligne++;
//                         }
//                         if (grille[i][colonne].candidats[val])
//                         {
//                             compteurcol++;
//                         }
//                     }

//                     for (i = blocLigne; i < blocLigne + 3; i++) {
//                         for (j = blocColonne; j < blocColonne + 3; j++) {
//                             if (grille[i][j].candidats[val] == true && (i != ligne || j != colonne)) {
//                                 compteurcase++;
//                             }
//                         }
//                     }

//                     if (compteurligne == 1 || compteurcol == 1 || compteurcase == 1)
//                     {
//                         grille[ligne][colonne].valeur = val;
//                         (*nbCaseRemplies)++;
//                         retirerLigneColCase(grille, ligne, colonne, val, nbCandidatElimine);
//                     }

//                 }
//             }
//         }
//     }
// }

// void pairesnue(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine){
//     int ligne, colonne, blocLigne, blocColonne, val, val2, i, j, valeur1, valeur2;
//     bool trouve;
//     for (ligne = 0; ligne < TAILLE; ligne++)
//     {
//         for (colonne = 0; colonne < TAILLE; colonne++)
//         {
//             if (grille[ligne][colonne].valeur == 0)
//             {
//                 if (grille[ligne][colonne].nbCandidats == 2)
//                 {
//                     trouve = false;
//                     for (int val = 1; val < TAILLE+1 && !trouve; ++val) {
//                         if (grille[ligne][colonne].candidats[val]) {
//                             valeur1 = val;
//                             trouve = true;
//                         }
//                     }
//                     trouve = false;
//                     for (int val2 = valeur1 + 1; val2 < TAILLE+1 && !trouve; ++val2) {
//                         if (grille[ligne][colonne].candidats[val2]) {
//                             valeur2 = val2;
//                             trouve = true;
//                         }
//                     }
//                     blocLigne = ligne / N * N;
//                     blocColonne = colonne / N * N;
//                     for (i = blocLigne; i < blocLigne + N; i++) {
//                         for (j = blocColonne; j < blocColonne + N; j++) {
//                             if (grille[i][j].nbCandidats == 2)
//                             {
//                                 if (grille[i][j].candidats[valeur1] == true && grille[i][j].candidats[valeur2] == true && (i != ligne || j != colonne)) {
//                                     for (int x = blocLigne; x < blocLigne + N; x++) {
//                                         for (int y = blocColonne; y < blocColonne + N; y++) {
//                                             if (x != i || y != j) {
//                                                 if (x != ligne || y != colonne) {
//                                                     if (grille[x][y].candidats[valeur1] == true) {
//                                                         retirerCandidat(&grille[x][y], valeur1, nbCandidatElimine);
//                                                         (*casevide)--;
//                                                         *prog = true;
//                                                     }
//                                                     if (grille[x][y].candidats[valeur2] == true) {
//                                                         retirerCandidat(&grille[x][y], valeur2, nbCandidatElimine);
//                                                         (*casevide)--;
//                                                         *prog = true;
//                                                     }
//                                                 }
//                                             }
//                                         }
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     } 
// }
