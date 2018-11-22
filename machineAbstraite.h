//
// Created by nassim on 17/11/2018.
//
#include "utility.h"

#ifndef TPSFSD_MACHINEABSTRAITE_H
#define TPSFSD_MACHINEABSTRAITE_H

#endif //TPSFSD_MACHINEABSTRAITE_H
/// MACHINE ABSTRAITE DU FICHIER
bool Ouvrir(char *nom, FICHIER *fichier, MODE mode);

bool Fermer(FICHIER *fichier);

int getEntete(FICHIER *file, ATTRIBUTE attribute, char *fileName);

bool lireDir(FICHIER *fichier, int index, Buffer *buffer);

bool EcrireDir(FICHIER *fichier, int index, Buffer *buffer);

bool aff_Entete(FICHIER *file, ATTRIBUTE attribute, int *value);

///-----------------------------------------------------------
/// FONCTIONS PARTICULIERES UTILISE POUR MANIPULER LA STRUCTURE

void putRecordFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *key, char *eff, char *article);

void getNextRecordFromZone(ZoneTompon *zone, int *index, char *taille, char *key, char *eff, char *article);

void getNextRecordInFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *eff, char *key, char *article);

bool initInserted(Inserted *elt, int key, char *article, bool deleted, size_t size);

void decalerZone(ZoneTompon *zone, int pos, int start);

void searchZone(ZoneTompon *zone, int key, bool *found, size_t *pos);