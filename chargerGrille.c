/*

void chargerGrille(tGrille g){
    char nomFichier[30];
    FILE * f;
    printf("Nom du fichier ? \n");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), TAILLE*TAILLE, f);
        fclose(f);
    }
}

*/

int chargerGrille(g){

    
    int nbCasesVides = 0;
    char nomFichier[30];
    int i;
    int j;

    printf("Nom du fichier ? \n");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");

    for (i=0; i < TAILLE; i++){
        for (j=0; j<TAILLE; j++){

                g[i][j][0] = fread(g, sizeof(int), 1, f);
                if (fread(g, sizeof(int), 1, f) == 0){
                    nbCasesVides ++;
                }
        }
    }

    return nbCasesVides;

}