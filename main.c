#include "machineAbstraite.h"


int _KEY_ = 1;

bool initLoad(FICHIER *fichier) {
    printf("Chargement initiale du fichier : %s ...\n", fichier->entete.fileName); /// entete du chargement ;
    printf("Entrer le nombre d'articles a inserer ...\n"); /// demende du nombre d'articles
    int key = _KEY_;
    int num_bloc = 0;   /// le nombre de blocs dans le fichier
    int nbArticle = 0; ///le nombre d'articles a insérer
    int articles; /// le nombre d'articles à insérer
    int nbChars = 0; /// le nombre de charactere d'articles inséréer
    scanf("%d", &nbArticle);
    bool buffPlein = false; /// indicateur de buffer plein pour l'ecriture
    bool chauvochement = false; /// indicateur de chauvochement

    viderBuffer(&((*fichier).buffW));  /// initialisé le buffer d'ecriture a vide

    /// Insertion initiale des articles
    for (articles = 0; articles <= nbArticle; ++articles) {
        char art[MAX_ARTICLE_LENGTH]; /// chaine de lecture a partir du clavier
        viderChaine(art, MAX_ARTICLE_LENGTH);
        printf("Entrer l'article ...\n"); /// demende
        size_t articleSize = getArticle(art); /// recupérer l'article dans art de taille articleSize
        if (articleSize == 0) {
            continue;
        }
        char *record = buildString(getLength(articleSize), VALID, genKey(&key),
                                   art); /// générer l'enregistrement avec la taille le champ effacé et la clé dans record
        nbChars = nbChars + (int) articleSize; /// on incrémente les chars
        /// Si le buffer n'a pas d'espace pour ecrire tous l'enregistrement , on chauvoche sur un autre bloc
        if (!addToBuffer(&((*fichier).buffW), record)) {
            /// si le chauvochement est bien dérouler on continue
            if (chauvocherBuffer(&((*fichier).buffW), record)) {
                buffPlein = true;
                chauvochement = true;
            }
        } else {
            if (strlen((*fichier).buffW.Record) == MAX_BLOC_LENGTH) { /// le buffer est plein mais pas de chauvochement
                buffPlein = true;
            }
        }

        while (chauvochement) {
            if (buffPlein == true) {
                EcrireDir(fichier, num_bloc, &((*fichier).buffW));
                viderBuffer(&((*fichier).buffW));
                buffPlein = false;
                num_bloc++;
            }
            if (!addToBuffer(&((*fichier).buffW), record)) {
                /// si le chauvochement est bien dérouler on continue
                if (chauvocherBuffer(&((*fichier).buffW), record)) {
                    buffPlein = true;
                    chauvochement = true;
                }
            } else {
                if (strlen((*fichier).buffW.Record) ==
                    MAX_BLOC_LENGTH) { /// le buffer est plein mais pas de chauvochement
                    buffPlein = true;
                } else {
                    chauvochement = false;
                }
            }

        }
    }
    /// si le buffer n'est pas plein on ecrit ce qui reste
    if (strlen((*fichier).buffW.Record) != 0 || buffPlein) {
        EcrireDir(fichier, num_bloc, &((*fichier).buffW));
        viderBuffer(&((*fichier).buffW));
    }
    /// On met à jour l'entete du fichier ;
    aff_Entete(fichier, NB_BLOCS, &num_bloc);
    aff_Entete(fichier, NB_ARTICLES, &articles);
    int charTotal = nbChars + (articles) * (4 + 3 +
                                            1); /// nombre de caractére d'articles + pour chaque article (taille + key +effacé)
    aff_Entete(fichier, NB_INSERTED, &charTotal);
    return true;

}

//bool ConvertEnteteToString()
int main() {

    char fileName[] = "data/read.txt";
    FICHIER file1;
    file1.filePtr = NULL;


    Ouvrir(fileName, &file1, _NOUVEAU);

    initLoad(&file1);

    Fermer(&file1);
    return 0;
}