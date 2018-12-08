//
// Created by nassim on 20/11/2018.
//
#include "Gestion.h"

#ifndef TPSFSD_FEATURES_H
#define TPSFSD_FEATURES_H

#endif //TPSFSD_FEATURES_H

/*
 * Description : Biblio des fonctions principales du programme , ( insertion , chargement , recherche , suppression )
 * Auteur : MERIDJA ABDELLAH NASSIM G02 2CP
 * */

bool initLoad(FICHIER *fichier,
              int *_KEY_); /// fonction de chargement initiale du fichiers a partir d'articles entree au clavier et des clé généré automatiquement
// est dans un ordre croissant à partir de 0
/// cout du chargement : au pire des cas chaque articles occupe un bloc dans le fichier (ie : sa taille est 999 ) ==> si n = nombre d'articles alors on aura n ecriture au disque
/// compléxité = O(n)

bool
Search(FICHIER *fichier, char *key, int *blocNum, int *indexInBloc, bool *found, ZoneTompon *zone,
       bool *foundZone); /// recherche d'une clé dans la zone et le fichier
/*
 * si l'article correspondant à la clé se trouve dans la zone  dans se cas indexBloc est l'index dans zone est foundZone = vrai , dans ce cas on aura 0 lecture
 * et 0 ecriture disque ==> recherche presque instantané
 * si l'article correspondant à la clé se trouve dans le fichier ==> recherche séquentielle dans le fichier , si on a n articles de taille maximal on aura au pire des cas n lecture de bloc
 * ==> complexité = 0(n)
 * */

bool
delete(FICHIER *fichier, char *key, ZoneTompon *zone); /// supprime un article soit de la zone soit du fichier physique
/*
 * si clé dans zone ==> suppression instantané pas de lecture disque ==> cout = 0
 * si clé dans fichier ==> si on a n articles au pire des cas on supprirme le dernier ==> n lecture + 1 ecriture de disque pour mettre a jour
 * ==> cout = n+1 accées ==> complexité = 0(n)
 * */

bool insert(FICHIER *fichier, int key, char *article, ZoneTompon *zone,
            char *newPotentielFile); /// insertion d'une clé soit dans la zone ou dans le fichier
/*
 * si on a de la place dans zone , on insere dans zone ==>  cout  = 0 accés disque (insertion instantané)
 * si la zone est pleine , on fusionne la zone courante avec le fichier ==> cout au pire cas = 2n+m accés disque avec n le nombre de bloc dans le fichier
 * m  est le nombre d'element de zone si on suppose qu'il sont de taille maximale == 999
 *remarque : si zone est de taille importante alors cette opération de fusion sera exécuté un nombre minimal de fois ie : le cout est nettement réduit par rapport au cout
 *de la méthode classique à décalage
 * */


void renameFile(FICHIER *fichier, char *newName); /// renommer un fichier ==> cout = 2 accés
void randomLoadingOfFile(FICHIER *fichier,
                         int dataSize); /// générer dataSize articles est les insére dans le fichier , cout = dataSize ecriture si on suppose que chaque araticle occupe un bloc (ie de taille maximale)
void affichierBloc(FICHIER *fichier, int bloc); /// ecrire un bloc a l'ecran ==> cout = 1 lecture direct

void afficherArticles(FICHIER *fichier,
                      ZoneTompon *zone); /// afficher le fichier et sa zone ==> cout = n lecture(de bloc) si n est le nombre de bloc
