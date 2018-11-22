#include "features.h"

int _KEY_ = 1;

void affichierBloc(FICHIER *fichier, int bloc) {
    Buffer *buff = &((*fichier).buffR);
    viderBuffer(buff);
    lireDir(fichier, bloc, buff);
    separator();
    size_t t = strlen(buff->Record);
    if (t != 0) {
        char temp[MAX_BLOC_LENGTH + 1] = "";
        viderChaine(temp, MAX_BLOC_LENGTH + 1);
        strncat(temp, buff->Record, MAX_BLOC_LENGTH);
        strcat(temp, "\0");
        printf("le bloc (%d) contient : %s \n", bloc, temp);
        separator();
    } else {
        printf("le bloc (%d) est vide ", bloc);
    }
}

void afficherArticles(FICHIER *fichier, ZoneTompon *zone) {
    /// affichage de la zone tampon :
    int index = 0;
    char taille[4];
    char eff;
    char key[5];
    char article[MAX_ARTICLE_LENGTH];
    separator();
    printf("Element insere recement ------- \n");
    separator();
    printf("Cle---||---ETAT---||---Taille---||---Article\n");
    separator();
    while (index < zone->nbElement) {
        getNextRecordFromZone(zone, &index, taille, key, &eff, article);
        printf("%s---||---%c---||---%s---||---%s\n", key, eff, taille, article);
        separator();
    }
    /// Affichage des articles dans le fichier
    if (fichier->entete.numberArticles > 0) {
        int pos = 0;
        int bloc = 0;
        char tailleF[4];
        char effF;
        char keyF[5];
        char articleF[MAX_ARTICLE_LENGTH];
        separator();
        printf("Les Articles dans le fichier ----");
        separator();
        printf("Cle---||---ETAT---||---Taille---||---Article\n");
        separator();
        while (bloc < fichier->entete.numberBlocs) {
            getNextRecordInFile(fichier, &bloc, &pos, tailleF, &effF, keyF, articleF);
            printf("%s---||---%c---||---%s---||---%s\n", keyF, effF, tailleF, articleF);
            separator();
        }

    }

}
//bool ConvertEnteteToString()
int main() {

    char fileName[] = "data/read.txt";
    FICHIER file1;
    file1.filePtr = NULL;


    Ouvrir(fileName, &file1, _ANCIEN);
    int bloc = 0;
    int pos = 0;
    bool found = false;
    file1.entete.numberBlocs = 10;
    file1.entete.numberArticles = 2;
    file1.entete.numberBlocs = 1;
    ZoneTompon zone;
    Inserted elemt1;
    initInserted(&elemt1, 3, fileName, false, strlen(fileName));
    zone.nbElement = 1;
    zone.array[0] = elemt1;
    insert(&file1, 5, fileName, &zone);
    afficherArticles(&file1, &zone);

    Fermer(&file1);


    return 0;
}