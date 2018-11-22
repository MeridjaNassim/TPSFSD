//
// Created by nassim on 17/11/2018.
//
#include "declarations.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#ifndef TPSFSD_UTILITY_H
#define TPSFSD_UTILITY_H

#endif //TPSFSD_UTILITY_H

bool flushBloc(FICHIER *fichier, int index);

bool readBloc(FICHIER *fichier, int index);

char *genKey(int *Key);

size_t getArticle(char *article);

char *getLength(size_t size);

char *buildString(char *length, State state, char *Key, char *article);

bool addToBuffer(Buffer *buffer, char *record);

bool chauvocherBuffer(Buffer *buffer, char *record);

bool viderBuffer(Buffer *buffer);

bool viderChaine(char *str, size_t length);

bool EcrireEntete(FICHIER *fichier);

bool lireEntete(FICHIER *fichier);

bool initEntete(FICHIER *fichier, char *Name, int nbBlocs, int nbArticles, int nbDeleted, int nbInserted, bool modified,
                int lastKey);

char *genEnteteName(const char *fileName);

void separator();

bool afficherEntete(FICHIER *fichier);

bool blocPlein(Bloc bloc);

char *subStringOfBloc(Buffer *buffer, int pos, size_t size);

bool finBloc(Bloc bloc, int pos);

void getInfo(Buffer *buff, char *taille, char *effac, char *key, int *pos, bool *depas);

int getTaille(char *chaine, int pos, size_t size);

char getEffac(char *chaine, int pos);

int getKey(char *chaine, int pos, size_t size);

bool reorganiserFichier(FICHIER *fichier);

bool fusionner(FICHIER *fichier, FICHIER *newFichier, ZoneTompon *zone);

bool reorganiser(FICHIER *fichier, ZoneTompon *zone, bool useZone);


