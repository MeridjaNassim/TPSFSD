//
// Created by nassim on 22/11/2018.
//
#include "machineAbstraite.h"

#ifndef TPSFSD_GESTION_H
#define TPSFSD_GESTION_H

#endif //TPSFSD_GESTION_H


bool reorganiserFichier(FICHIER *fichier);

bool fusionner(FICHIER *fichier, FICHIER *newFichier, ZoneTompon *zone);

bool reorganiser(FICHIER *fichier, ZoneTompon *zone, bool useZone);