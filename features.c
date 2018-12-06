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
    Buffer *buffer = &((*fichier).buffW);
    viderBuffer(buffer);  /// initialisé le buffer d'ecriture a vide

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
        while (!addToBuffer(buffer, record)) {
            chauvocherBuffer(buffer, record);
            EcrireDir(fichier, num_bloc, buffer);
            num_bloc++;
            viderBuffer(buffer);
        }

    }
    /// si le buffer n'est pas plein on ecrit ce qui reste
    if (strlen(buffer->Record) != 0) {
        EcrireDir(fichier, num_bloc, buffer);
        viderBuffer(buffer);
    }
    /// On met à jour l'entete du fichier ;
    num_bloc = num_bloc + 1;
    aff_Entete(fichier, NB_BLOCS, &num_bloc);
    bool modif = true;
    aff_Entete(fichier, MODIFIED, &modif);
    articles = articles - 1;
    aff_Entete(fichier, NB_ARTICLES, &articles);
    int charTotal = nbChars + (articles) * (4 + 3 +
                                            1); /// nombre de caractére d'articles + pour chaque article (taille + key +effacé)
    aff_Entete(fichier, NB_INSERTED, &charTotal);
    aff_Entete(fichier, LAST_KEY, _KEY_);
    return true;

}

bool
Search(FICHIER *fichier, char *key, int *blocNum, int *indexInBloc, bool *found, ZoneTompon *zone, bool *foundZone) {
    /// FONCTION utilisé pour réaliser une recherche d'un articles dans le fichier a partir de sa clé primaire
    /// returns : le num de bloc et la position dans le bloc du début de l'article si trouver , sinon ou on doit l'insérer ,
    /// On recherche d'abord dans la zone Tempon :
    searchZone(zone, getKey(key, 0, 4), foundZone,
               (size_t *) indexInBloc); /// si la  clé se trouve dans la zone tompon alors on ne cherche pas dans le fichier(recherche dans zone est dichotomique)
    if ((*foundZone)) {
        return true;
    }
    ///-----------------------------------------
    /// RECHERCHE DANS LE FICHIER -----------------------
    int fileLastKey = fichier->entete.lastKey - 1; /// dernier clé du fichier
    int compare = strcmp(key, genKey(&fileLastKey)); /// on compare la clé chercher par la derniere clé du fichier
    if (compare > 0) {
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
        while (!trouv && !stop) {
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
                position = position + getTaille(taille, 0, 3) + 5; /// On saute car c'est supprimé
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

bool delete(FICHIER *fichier, char *key, ZoneTompon *zone) {
    /// fonction qui supprime logiquement un enregistrement dans le fichier a partir de sa clé
    int bloc = 0;
    int pos = 0;
    bool found = false;
    bool foundZone = false;
    Search(fichier, key, &bloc, &pos,
           &found, zone,
           &foundZone); /// on recherche la clé dans le fichier ==> si trouvé alors on a le bloc et la position dans le bloc
    if (foundZone) {
        zone->array[pos].deleted = true;
        return true;
    }
    if (found) {
        Buffer *buffer = &((*fichier).buffR);
        viderBuffer(buffer); /// on vide le buffer pour ne pas avoir des erreurs
        if (pos >=
            5) { /// si la position est supérieur a 5 donc le char effécé est dans le meme bloc à la position pos -5
            lireDir(fichier, bloc, buffer);
            buffer->Record[pos - 5] = 'D'; /// on met a jour le char effacé
            EcrireDir(fichier, bloc, buffer);
        } else { /// sinon le char effacé se trouve dans le bloc précédant à la position(5-pos)
            lireDir(fichier, bloc - 1, buffer);
            buffer->Record[MAX_BLOC_LENGTH - (5 - pos)] = 'D';
            EcrireDir(fichier, bloc - 1, buffer);
        }
        fichier->entete.modified = true; /// fichier modifié
        fichier->entete.numberDeleted++; /// nombre de supprimés
        return true; /// suppression avec succée;

    }
    return false; /// on a pas trouvé la clé donc on supprime rien
}

bool insert(FICHIER *fichier, int key, char *article, ZoneTompon *zone, char *newPotentielFile) {
    /// insertion dans un fichier d'article a partir de sa clé
    bool insererDansZone = false;
    if (zone->nbElement < MAX_ZONE_TEMPON) {
        /// on peut insérer dans la zone tempon
        bool found = false;
        size_t pos = 0;
        searchZone(zone, key, &found, &pos);
        if (found) {
            zone->array[pos].deleted = false; /// on réactive le champ si c'est effacé
        } else {
            /// on insére un nouveau element dans la zone dans la position pos pour qu'elle soit trié
            Inserted newElement;
            size_t size = strlen(article); /// taille d'article
            if (initInserted(&newElement, key, article, false, size)) { /// initialisation de l'element
                decalerZone(zone, 1, (int) pos); /// décalage  des element supérieure dans la zone par une position
                zone->array[pos] = newElement; /// on insére
                insererDansZone = true;
                zone->nbElement++;
            } else {
                insererDansZone = false;
            }

        }
    } else {
        insererDansZone = false;
    }
    if (!insererDansZone) {
        /// si on a pas put insérer dans la zone on réorganise le fichier est la zone courante dans un nouveau
        char fileName[MAX_FILE_NAME];
        reorganiser(fichier, zone, true, fileName);
        strcpy(newPotentielFile, fileName);
        insert(fichier, key, article, zone, fileName); /// appel récursif pour réinsérer la donnee
    }
    return true;
}

void renameFile(FICHIER *fichier, char *newName) {
    if (newName != NULL) {
        rename(genEnteteName(fichier->entete.fileName), genEnteteName(newName));
        rename(fichier->entete.fileName, newName);
        strcpy(fichier->entete.fileName, newName);
        EcrireEntete(fichier);
    }

}

void randomLoadingOfFile(FICHIER *fichier, int dataSize) {
    /// fonction pour générér des articles aléatoirement et les insérés dans le fichier
    int key = -1; /// la clé primaire des articles
    int num_bloc = 0;   /// le nombre de blocs dans le fichier
    int nbArticle = 0; ///le nombre d'articles a insérer
    int nbChars = 0; /// le nombre de charactere d'articles inséréer
    Buffer *buffer = &((*fichier).buffW); /// ptr vers notre buffer
    viderBuffer(buffer); /// on le vide pour ne pas avoir des erreurs
    for (int i = 0; i < dataSize; ++i) {
        int r = rand() % 1000; /// on génére un taille aléatoire comprise entre 0 et 999
        if (r == 0) {
            /// si la taille aléatoire est nul on la met a 10
            r = 10;
        }
        char *randomStr = randString(r); /// on génére un article aléatoire
        char *record = buildString(getLength((size_t) r), VALID, genKey(&key),
                                   randomStr); /// générer l'enregistrement avec la taille le champ effacé et la clé dans record
        nbArticle++; /// inc le nombre d'articles
        nbChars = nbChars + (int) r; /// on incrémente les chars
        /// Si le buffer n'a pas d'espace pour ecrire tous l'enregistrement , on chauvoche sur un autre bloc
        while (!addToBuffer(buffer, record)) {
            chauvocherBuffer(buffer, record);
            EcrireDir(fichier, num_bloc, buffer);
            num_bloc++;
            viderBuffer(buffer);
        }

    }
    /// si le buffer n'est pas plein on ecrit ce qui reste
    if (strlen(buffer->Record) != 0) {
        EcrireDir(fichier, num_bloc, buffer);
        viderBuffer(buffer);
    }
/// On met à jour l'entete du fichier ;
    num_bloc = num_bloc + 1;
    aff_Entete(fichier, NB_BLOCS, &num_bloc);
    bool modif = true;
    aff_Entete(fichier, MODIFIED, &modif);
    aff_Entete(fichier, NB_ARTICLES, &nbArticle);
    int charTotal = nbChars + (nbArticle) * (4 + 3 +
                                             1); /// nombre de caractére d'articles + pour chaque article (taille + key +effacé)
    aff_Entete(fichier, NB_INSERTED, &charTotal);
    aff_Entete(fichier, LAST_KEY, &key);
    EcrireEntete(fichier);
}

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
        bool stop;
        while (bloc < fichier->entete.numberBlocs && !stop) {
            getNextRecordInFile(fichier, &bloc, &pos, tailleF, &effF, keyF, articleF);
            if (strlen(tailleF) == 0) {
                stop = true;
            }
            printf("%s---||---%c---||---%s---||---%s\n", keyF, effF, tailleF, articleF);
            separator();
        }

    }

}