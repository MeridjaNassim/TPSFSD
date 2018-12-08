//
// Created by nassim on 22/11/2018.
//

#include "Gestion.h"


bool reorganiserFichier(FICHIER *fichier, char *newName) {
    /// Fonction utilisé pour réorganiser un fichier de donnes uniquement sans modifier la zone tompon
    FICHIER newFichier;
    /// un nouveau nom pour ce fichier
    strcpy(newName, genReorgFileName(fichier->entete.fileName));
    Ouvrir(newName, &newFichier,
           _NOUVEAU); /// on ouvre ce fichier on mode NOUVEAU (l'entete est généré automatiquement)
    int currentBloc = 0;

    int pos = 0;
    /// Variable de fichier :
    char tailleF[4];
    char keyF[5];
    char effF;
    char articleF[MAX_ARTICLE_LENGTH];
    ///---------------------
    /// Variables du nouveau fichier ----
    int currentNewBloc = 0;
    int posNew = 0;
    int nbBlocs = 0;
    int nbChars = 0;
    int nbArticles = 0;
    int lastKey = 0;
    //----------------------------------
    while (currentBloc < fichier->entete.numberBlocs) {
        getNextRecordInFile(fichier, &currentBloc, &pos, tailleF, &effF, keyF, articleF);
        if (effF == 'V') {
            putRecordFile(&newFichier, &currentNewBloc, &posNew, tailleF, keyF, &effF, articleF);
            nbArticles++;
            nbChars = nbChars + (getTaille(tailleF, 0, 3) + 1 + 4 + 3);
        }
    }

    nbBlocs = currentNewBloc + 1;

    newFichier.entete.numberBlocs = nbBlocs;
    newFichier.entete.numberInserted = nbChars;
    newFichier.entete.numberArticles = nbArticles;
    newFichier.entete.lastKey = getKey(keyF, 0, 4);
    newFichier.entete.numberDeleted = 0;
    newFichier.entete.modified = true;
    EcrireEntete(&newFichier);
    Fermer(&newFichier);
    return true;

}

bool fusionner(FICHIER *fichier, FICHIER *newFichier, ZoneTompon *zone) {
    /// fonction qui fusionne un fichier avec une zone d'insertion dans un nouveau fichier
    int currentBloc = 0; /// bloc initial du fichier primaire
    int currentNewBloc = 0; /// bloc initial du nouveau fichier
    int index = 0; /// index initial de la zone
    int pos = 0;/// position initial dans fichier
    int newPos = 0; /// posititon initial dans le nouveau fichier
    bool getFromFile = true;
    bool getFromZone = true;
    bool currentInserted = false;
    int nbChar = 0; /// nombre de characteres insérés dans le new fichier
    int lastKey = 0; /// la derniere clé dans le new fichier
    int nbArticles = 0; /// le nombre d'articles dans le new fichier
    /// Variable de fichier :
    char tailleF[4];
    char keyF[5];
    char effF;
    char articleF[MAX_ARTICLE_LENGTH];
    ///---------------------
    /// Variable de Zone :
    char tailleZ[4];
    char keyZ[5];
    char effZ;
    char articleZ[MAX_ARTICLE_LENGTH];
    ///---------------------
    /// Tant que on est pas a la fin du fichier ni la fin de la zone
    while ((currentBloc <= fichier->entete.numberBlocs && index < zone->nbElement) || !currentInserted) {
        if (!currentInserted) {
            if (getFromFile) {
                /// On prends le prochain record depuis fichier
                getNextRecordInFile(fichier, &currentBloc, &pos, tailleF, &effF, keyF, articleF);
            }
            if (getFromZone) {
                /// On prends le prochaine recorrd depuis zone
                getNextRecordFromZone(zone, &index, tailleZ, keyZ, &effZ, articleZ);
            }
        }
        if (effF == 'D' && effZ == 'V') {
            /// si le record du fichier est supprimé et selui de la zone ne l'est pas
//            putRecordFile(newFichier, &currentNewBloc, &newPos, tailleZ, keyZ, &effZ, articleZ);
            getFromFile = true;
            getFromZone = false;
            currentInserted = false;
        }
        if (effF == 'V' && effZ == 'D') {
            /// si le record du zone est supprimé et selui de fichier ne l'est pas
//            putRecordFile(newFichier, &currentNewBloc, &newPos, tailleF, keyF, &effF, articleF);
            getFromFile = false;
            getFromZone = true;
            currentInserted = false;
        }
        if (effF == 'D' && effZ == 'D') {
            /// si l'est deux son supprimé
            getFromFile = true;
            getFromZone = true;
        }
        if (effF == 'V' && effZ == 'V') {
            /// si les deux sont valables Alors on compare les clé
            int compare = strcmp(keyF, keyZ);
            if (compare < 0) {
                /// si clé du fichier est < a clé de Zone on ecrit celle du fichier
                putRecordFile(newFichier, &currentNewBloc, &newPos, tailleF, keyF, &effF, articleF);
                nbArticles++;
                nbChar = nbChar + getTaille(tailleF, 0, 3) + 1 + 3 + 4;
                lastKey = getKey(keyZ, 0, 4);
                if (!getFromFile && !getFromZone) {
                    currentInserted = true;
                }
                getFromFile = true; /// on récupére un nouveau record de fichier
                getFromZone = false; /// on a pas inséré le record courant
            } else if (compare > 0) {
                /// si clé de zone est < a clé de fichier on ecrit celle de zone
                putRecordFile(newFichier, &currentNewBloc, &newPos, tailleZ, keyZ, &effZ, articleZ);
                nbArticles++;
                nbChar = nbChar + getTaille(tailleZ, 0, 3) + 1 + 3 + 4;
                lastKey = getKey(keyF, 0, 4);
                currentInserted = false;
                if (index < zone->nbElement) {
                    getFromZone = true; /// on recupére un nouveau record de zone
                } else {
                    viderChaine(tailleZ, 4);
                    strcpy(keyZ, keyF);
                    viderChaine(articleZ, MAX_ARTICLE_LENGTH);
                    effZ = 'V';
                }
                getFromFile = false; /// on a pas encore inséré celui du fichier
            } else {
                /// si egal ==> on autorise pas les dupliqué alors on met celui de fichier .
                putRecordFile(newFichier, &currentNewBloc, &newPos, tailleF, keyF, &effF, articleF);
                nbArticles++;
                nbChar = nbChar + getTaille(tailleF, 0, 3) + 1 + 3 + 4;
                lastKey = getKey(keyF, 0, 4);
                if (!getFromFile && !getFromZone) {
                    currentInserted = true;
                }
                getFromFile = true; ///on récupére un nouveau record de fichier
                getFromZone = true;/// on recupére un nouveau record de zone
            }
        }

    }
    /// tant que on a encore des blocs dans le fichier on ecrit tous les records qu'il contiennent
    while (currentBloc < fichier->entete.numberBlocs) {
        getNextRecordInFile(fichier, &currentBloc, &pos, tailleF, &effF, keyF, articleF);
        putRecordFile(newFichier, &currentNewBloc, &newPos, tailleF, keyF, &effF, articleF);
        nbArticles++;
        nbChar = nbChar + getTaille(tailleF, 0, 3) + 1 + 3 + 4;
        lastKey = getKey(keyF, 0, 4);
    }
    /// Tant que on a encore des records non ecrit dans la zone on les ecrit dans le fichier
    while (index < zone->nbElement) {
        getNextRecordFromZone(zone, &index, tailleZ, keyZ, &effZ, articleZ);
        putRecordFile(newFichier, &currentNewBloc, &newPos, tailleZ, keyZ, &effZ, articleZ);
        nbArticles++;
        nbChar = nbChar + getTaille(tailleZ, 0, 3) + 1 + 3 + 4;
        lastKey = getKey(keyZ, 0, 4);
    }
    zone->nbElement = 0;
    int lastbloc = currentNewBloc;
    initEntete(newFichier, newFichier->entete.fileName, lastbloc, nbArticles, 0, nbChar, false, lastKey);
    EcrireEntete(newFichier);
}

bool reorganiser(FICHIER *fichier, ZoneTompon *zone, bool useZone, char *newFileName) {
    /// si la zone est utilisé est elle n'est pas a NULL
    int nbChars;
    int nbArticles;
    int nbBlocs;
    int lastKey;
    if (useZone && zone != NULL) {
        int smallestZoneKey = zone->array[0].key; /// on récupére la premiere (plus petite)clé de la zone
        if (smallestZoneKey > fichier->entete.lastKey) {
            /// la plus petite clé de la zone est > a la plus grande du fichier ==> les element de la zone sont inséré en dernier
            reorganiser(fichier, NULL, false, newFileName); /// on reorganise d'abord le  fichier seul
            Buffer *buff = &((*fichier).buffR);
            int b = fichier->entete.numberBlocs == 0 ? 0 : fichier->entete.numberBlocs - 1;
            lireDir(fichier, b, buff); /// on lit le dernier bloc du fichier réorganisé ;
            nbChars = fichier->entete.numberInserted;
            nbArticles = fichier->entete.numberArticles;
            nbBlocs = fichier->entete.numberBlocs;
            if (blocPlein(*buff)) { /// si le dernier bloc est plein
                fichier->entete.numberBlocs++; /// on ecrit dans un nouveau bloc qu'on ajoute au fichier
                int currentBloc = fichier->entete.numberBlocs - 1;
                viderBuffer(buff);
                /// pour chaque element dans la zone
                for (int i = 0; i < zone->nbElement; ++i) {
                    Inserted elt = zone->array[i]; /// on recupére l'element
                    if (!elt.deleted) { /// si il n'est supprimé de la zone
                        lastKey = elt.key;
                        int keyM = elt.key - 1;
                        char *keyI = genKey(&keyM); /// on génére sa clé
                        char *record = buildString(getLength(elt.articleSize), VALID, keyI,
                                                   elt.article); /// on construit son record a ecrire
                        nbArticles++;
                        nbChars = nbChars + (int) strlen(record);
                        while (!addToBuffer(buff, record)) {
                            /// tant que record contient encore des char on chauvauche sur le bloc courant est on l'ecrit et on vide le buffer
                            chauvocherBuffer(buff, record);
                            EcrireDir(fichier, currentBloc, buff);
                            viderBuffer(buff);
                            currentBloc++;
                            nbBlocs++;
                        }
                        if (strlen(buff->Record) != 0) {
                            EcrireDir(fichier, currentBloc, buff);
                            viderBuffer(buff);
                            currentBloc++;
                        }
                        /// insertion du record courant est fini
                    }
                }
                zone->nbElement = 0;
            } else {
                /// dernier bloc non plein
                int currentBloc =
                        fichier->entete.numberBlocs - 1; /// on continue a ecrire a partir de ce bloc qui est le dernier
                for (int i = 0; i < zone->nbElement; ++i) {
                    Inserted elt = zone->array[i];
                    if (!elt.deleted) {
                        lastKey = elt.key;
                        int keyM = elt.key - 1;
                        char *keyI = genKey(&keyM);
                        char *record = buildString(getLength(elt.articleSize), VALID, keyI,
                                                   elt.article);/// génération du record
                        nbArticles++;
                        nbChars = nbChars + (int) strlen(record);
                        /// l'ecriture du record sur le fichier
                        while (!addToBuffer(buff, record)) {
                            chauvocherBuffer(buff, record);
                            EcrireDir(fichier, currentBloc, buff);
                            viderBuffer(buff);
                            currentBloc++;
                            nbBlocs++;
                        }
                        if (strlen(buff->Record) != 0 && i == zone->nbElement - 1) { /// dernier element
                            EcrireDir(fichier, currentBloc, buff);
                            viderBuffer(buff);
                            currentBloc++;
                            nbBlocs++;
                        }
                    }
                }
                zone->nbElement = 0; /// on a plus de records inséré dans zone on peut alors l'utilisé

            }
            strcpy(fichier->entete.fileName, newFileName);
            fichier->entete.numberBlocs = nbBlocs;
            fichier->entete.numberArticles = nbArticles;
            fichier->entete.numberInserted = nbChars;
            fichier->entete.lastKey = lastKey;
            fichier->entete.modified = true;
            Fermer(fichier);
        } else {
            /// si la plus grande clé dans fichier est inférieure a la plus petite clé dans zone
            static FICHIER newFichier; /// le fichier reorganisé a retourné
            char *newName = genReorgFileName(fichier->entete.fileName); /// un nouveau nom pour ce fichier
//            strcpy(newName, "New"); /// on rajoute New a debut du fichier
//            strcat(newName, fichier->entete.fileName);
            Ouvrir(newName, &newFichier,
                   _NOUVEAU); /// on ouvre ce fichier on mode NOUVEAU (l'entete est généré automatiquement)
            fusionner(fichier, &newFichier, zone); ///  on fusionne zone est fichier dans le new fichier
            strcpy(newFileName, newName);
        }
    } else {
        /// reorganisation uniquement du fichier;
        reorganiserFichier(fichier, newFileName);
        Ouvrir(newFileName, fichier, _ANCIEN);
    }
}

void SauvegarderZoneFichierVide(FICHIER *fichier, ZoneTompon *zone) {
    /// Sauvegarde toute la zone tompon du programme dans un fichier supposé vide , elle est utilisé quand on insére dans un fichier initialement vide
    /// pour éviter l'appelle de la fonction de réorganisation
    Buffer *buffer = &((*fichier).buffW); /// ptr vers notre buffer
    int bloc = 0; /// bloc courrant
    int nbArticles = 0; /// nombre d'articles
    int nbChar = 0; /// nombre de chars
    int lastKey = -1; /// derniere clé dans le fichier
    /// On parcours les elements de la zone et on insére chaqun dans le fichier
    for (int i = 0; i < zone->nbElement; ++i) {
        Inserted elt = zone->array[i]; /// element courrant
        State state = elt.deleted ? DELETED : VALID; /// récupérer son etat dans state
        if (state == DELETED) continue; /// si supprimé on le passe
        int KeyM = elt.key - 1;
        char *record = buildString(getLength(elt.articleSize), state, genKey(&KeyM),
                                   elt.article); /// on génére l'enregistrement correspondant dans record
        nbArticles++;
        nbChar = nbChar + (int) elt.articleSize + 1 + 3 + 4;
        lastKey = KeyM; /// maj de la derniere clef
        /// on chauvauche le record est on l'ecrit tant qu'on peut pas l'ajouter entriement au buffer
        while (!addToBuffer(buffer, record)) {
            chauvocherBuffer(buffer, record);
            EcrireDir(fichier, bloc, buffer);
            bloc++;
            viderBuffer(buffer);
        }

    }
    /// si on a encore des char restant on les ecrit
    if (strlen(buffer->Record) != 0) {
        EcrireDir(fichier, bloc, buffer);
    }
    /// MAJ de l'entete
    fichier->entete.numberBlocs = bloc;
    fichier->entete.numberDeleted = 0;
    fichier->entete.modified = false;
    fichier->entete.lastKey = lastKey;
    fichier->entete.numberArticles = nbArticles;
    fichier->entete.numberInserted = nbChar;
    EcrireEntete(fichier);

}

