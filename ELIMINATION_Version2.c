#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 3
#define TAILLE (N*N)


typedef struct {
    int valeur;
    bool candidats[TAILLE + 1];
    int nbCandidats;
} tCase2;

typedef tCase2 tGrille[TAILLE][TAILLE];

int chargerGrille(tGrille grille, char *nomFichier);
bool possible(tGrille grille, int ligne, int colonne, int valeur);
void ajouterCandidat(tCase2 *lacase, int val, float *nbCandidatInitial);
void retirerCandidat(tCase2 *lacase, int val, float *nbCandidatElimine);
bool estCandidat(tCase2 lacase, int val);
int nbCandidat(tCase2 lacase);
void initialiserCandidat(tGrille grille, float *nbCandidatInitial);
void retirerLigneColCase(tGrille grille, int ligne, int colonne, int valeur, float *nbCandidatElimine);
void singletonnu(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies);
//void singletoncache(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies);
//void pairesnue(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine);
void afficheStats(float nbCaseRemplies, float nbCaseInitiales, float nbCandidatElimine, float nbCandidatInitial, char *nomFichier);

int main(){
    tGrille grille;
    bool progression;
    int nbCaseVides;
    float nbCaseInitiales, nbCandidatElimine, nbCaseRemplies, nbCandidatInitial;
    char nomFichier[30];
    nbCandidatElimine = 0;
    nbCaseRemplies = 0;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    nbCaseVides = chargerGrille(grille, nomFichier);
    nbCaseInitiales = nbCaseVides;
    initialiserCandidat(grille, &nbCandidatInitial);
    clock_t horaire = clock();
    progression = true;
    do
    {
        progression = false;
        singletonnu(grille, &nbCaseVides, &progression, &nbCandidatElimine, &nbCaseRemplies);
        //singletoncache(grille, &nbCaseVides, &progression, &nbCandidatElimine, &nbCaseRemplies);
        //pairesnue(grille, &nbCaseVides, &progression, &nbCandidatElimine);
    } while (nbCaseVides != 0 && progression);
    afficheStats(nbCaseRemplies, nbCaseInitiales, nbCandidatElimine, nbCandidatInitial, nomFichier);
    clock_t horaire2 = clock();
    double  tmpsCPU = (horaire2 - horaire)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps CPU = %.6f secondes\n",tmpsCPU);
    return EXIT_SUCCESS;
}

int chargerGrille(tGrille grille, char *nomFichier){
    int res = 0;
    int valcase;
    FILE * f;
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        for (int boucle1 = 0; boucle1 < TAILLE; boucle1++)
        {
            for (int boucle2 = 0; boucle2 < TAILLE; boucle2++)
            {
                fread(&valcase, sizeof(int), 1, f);
                grille[boucle1][boucle2].valeur = valcase;
                if(valcase == 0){
                    res++;
                }
            }
        }
    }
    fclose(f);

    return res;
}

bool possible(tGrille grille, int ligne, int colonne, int valeur){
    int i, j;
    bool res = true;
    int blocLigne = ligne / N * N;
    int blocColonne = colonne / N * N;

    for (i = 0; i < TAILLE; i++) {
        if (grille[ligne][i].valeur == valeur) {
            res = false;
        }

        if (grille[i][colonne].valeur == valeur) {
            res = false;
        }
    }

    for (i = blocLigne; i < blocLigne + 3; i++) {
        for (j = blocColonne; j < blocColonne + 3; j++) {
            if (grille[i][j].valeur == valeur && (i != ligne || j != colonne)) {
                res = false;
            }
        }
    }

    return res;
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

void singletonnu(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies){
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
                        (*nbCaseRemplies)++;
                        (*casevide)--;
                        retirerLigneColCase(grille, ligne, colonne, val, nbCandidatElimine);
                        *prog = true;
                        }
                    }
                }
            }
        } 
    }   
}

void singletoncache(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine, float *nbCaseRemplies){
    int ligne, colonne, val, compteurligne, compteurcol, compteurcase, i, j, blocLigne, blocColonne;
    for (ligne = 0; ligne < TAILLE; ligne++) {
        for (colonne = 0; colonne < TAILLE; colonne++) {
            for (val = 1; val < TAILLE+1; val++) {
                if (grille[ligne][colonne].valeur == 0 && estCandidat(grille[ligne][colonne], val))
                {
                    blocLigne = ligne / N * N;
                    blocColonne = colonne / N * N;
                    compteurligne = 0;
                    compteurcol = 0;
                    compteurcase = 0;

                    for (i = 0; i < TAILLE; i++)
                    {
                        if (grille[ligne][i].candidats[val])
                        {
                            compteurligne++;
                        }
                        if (grille[i][colonne].candidats[val])
                        {
                            compteurcol++;
                        }
                    }

                    for (i = blocLigne; i < blocLigne + 3; i++) {
                        for (j = blocColonne; j < blocColonne + 3; j++) {
                            if (grille[i][j].candidats[val] == true && (i != ligne || j != colonne)) {
                                compteurcase++;
                            }
                        }
                    }

                    if (compteurligne == 1 || compteurcol == 1 || compteurcase == 1)
                    {
                        grille[ligne][colonne].valeur = val;
                        (*nbCaseRemplies)++;
                        retirerLigneColCase(grille, ligne, colonne, val, nbCandidatElimine);
                    }

                }
            }
        }
    }
}

void pairesnue(tGrille grille, int *casevide, bool *prog, float *nbCandidatElimine){
    int ligne, colonne, blocLigne, blocColonne, val, val2, i, j, valeur1, valeur2;
    bool trouve;
    for (ligne = 0; ligne < TAILLE; ligne++)
    {
        for (colonne = 0; colonne < TAILLE; colonne++)
        {
            if (grille[ligne][colonne].valeur == 0)
            {
                if (grille[ligne][colonne].nbCandidats == 2)
                {
                    trouve = false;
                    for (int val = 1; val < TAILLE+1 && !trouve; ++val) {
                        if (grille[ligne][colonne].candidats[val]) {
                            valeur1 = val;
                            trouve = true;
                        }
                    }
                    trouve = false;
                    for (int val2 = valeur1 + 1; val2 < TAILLE+1 && !trouve; ++val2) {
                        if (grille[ligne][colonne].candidats[val2]) {
                            valeur2 = val2;
                            trouve = true;
                        }
                    }
                    blocLigne = ligne / N * N;
                    blocColonne = colonne / N * N;
                    for (i = blocLigne; i < blocLigne + N; i++) {
                        for (j = blocColonne; j < blocColonne + N; j++) {
                            if (grille[i][j].nbCandidats == 2)
                            {
                                if (grille[i][j].candidats[valeur1] == true && grille[i][j].candidats[valeur2] == true && (i != ligne || j != colonne)) {
                                    for (int x = blocLigne; x < blocLigne + N; x++) {
                                        for (int y = blocColonne; y < blocColonne + N; y++) {
                                            if (x != i || y != j) {
                                                if (x != ligne || y != colonne) {
                                                    if (grille[x][y].candidats[valeur1] == true) {
                                                        retirerCandidat(&grille[x][y], valeur1, nbCandidatElimine);
                                                        (*casevide)--;
                                                        *prog = true;
                                                    }
                                                    if (grille[x][y].candidats[valeur2] == true) {
                                                        retirerCandidat(&grille[x][y], valeur2, nbCandidatElimine);
                                                        (*casevide)--;
                                                        *prog = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
}

void afficheStats(float nbCaseRemplies, float nbCaseInitiales, float nbCandidatElimine, float nbCandidatInitial, char *nomFichier){
    float taux_remplissage;
    float taux_elimination;
    taux_remplissage = (nbCaseRemplies/nbCaseInitiales)*100;
    taux_elimination = (nbCandidatElimine/nbCandidatInitial)*100;
    printf("\t******  RESULTATS POUR %s ****** \n", nomFichier);
    printf("Nombre de cases remplies  = %.f sur %.f\t",nbCaseRemplies, nbCaseInitiales);
    printf("Taux de remplissage = %.3f %%\n",taux_remplissage);
    printf("Nombre de candidats éliminés : %.f \t",nbCandidatElimine);
    printf("Pourcentage d'élimination : %.3f %% \n",taux_elimination);   
}
