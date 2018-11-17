//
// Created by nassim on 17/11/2018.
//

#include "utility.h"

bool flushBloc(FICHIER *fichier, int index) {
    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
    size_t count = fwrite(&((*fichier).blocs[index]), MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
    if (count == 0) {
        return false;
    }
    return true;
}

bool readBloc(FICHIER *fichier, int index) {
    fseek((*fichier).filePtr, MAX_BLOC_LENGTH * index, SEEK_SET);
    size_t count = fread(&((*fichier).blocs[index]), MAX_BLOC_LENGTH, 1, (*fichier).filePtr);
    if (count == 0) {
        return false;
    }
    return true;

}

char *genKey(int *Key) {
    char *ret = malloc(sizeof(char) * 5);
    snprintf(ret, 5, "%04d", *Key);
    *Key = *Key++;
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