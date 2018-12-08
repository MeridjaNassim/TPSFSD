//
// Created by nassim on 22/11/2018.
//

#include "machineAbstraite.h"

#ifndef TPSFSD_GESTION_H
#define TPSFSD_GESTION_H

#endif //TPSFSD_GESTION_H

/*
* Description : Biblio des fonctions De gestion du fichier , ( reorganisation , fusion , sauvegarde  )
* Auteur : MERIDJA ABDELLAH NASSIM G02 2CP
* */
bool reorganiserFichier(FICHIER *fichier,
                        char *newFileName); /// réorganise la fichier physique en supprimant les données supprimer et donne le nom du nouveau fichier

bool fusionner(FICHIER *fichier, FICHIER *newFichier,
               ZoneTompon *zone); /// fusionne entre la zone Tampon du programme est le fichier

bool reorganiser(FICHIER *fichier, ZoneTompon *zone, bool useZone,
                 char *newFileName); /// le module de réorganisation général qui prend en charge la zone tompon
void SauvegarderZoneFichierVide(FICHIER *fichier,
                                ZoneTompon *zone); /// sauvegarde la zone tompon dans le fichier si il est vide