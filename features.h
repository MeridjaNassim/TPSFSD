//
// Created by nassim on 20/11/2018.
//
#include "machineAbstraite.h"

#ifndef TPSFSD_FEATURES_H
#define TPSFSD_FEATURES_H

#endif //TPSFSD_FEATURES_H

bool initLoad(FICHIER *fichier, int *_KEY_);

bool
Search(FICHIER *fichier, char *key, int *blocNum, int *indexInBloc, bool *found, ZoneTompon *zone, bool *foundZone);

bool delete(FICHIER *fichier, char *key, ZoneTompon *zone);

bool insert(FICHIER *fichier, int key, char *article, ZoneTompon *zone);