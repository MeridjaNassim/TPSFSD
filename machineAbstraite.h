//
// Created by nassim on 17/11/2018.
//
#include "utility.h"

#ifndef TPSFSD_MACHINEABSTRAITE_H
#define TPSFSD_MACHINEABSTRAITE_H

#endif //TPSFSD_MACHINEABSTRAITE_H
/*
* Description : Biblio de la machine abstraite du fichier
* Auteur : MERIDJA ABDELLAH NASSIM G02 2CP
* */
/// MACHINE ABSTRAITE DU FICHIER
bool Ouvrir(char *nom, FICHIER *fichier, MODE mode); /// ouvre un fichier

bool Fermer(FICHIER *fichier); /// ferme le fichier

int getEntete(FICHIER *file, ATTRIBUTE attribute,
              char *fileName); /// retournne l'ATTRIBUE  spécifié à partir de l'entete et retourne le nom dans fileName

bool lireDir(FICHIER *fichier, int index, Buffer *buffer); /// lit le bloc de num index du fichier dans buffer

bool EcrireDir(FICHIER *fichier, int index, Buffer *buffer); /// ecrit dans le  bloc index le contenu de buffer

bool aff_Entete(FICHIER *file, ATTRIBUTE attribute, int *value); /// affecte value à l'attribue

///-----------------------------------------------------------
/// FONCTIONS PARTICULIERES UTILISE POUR MANIPULER LA STRUCTURE

void putRecordFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *key, char *eff,
                   char *article);  /// utilisé dans la fusion pour avancé dynamiquement le bloc et la pos a cause du chauvauchement

void getNextRecordFromZone(ZoneTompon *zone, int *index, char *taille, char *key, char *eff,
                           char *article); /// recupére le prochain record de la zone (utilisé dans fusion)

void getNextRecordInFile(FICHIER *fichier, int *bloc, int *index, char *taille, char *eff, char *key,
                         char *article);/// récupére le prochain Record du fichier est met ajour  bloc et pos (utilisé dans fusion et reorganisation)

bool initInserted(Inserted *elt, int key, char *article, bool deleted, size_t size); /// initialize l'element à insérer

void decalerZone(ZoneTompon *zone, int pos,
                 int start); /// décale les élements de zone pas pos position (utilisé dans l'insértion dans zone)

void searchZone(ZoneTompon *zone, int key, bool *found, size_t *pos); /// recherche une clé dans la zone