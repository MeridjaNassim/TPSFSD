//
// Created by nassim on 17/11/2018.
//
#include "machineAbstraite.h"

bool Ouvrir(char *nom, FICHIER *fichier, MODE mode) {
    if (nom == NULL) {
        return false;
    }
    if (fichier == NULL) {
        fichier = malloc(sizeof(FICHIER));
    }
    if (mode == _ANCIEN) {
        if ((*fichier).filePtr == NULL) {
            char *enteteName = genEnteteName(nom);
            fichier->entetePtr = fopen(enteteName, READ_APPEND);
            lireEntete(fichier);
            (*fichier).filePtr = fopen(nom, READ_APPEND);
        } else {
            fclose((*fichier).filePtr);
            (*fichier).filePtr = fopen(nom, READ_APPEND);
        }
        if ((*fichier).filePtr != NULL) {
            return true;
        }
    } else {

        if ((*fichier).filePtr == NULL) {
            (*fichier).filePtr = fopen(nom, READ_WRITE);
        } else {
            fclose((*fichier).filePtr);
            (*fichier).filePtr = fopen(nom, READ_WRITE);
        }
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
    if ((*fichier).entete.modified) {
        fflush((*fichier).filePtr);
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
    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET + sizeof(ENTETE));
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

    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET + sizeof(ENTETE));
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