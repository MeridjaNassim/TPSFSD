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