#include <stdio.h>
#include <stdlib.h>
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

void chargerGrille(tGrille grille);
bool possible(tGrille grille, int ligne, int colonne, int valeur);
void ajouterCandidat(tCase2 *lacase, int val);
void retirerCandidat(tCase2 *lacase, int val);
bool estCandidat(tCase2 lacase, int val);
int nbCandidat(tCase2 lacase);
void initialiserCandidat(tGrille grille);
void retirerLigneColCase(tGrille grille, int ligne, int colonne, int valeur);
void singletonnu(tGrille grille, bool *progression);
void singletoncache(tGrille grille, bool *progression);
bool backtracking(tGrille grille, int numeroCase);
void pairesnue(tGrille grille, bool *progression);

int main(){
    tGrille grille;
    bool progression;
    char nomFichier[30];
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    chargerGrille(grille);
    afficherGrille(grille);
    initialiserCandidat(grille);
    afficherGrille(grille);
    clock_t debut = clock();
    progression = true;
    do
    {
        progression = false;
        singletonnu(grille, &progression);
        singletoncache(grille, &progression);
        pairesnue(grille, &progression);
    } while (nbCaseVides != 0 && progression);
    backtracking(grille, 0);
    afficherGrille(grille);
    clock_t fin = clock();
    double tmpsCPU = (fin - debut)*1.0 / CLOCKS_PER_SEC;
    printf( "Temps résolution de grille : %.6f secondes\n",tmpsCPU);
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

void ajouterCandidat(tCase2 *lacase, int val){
    (*lacase).candidats[val] = true;
    (*lacase).nbCandidats++;
}

void retirerCandidat(tCase2 *lacase, int val){
    (*lacase).candidats[val] = false;
    (*lacase).nbCandidats--;
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

void initialiserCandidat(tGrille grille){
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
                        ajouterCandidat(&grille[ligne][colonne], valeur);
                    }
                    
                }
            }
        }
    }
}

void retirerLigneColCase(tGrille grille, int ligne, int colonne, int valeur){
    int i, j;
    int blocLigne = ligne / N * N;
    int blocColonne = colonne / N * N;

    for (i = 0; i < TAILLE; i++) {
        if (grille[ligne][i].candidats[valeur]) {
            retirerCandidat(&grille[ligne][i], valeur);
        }

        if (grille[i][colonne].candidats[valeur]) {
            retirerCandidat(&grille[i][colonne], valeur);
        }
    }

    for (i = blocLigne; i < blocLigne + N; i++) {
        for (j = blocColonne; j < blocColonne + N; j++) {
            if (grille[i][j].candidats[valeur] && (i != ligne || j != colonne)) {
                retirerCandidat(&grille[i][j], valeur);
            }
        }
    }
}

void singletonnu(tGrille grille, bool *progression){
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
                        retirerLigneColCase(grille, ligne, colonne, val);
                        *progression = true;
                        }
                    }
                }
            }
        } 
    }   
}

void singletoncache(tGrille grille, bool *progression){
    int ligne, colonne, val, compteurligne, compteurcol, compteurcase, i, j, blocLigne, blocColonne;
    for (ligne = 0; ligne < TAILLE; ligne++){
        for (colonne = 0; colonne < TAILLE; colonne++){
            for (val = 1; val < TAILLE+1; val++){
                if (grille[ligne][colonne].valeur == 0 && estCandidat(grille[ligne][colonne], val)){
                    blocLigne = ligne / N * N;
                    blocColonne = colonne / N * N;
                    compteurligne = 0;
                    compteurcol = 0;
                    compteurcase = 0;
                    for (i = 0; i < TAILLE; i++){
                        if (grille[ligne][i].candidats[val]){
                            compteurligne++;
                        }
                        if (grille[i][colonne].candidats[val]){
                            compteurcol++;
                        }
                    }
                    for (i = blocLigne; i < blocLigne + 3; i++){
                        for (j = blocColonne; j < blocColonne + 3; j++){
                            if (grille[i][j].candidats[val] == true && (i != ligne || j != colonne)){
                                compteurcase++;
                            }
                        }
                    }

                    if (compteurligne == 1 || compteurcol == 1 || compteurcase == 1){
                        grille[ligne][colonne].valeur = val;
                        retirerLigneColCase(grille, ligne, colonne, val);
                    }

                }
            }
        }
    }
}

void pairesnue(tGrille grille, bool *progression){
    int ligne, colonne, blocLigne, blocColonne, i, j, valeur1, valeur2;
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
                                                        retirerCandidat(&grille[x][y], valeur1);
                                                        *progression = true;
                                                    }
                                                    if (grille[x][y].candidats[valeur2] == true) {
                                                        retirerCandidat(&grille[x][y], valeur2);
                                                        *progression = true;
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

bool backtracking(tGrille grille, int numeroCase){
    int ligne, colonne;
    bool resultat = false;
    if(numeroCase == TAILLE*TAILLE){
        resultat = true;
    }
    else {
        ligne = numeroCase / TAILLE ; 
        colonne = numeroCase % TAILLE ; 
        if((grille[ligne][colonne].valeur) != 0){
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
                        grille[ligne][colonne].valeur = 0;
                    }
                }

            }
        }   
    }
    return resultat;
} 

