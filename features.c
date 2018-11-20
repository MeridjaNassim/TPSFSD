//
// Created by nassim on 20/11/2018.
//

#include "features.h"

bool initLoad(FICHIER *fichier, int *_KEY_) {
    printf("Chargement initiale du fichier : %s ...\n", fichier->entete.fileName); /// entete du chargement ;
    printf("Entrer le nombre d'articles a inserer ...\n"); /// demende du nombre d'articles
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
        printf("Entrer l'article %d...\n", articles); /// demende
        size_t articleSize = getArticle(art); /// recupérer l'article dans art de taille articleSize
        if (articleSize == 0) {
            continue;
        }
        char *record = buildString(getLength(articleSize), VALID, genKey(_KEY_),
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
    articles = articles - 1;
    aff_Entete(fichier, NB_ARTICLES, &articles);
    int charTotal = nbChars + (articles) * (4 + 3 +
                                            1); /// nombre de caractére d'articles + pour chaque article (taille + key +effacé)
    aff_Entete(fichier, NB_INSERTED, &charTotal);
    aff_Entete(fichier, LAST_KEY, _KEY_);
    return true;

}

bool Search(FICHIER *fichier, char *key, int *blocNum, int *indexInBloc, bool *found) {
    /// FONCTION utilisé pour réaliser une recherche d'un articles dans le fichier a partir de sa clé primaire
    /// returns : le num de bloc et la position dans le bloc du début de l'article si trouver , sinon ou on doit l'insérer ,
    int fileLastKey = fichier->entete.lastKey - 1; /// dernier clé du fichier
    int compare = strcmp(key, genKey(&fileLastKey)); /// on compare la clé chercher par la derniere clé du fichier
    if (compare < 0) {
        /// la clé est plus grande que la plus grande clé dans fichier ==> elle n'existe pas on insérera dans le dernier bloc
        int bloc = fichier->entete.numberBlocs; /// dernier bloc
        lireDir(fichier, bloc, &(*fichier).buffR);
        int position = (int) strlen((*fichier).buffR.Record); ///  la position d'insertion
        if (blocPlein((*fichier).buffR)) {
            bloc = bloc + 1;
            position = 0;
        }
        *blocNum = bloc;
        *indexInBloc = position;
    } else {
        /// clé est inférieure a la derniere clé
        /// Recherche séquentielle dans le fichier en tenant compte des chauvochement
        int position = 0;
        int currentBloc = 0;
        bool trouv = false;
        bool stop = false;
        Buffer *buff = &(*fichier).buffR; /// pointer vers le buffer read du fichier
        viderBuffer(buff); /// on vide le buffer
        lireDir(fichier, currentBloc, buff); /// lecture premier bloc
        while (!trouv && &stop) {
            char taille[3];
            viderChaine(taille, 3);
            ///Lecture de la taille
            for (int i = 0; i < 3; ++i) {
                taille[i] = buff->Record[position];
                position++;
                if (position >= MAX_BLOC_LENGTH) {
                    currentBloc++;
                    viderBuffer(buff);
                    lireDir(fichier, currentBloc, buff);
                    position = 0;
                }
            }
            /// lecture du effacé
            char effac = buff->Record[position];
            if (effac == 'D') {
                position = position + getTaille(taille, 0, 3) + 4; /// On saute car c'est supprimé
                if (position >= MAX_BLOC_LENGTH) {
                    currentBloc++;
                    viderBuffer(buff);
                    lireDir(fichier, currentBloc, buff);
                    position = position - MAX_BLOC_LENGTH;
                }
                continue;
            }
            position++;
            if (position >= MAX_BLOC_LENGTH) {
                currentBloc++;
                viderBuffer(buff);
                lireDir(fichier, currentBloc, buff);
                position = 0;
            }
            char keyS[5];
            viderChaine(keyS, 5);
            ///Lecture de la clé
            for (int i = 0; i < 4; ++i) {
                keyS[i] = buff->Record[position];
                position++;
                if (position >= MAX_BLOC_LENGTH) {
                    currentBloc++;
                    viderBuffer(buff);
                    lireDir(fichier, currentBloc, buff);
                    position = 0;
                }
            }
            if (strcmp(keyS, key) == 0) {
                /// clé trouvé
                trouv = true;
                *blocNum = currentBloc;
                *indexInBloc = position;
            } else {
                if (strcmp(keyS, key) > 0) {
                    /// clé n'existe pas car la clé courante est supérieure
                    trouv = false;
                    *blocNum = currentBloc; /// le bloc ou on doit inséréer
                    *indexInBloc = position - 8; /// l'indice ou on doit inséré
                    stop = true;
                } else {
                    position = position + getTaille(taille, 0, 3);/// on saute l'article courant
                    if (position >= MAX_BLOC_LENGTH) {
                        currentBloc++;
                        if (currentBloc <= fichier->entete.numberBlocs) { /// si on a encore des blocs dans le fichier
                            lireDir(fichier, currentBloc, buff);
                            position = position - MAX_BLOC_LENGTH; /// position de départ dans le nouveau fichier
                        } else {
                            *blocNum = currentBloc;
                            *indexInBloc = 0;
                            stop = true;
                        }
                    }
                }
            }
        }
        *found = trouv;

    }

    return true;

}