//
// Created by nassim on 17/11/2018.
//
#include "declarations.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#ifndef TPSFSD_UTILITY_H
#define TPSFSD_UTILITY_H

#endif //TPSFSD_UTILITY_H

/*
* Description : Biblio des fonctions utiles pour la mise en oeuvre du programme.
* Auteur : MERIDJA ABDELLAH NASSIM G02 2CP
* */

char *genKey(int *Key); /// convertit la clé key en chaine de 4 chars est l'incrémente (générateur de clé)

size_t
getArticle(char *article); /// recupére un article a partir du clavier dans article et retourne sa taille en size_t

char *getLength(size_t size); /// convertit une taille en entier en chaine sur 3 chars

char *buildString(char *length, State state, char *Key,
                  char *article); /// construit l'enregistrement à ecrire dans le fichier

bool addToBuffer(Buffer *buffer,
                 char *record); /// ajoute un record au buffer si il ya de la place et retourne vrai sinon elle retourne faux

bool chauvocherBuffer(Buffer *buffer,
                      char *record); /// ajoute un nombre de char de record a buffer pour qu'il soit romplie est laisse le reste dans record

bool viderBuffer(Buffer *buffer); /// vide tous le buffer pour evité les erreurs

bool viderChaine(char *str, size_t length); /// vide une chaine

bool EcrireEntete(FICHIER *fichier); /// ecrit l'entete du fichier dans le fichier entete

bool lireEntete(FICHIER *fichier); /// lit l'entete du fichier pour récupérer ses caractéristiques

bool initEntete(FICHIER *fichier, char *Name, int nbBlocs, int nbArticles, int nbDeleted, int nbInserted, bool modified,
                int lastKey); /// initialise l'entete

char *
genEnteteName(const char *fileName); /// genere le nom de l'entete a partir du nom du fichier et la termine par .ent

char *genReorgFileName(
        const char *fileName); /// génére un nom pour le fichier après reorganisation pour ne pas perde l'ancien fichier

void separator(); /// affiche un separateur sur l'ecran

bool afficherEntete(FICHIER *fichier); /// affiche toute l'entete du fichier

bool blocPlein(Bloc bloc); /// retourne vrai si le bloc est plein

char *subStringOfBloc(Buffer *buffer, int pos, size_t size); /// n'est pas utilisé

bool finBloc(Bloc bloc, int pos); /// n'est pas utilisé

void getInfo(Buffer *buff, char *taille, char *effac, char *key, int *pos, bool *depas); /// n'est pas utilisé

int getTaille(char *chaine, int pos, size_t size); /// recupére un entier taille a partir de la position pos das chaine

char getEffac(char *chaine, int pos); /// recupére un char effacé

int getKey(char *chaine, int pos, size_t size); /// récupére une clé a partir de la position pos dans chaine

bool fusionner(FICHIER *fichier, FICHIER *newFichier,
               ZoneTompon *zone); /// fusionne le fichier avec sa zone tompon et retourne le nouveau fichier dans newFichier


char *randString(int length); /// génére une chaine aléatoirement de longueur length

bool continuE(); /// déroulement du programme

void AfficherMenu(); /// affiche le  menu d'options

void askFileName(char *filename); /// demande le nom du fichier

void affichierMain(); /// premier affichage du programme
