//
// Created by nassim on 17/11/2018.
//
#include "utility.h"

#ifndef TPSFSD_MACHINEABSTRAITE_H
#define TPSFSD_MACHINEABSTRAITE_H

#endif //TPSFSD_MACHINEABSTRAITE_H

bool Ouvrir(char *nom, FICHIER *fichier, MODE mode);

bool Fermer(FICHIER *fichier);

int getEntete(FICHIER *file, ATTRIBUTE attribute, char *fileName);

bool lireDir(FICHIER *fichier, int index, Buffer *buffer);

bool EcrireDir(FICHIER *fichier, int index, Buffer *buffer);

bool aff_Entete(FICHIER *file, ATTRIBUTE attribute, int *value);