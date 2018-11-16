#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BLOC_LENGTH 50
#define BASE_FILE_SIZE_IN_BLOCS 10
#define MAX_FILE_NAME 1000
#define READ_APPEND "r+"
#define READ_WRITE "w+"

typedef enum {
    _ANCIEN, _NOUVEAU
} MODE;
typedef enum {
    NAME, NB_BLOCS, NB_ARTICLES, NB_DELETED, NB_INSERTED, MODIFIED
} ATTRIBUTE;
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
} ENTETE;
typedef struct FICHIER {
    FILE *filePtr;
    Bloc blocs[BASE_FILE_SIZE_IN_BLOCS];
    Buffer buff1;
    Buffer buff2;
    ENTETE entete;
} FICHIER;

bool flushBloc(FICHIER *fichier,int index){
    fseek((*fichier).filePtr,MAX_BLOC_LENGTH*index,SEEK_SET);
    size_t count = fwrite(&((*fichier).blocs[index]),MAX_BLOC_LENGTH,1,(*fichier).filePtr);
    if(count == 0){
        return false;
    }
    return true;
}
bool readBloc(FICHIER *fichier ,int index){
    fseek((*fichier).filePtr,MAX_BLOC_LENGTH*index,SEEK_SET);
    size_t count = fread(&((*fichier).blocs[index]),MAX_BLOC_LENGTH,1,(*fichier).filePtr);
    if(count == 0){
        return false;
    }
    return true;

}
//region MACHINEABSTRAITE

bool aff_Entete(FICHIER *file,ATTRIBUTE attribute , int * value){
    char * ptr;
    switch (attribute){
        case NAME:
            ptr = (char *) value;
            strcpy((*file).entete.fileName,ptr);
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
            (*file).entete.modified =(bool) *value;
            break;
    }
    return true;
}
bool Ouvrir(char *nom, FICHIER *fichier, MODE mode) {
    if (nom == NULL) {
        return false;
    }
    if (fichier == NULL) {
        fichier = malloc(sizeof(FICHIER));
    }
    if (mode == _ANCIEN) {
        if ((*fichier).filePtr == NULL) {
            (*fichier).filePtr = fopen(nom, READ_APPEND);
        } else {
            fclose((*fichier).filePtr);
            (*fichier).filePtr = fopen(nom, READ_APPEND);
        }
        if ((*fichier).filePtr != NULL) { return true; }
    } else {

        if ((*fichier).filePtr == NULL) {
            (*fichier).filePtr = fopen(nom, READ_WRITE);
        } else {
            fclose((*fichier).filePtr);
            (*fichier).filePtr = fopen(nom, READ_WRITE);
        }
        if ((*fichier).filePtr != NULL) { return true; }
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
int getEntete(FICHIER *file , ATTRIBUTE attribute,char * fileName ){
    strcpy(fileName,(*file).entete.fileName);
    switch (attribute){
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
    }
    return -1;
}

bool lireDir(FICHIER * fichier , int index ,Buffer *buffer){
    if(fichier == NULL){
        return false;
    }
    if(index >(*fichier).entete.numberBlocs){
        return false;
    }
    readBloc(fichier,index);
    *buffer = (*fichier).blocs[index];
    return true;

}

bool EcrireDir(FICHIER * fichier , int index ,Buffer *buffer){
    if(fichier == NULL){
        return false;
    }
    if(index >(*fichier).entete.numberBlocs){
        return false;
    }
    size_t count=0;
    (*fichier).blocs[index] =*buffer;
    flushBloc(fichier,index);
    return true;

}
//endregion
int main() {

    char fileName[] = "data/read.txt";
    FICHIER file1;
    file1.filePtr =NULL;
    file1.entete.modified = false;
    file1.entete.numberBlocs =10;


    Ouvrir(fileName, &file1, _ANCIEN);
    printf("THE adress is %p \n", file1.filePtr);
    Buffer buffer2;
    strcpy(buffer2.Record,"OverwdingContent");
    EcrireDir(&file1,1,&buffer2);
    Fermer(&file1);
    return 0;
}