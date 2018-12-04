//
// Created by nassim on 17/11/2018.
//
#include "machineAbstraite.h"

bool Ouvrir(char *nom, FICHIER *fichier, MODE mode) {
    if (nom == NULL || nom[0] == '\0') {
        return false;
    }
    if (fichier == NULL) {
        fichier = malloc(sizeof(FICHIER));
    }
    if (mode == _ANCIEN) {

        char *enteteName = genEnteteName(nom);
        fichier->entetePtr = fopen(enteteName, READ_APPEND);
        lireEntete(fichier);
        (*fichier).filePtr = fopen(nom, READ_APPEND);

        if ((*fichier).filePtr != NULL) {
            return true;
        }
    } else {


        (*fichier).filePtr = fopen(nom, READ_WRITE);
        if ((*fichier).filePtr != NULL) {
            initEntete(fichier, nom, 0, 0, 0, 0, false, -1); /// initialisation de l'entete
            char *enteteName = genEnteteName(nom);
            fichier->entetePtr = fopen(enteteName, READ_WRITE);
            EcrireEntete(fichier);

            return true;
        }


    }
    return false;
}

bool Fermer(FICHIER *fichier) {
    fflush((*fichier).filePtr);
    /// on vas enregistré si fichier modifié
    if (fichier->entete.modified) {
        fichier->entete.modified = false;
        EcrireEntete(fichier);
    }
    int answer = fclose((*fichier).filePtr);
    if (answer == 0) {
        return true;
    } else return false;
}

int getEntete(FICHIER *file, ATTRIBUTE attribute, char *fileName) {
    strcpy(fileName, (*file).entete.fileName);
    switch (attribute) {
        case NAME:
            return -1;
        case NB_BLOCS:
            return (*file).entete.numberBlocs;
        case NB_ARTICLES:
            return (*file).entete.numberArticles;
        case NB_DELETED:
            return (*file).entete.numberDeleted;
        case NB_INSERTED:
            return (*file).entete.numberInserted;
        case MODIFIED:
            return (*file).entete.modified;
        case LAST_KEY:
            return (*file).entete.lastKey;
    }
    return -1;
}

bool lireDir(FICHIER *fichier, int index, Buffer *buffer) {
    if (fichier == NULL) {
        return false;
    }
    if (index > (*fichier).entete.numberBlocs) {
        return false;
    }
    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
    size_t count = fread(buffer, MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
    if (count == 0) {
        return false;
    }
    return true;


}

bool EcrireDir(FICHIER *fichier, int index, Buffer *buffer) {
    if (fichier == NULL) {
        return false;
    }
    if (index > (*fichier).entete.numberBlocs) {
        (*fichier).entete.numberBlocs++;
        (*fichier).entete.modified = true;
    }

    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
    size_t count = fwrite(buffer->Record, MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
    if (count == 0) {
        return false;
    }
    return true;

}

bool aff_Entete(FICHIER *file, ATTRIBUTE attribute, int *value) {
    char *ptr;
    switch (attribute) {
        case NAME:
            ptr = (char *) value;
            strcpy((*file).entete.fileName, ptr);
            break;
        case NB_BLOCS:
            (*file).entete.numberBlocs = *value;
            break;
        case NB_ARTICLES:
            (*file).entete.numberArticles = *value;
            break;
        case NB_DELETED:
            (*file).entete.numberDeleted = *value;
            break;
        case NB_INSERTED:
            (*file).entete.numberInserted = *value;
            break;
        case MODIFIED:
            (*file).entete.modified = (bool) *value;
            break;
        case LAST_KEY:
            (*file).entete.lastKey = *value;
            break;
    }
    return true;
}

void putRecordFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *key, char *eff, char *article) {
    Buffer *buff = &((*fichier).buffW); /// buffer d'ecriture
    viderBuffer(buff); /// on le vide pour ne pas avoir d'erreur
    lireDir(fichier, (*bloc), buff); /// on lit le bloc courant
    State state = VALID;
    if ((*eff) == 'D') {
        state = DELETED;
    }
    char *record = buildString(taille, state, key, article); /// on construit le record a ecrire
    while (!addToBuffer(buff, record)) {
        /// tant que record n'est pas ajouté entierement dans le buffer on chevauche
        chauvocherBuffer(buff, record);
        EcrireDir(fichier, (*bloc), buff); /// on ecrit le buffer plein
        (*bloc) = (*bloc) + 1; /// on incremente le nombre de bloc
        viderBuffer(buff);
    }
    if (strlen(buff->Record) != 0) {
        EcrireDir(fichier, (*bloc), buff);
    }
}

void getNextRecordFromZone(ZoneTompon *zone, int *index, char *taille, char *key, char *eff, char *article) {
    viderChaine(taille, 4);
    viderChaine(key, 5);
    viderChaine(article, MAX_ARTICLE_LENGTH);
    Inserted elt = zone->array[*(index)];
    (*index) = (*index) + 1;
    strcpy(taille, getLength(elt.articleSize));
    int keyTemp = elt.key - 1;
    strcpy(key, genKey(&keyTemp));
    if (elt.deleted) {
        *eff = 'D';
    } else {
        *eff = 'V';
    }
    strcpy(article, elt.article);
}

void getNextRecordInFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *eff, char *key, char *article) {
    /// fonction qui récupére le prochain record a partir du bloc a la position index
    Buffer *buff = &((*fichier).buffR);
    viderBuffer(buff);
    viderChaine(taille, 4);
    viderChaine(key, 5);
    viderChaine(article, MAX_ARTICLE_LENGTH);
    lireDir(fichier, (*bloc), buff);
    for (int i = 0; i < 3; ++i) {
        taille[i] = buff->Record[(*index)];
        (*index)++;
        if ((*index) >= MAX_BLOC_LENGTH) {
            (*bloc)++;
            viderBuffer(buff);
            lireDir(fichier, (*bloc), buff);
            (*index) = 0;
        }
    }
    /// lecture du effacé
    *eff = buff->Record[(*index)];
    (*index)++;
    if ((*index) >= MAX_BLOC_LENGTH) {
        (*bloc)++;
        viderBuffer(buff);
        lireDir(fichier, (*bloc), buff);
        (*index) = 0;
    }

    ///Lecture de la clé
    for (int i = 0; i < 4; ++i) {
        key[i] = buff->Record[(*index)];
        (*index)++;
        if ((*index) >= MAX_BLOC_LENGTH) {
            (*bloc)++;
            viderBuffer(buff);
            lireDir(fichier, (*bloc), buff);
            (*index) = 0;
        }
    }
    size_t tai = (size_t) getTaille(taille, 0, 3);
    for (int j = 0; j < tai; ++j) {
        article[j] = buff->Record[(*index)];
        (*index)++;
        if ((*index) >= MAX_BLOC_LENGTH) {
            (*bloc)++;
            viderBuffer(buff);
            lireDir(fichier, (*bloc), buff);
            (*index) = 0;
        }
    }
}

bool initInserted(Inserted *elt, int key, char *article, bool deleted, size_t size) {
    if (elt == NULL) {
        return false;
    } else {
        elt->key = key;
        elt->deleted = deleted;
        elt->articleSize = size;
        strcpy(elt->article, article);
    }
}

void decalerZone(ZoneTompon *zone, int pos, int start) {
    if (zone->nbElement < MAX_ZONE_TEMPON) {
        for (int i = zone->nbElement - 1; i >= start; --i) {
            zone->array[i + 1] = zone->array[i];
        }
    }
}

void searchZone(ZoneTompon *zone, int key, bool *found, size_t *pos) {
    /// recherche dichootomique d'une clé dans la zone tompon
    int inf = 0;
    int sup = zone->nbElement - 1;
    while (inf <= sup) {
        int middle = (inf + sup) / 2;
        int var = zone->array[middle].key;
        if (var == key) {
            *found = true;
            *pos = (size_t) middle;
            break;
        } else if (key > var) {
            inf = middle + 1;
        } else {
            sup = middle - 1;
        }
    }
    if (!(*found)) {
        *pos = (size_t) inf;
    }
}