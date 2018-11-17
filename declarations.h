//
// Created by nassim on 17/11/2018.
//
#include <stdio.h>
#include <stdbool.h>

#ifndef TPSFSD_DECLARATIONS_H
#define TPSFSD_DECLARATIONS_H
#define MAX_BLOC_LENGTH 10000
#define BASE_FILE_SIZE_IN_BLOCS 10
#define MAX_FILE_NAME 1000
#define MAX_ARTICLE_LENGTH 999
#define READ_APPEND "r+"
#define READ_WRITE "w+"
#endif //TPSFSD_DECLARATIONS_H

/// Type definitions : --------
typedef enum {
    _ANCIEN, _NOUVEAU
} MODE;
typedef enum {
    NAME, NB_BLOCS, NB_ARTICLES, NB_DELETED, NB_INSERTED, MODIFIED, LAST_KEY
} ATTRIBUTE;
typedef enum {
    VALID, DELETED, UNKNOWN
} State;
typedef struct Bloc {
    char Record[MAX_BLOC_LENGTH];
} Bloc, Buffer;
typedef struct ENTETE {
    char fileName[MAX_FILE_NAME];
    int numberBlocs;
    int numberArticles;
    int numberDeleted;
    int numberInserted;
    bool modified;
    int lastKey;
} ENTETE;
typedef struct FICHIER {
    FILE *filePtr;
    Bloc blocs[BASE_FILE_SIZE_IN_BLOCS];
    Buffer buff1;
    Buffer buff2;
    ENTETE entete;
} FICHIER;