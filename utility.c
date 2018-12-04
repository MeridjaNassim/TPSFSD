//
// Created by nassim on 17/11/2018.
//

#include "utility.h"
//bool flushBloc(FICHIER *fichier, int index) {
//    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
//    size_t count = fwrite(&((*fichier).buffW), MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
//    if (count == 0) {
//        return false;
//    }
//    return true;
//}
//
//bool readBloc(FICHIER *fichier, int index) {
//    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
//    size_t count = fread(&((*fichier).buffR), MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
//    if (count == 0) {
//        return false;
//    }
//    return true;
//
//}

char *genKey(int *Key) {
    *Key = (*Key) + 1;
    char *ret = malloc(sizeof(char) * 5);
    snprintf(ret, 5, "%04d", *Key);
    return ret;
}

size_t getArticle(char *article) {
    fgets(article, MAX_ARTICLE_LENGTH, stdin);
    size_t size = strlen(article) - 1;

    if (article[size] == '\n')
        article[size] = '\0';
    return size;
}

char *getLength(size_t size) {
    char *buffer = malloc(sizeof(char) * 4);
    snprintf(buffer, 4, "%03d", (int) size);
    return buffer;
}

char *buildString(char *length, State state, char *Key, char *article) {
    char *result = malloc(sizeof(char) * MAX_BLOC_LENGTH);
    if (length != NULL) {
        strcpy(result, length);
    } else {
        return NULL;
    }
    if (state == VALID) {
        strcat(result, "V");
    } else {
        if (state == DELETED) {
            strcat(result, "D");
        } else {
            strcat(result, "U");
        }
    }
    if (Key != NULL) {
        strcat(result, Key);
        if (article != NULL) {
            strcat(result, article);
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
    return result;
}

bool addToBuffer(Buffer *buffer, char *record) {
    /// fonction qui ajoute un record au buffer si il y'a de l'espace suffisant
    ///Si buffer est vide on crée un nouveau est on affect le record dans buffer
    if (buffer == NULL) {
        buffer = malloc(sizeof(buffer));
        strcpy((*buffer).Record, record);
        return true;
    }
    int sizeBuff = (int) strlen((*buffer).Record); /// recupéré la taille du buffer courante
    int sizeRecord = (int) strlen(record); /// la taille de l'enregistrement a ecrire
    size_t emptySpace = (size_t) (MAX_BLOC_LENGTH - sizeBuff); /// l'espace vide dans le buffer
    if (emptySpace >= sizeRecord) { /// si on a de la place pour ecrire
        strcat((*buffer).Record, record);

        return true;
    } else {
        return false;

    }

}

bool chauvocherBuffer(Buffer *buffer, char *record) {
    /// fonction qui ajoute une partie de record à buffer pour le remplir 100
    /// et elle retourne dans record le reste du record
    if (buffer == NULL || record == NULL) {
        return false;
    }
    size_t buffSize = strlen(buffer->Record); /// taille du buffer courante
    size_t emptySpace = MAX_BLOC_LENGTH - buffSize; /// espace libre du buffer
    strncat(buffer->Record + buffSize, record, emptySpace); /// on copie les caractére possible dans le buffer
    strcpy(record, record + emptySpace); /// mettre le reste dans record pour l'ecrire dans un nouveau buffer
    return true;
}

bool viderBuffer(Buffer *buffer) {
    /// Fonction utilisé pour vider le contenu du Buffer
    if (buffer == NULL) {
        return false;
    }
    memset(buffer->Record, 0, sizeof(*buffer));
    return true;
}

bool viderChaine(char *str, size_t length) {
    if (str == NULL) {
        return false;
    }
    memset(str, 0, length);
    return true;
}

bool EcrireEntete(FICHIER *fichier) {
    if (fichier == NULL || fichier->entetePtr == NULL) {
        return false;
    }

    rewind(fichier->entetePtr);
    size_t count = fwrite(&((*fichier).entete), sizeof((*fichier).entete), 1, fichier->entetePtr);
    if (count == 0) {
        return false;
    }
    return true;
}

bool lireEntete(FICHIER *fichier) {
    if (fichier == NULL || fichier->entetePtr == NULL) {
        return false;
    }
    rewind(fichier->entetePtr);
    size_t count = fread(&((*fichier).entete), sizeof(ENTETE), 1, fichier->entetePtr);
    if (count == 0) {
        return false;
    }
    return true;
}

bool initEntete(FICHIER *fichier, char *Name, int nbBlocs, int nbArticles, int nbDeleted, int nbInserted, bool modified,
                int lastKey) {
    if (fichier == NULL) {
        return false;
    }
    (*fichier).entete.numberBlocs = nbBlocs;

    (*fichier).entete.numberArticles = nbArticles;

    (*fichier).entete.numberDeleted = nbDeleted;

    (*fichier).entete.numberInserted = nbInserted;

    (*fichier).entete.modified = modified;
    (*fichier).entete.lastKey = lastKey;

    strcpy((*fichier).entete.fileName, Name);

    return true;
}

char *genEnteteName(const char *fileName) {
    char *entete = malloc(sizeof(char) * MAX_FILE_NAME);
    strcpy(entete, fileName);
    char *firstPart = strtok(entete, ".");
    strcpy(entete, strcat(firstPart, ".ent"));
    return entete;
}

char *genReorgFileName(const char *fileName) {
    char *reor = malloc(sizeof(char) * MAX_FILE_NAME);
    strcpy(reor, fileName);
    char *firstPart = strtok(reor, ".");
    strcpy(reor, strcat(firstPart, "(edited).txt"));
    return reor;
}

void separator() {
    printf("--------------------------------\n");
}

bool afficherEntete(FICHIER *fichier) {
    separator();
    printf("Affichage de l'entete du fichier\n");
    separator();
    printf("Nom/Path du fichier entete est : %s \n", genEnteteName(fichier->entete.fileName));
    separator();
    printf("Nom/Path du fichier est : %s\n", fichier->entete.fileName);
    separator();
    printf("Nombre d'articles dans le fichier est : %d\n", fichier->entete.numberArticles);
    separator();
    printf("Nombre de Blocs dans le fichier est : %d\n", fichier->entete.numberBlocs);
    separator();
    printf("Nombre de characteres dans le fichier est : %d\n", fichier->entete.numberInserted);
    separator();
    printf("Nombre d'articles supprimes dans le fichier est : %d\n", fichier->entete.numberDeleted);
    separator();
    printf("Derniere cle dans le fichier est : %d\n", fichier->entete.lastKey);
    separator();
    printf("Le fichier a etait modifié sans enregistré : %s\n", fichier->entete.modified ? "Oui" : "Non");

}


bool blocPlein(Bloc bloc) {
    if (strlen(bloc.Record) == MAX_BLOC_LENGTH) {
        return true;
    }
    return false;
}

char *subStringOfBloc(Buffer *buffer, int pos, size_t size) {
    char *result = malloc(sizeof(char) * size);
    strncpy(result, buffer->Record + pos, size);
    return result;
}

bool finBloc(Bloc bloc, int pos) {
    return pos >= MAX_BLOC_LENGTH;
}

void getInfo(Buffer *buff, char *taille, char *effac, char *key, int *pos, bool *depas) {
    int index = 0;
    while ((*pos) < MAX_BLOC_LENGTH && index < 3) {
        taille[index] = buff->Record[(*pos)];
        index++;
        (*pos)++;
    }
    if ((*pos) >= MAX_BLOC_LENGTH) {
        *depas = true;
    } else {
        *effac = buff->Record[(*pos)];
        (*pos)++;
        if ((*pos) >= MAX_BLOC_LENGTH) *depas = true;
        else {
            int index2 = 0;
            while ((*pos) < MAX_BLOC_LENGTH && index2 < 4) {
                key[index2] = buff->Record[(*pos)];
                index2++;
                (*pos)++;
            }
            if ((*pos) >= MAX_BLOC_LENGTH) *depas = true;
        }
    }
}

int getTaille(char *chaine, int pos, size_t size) {
    char *taille = malloc(sizeof(char) * size);
    strncpy(taille, chaine + pos, size);
    int tai = atoi(taille);
    return tai;
}

char getEffac(char *chaine, int pos) {
    return chaine[pos];
}

int getKey(char *chaine, int pos, size_t size) {
    char *taille = malloc(sizeof(char) * size);
    strncpy(taille, chaine + pos, size);
    int key = atoi(taille);
    return key;
}
