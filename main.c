#include "features.h"

/**
 * TP SFSD 2CP- 2018/2019- STRUCTURES SIMPLES
 * Auteur : MERIDJA ABDELLAH NASSIM (monome)
 * GROUPE : 02
 * ENSEIGNANT : Mme . Alia
 * */


int _KEY_ = -1; /// clé initial pour chargement initiale

int main() {


    /// MAIN PROGRAMME LOOOP
    affichierMain();
    //region VARIABLES DU FICHIER
    FICHIER file;
    ZoneTompon zone;
    zone.nbElement = 0;
    char fileName[MAX_FILE_NAME];
    bool fileOpened = false;
    //endregion
    /// Ouverture du fichier selon le mode
    while (!fileOpened) {
        int choice1 = 0;
        printf("Entrer votre choix d'exécution \n");
        scanf("%d", &choice1);
        switch (choice1) {
            case 1:
                printf("Entrer le nom/Path du nouveau fichier a creer \n");
                askFileName(fileName);
                Ouvrir(fileName, &file, _NOUVEAU);
                break;
            case 2:
                printf("Entrer le nom/Path de l'ancien  fichier\n");
                askFileName(fileName);
                Ouvrir(fileName, &file, _ANCIEN);
                break;
            default:
                break;
        }
        if (file.filePtr != NULL) {
            fileOpened = true;
            separator();
        } else {
            printf("Le fichier na pas pu etre créer ou ouvrir ... reessayer \n");
            separator();
        }
    }
    //region Variables de manipulation du menu
    bool quit = false;
    int choice2 = 0;
    int searchedkey = 0;
    int tempKey;
    int numBloc = 0;
    int nbdonnes = 0;
    int pos = 0;
    bool found = false;
    bool foundZone = false;
    char articleInsert[MAX_ARTICLE_LENGTH];
    char newFileName[MAX_FILE_NAME];
    char KeyChar[5];
    //endregion
    while (!quit) {
        AfficherMenu();
        separator();
        printf("Entrer votre choix : ");
        scanf("%d", &choice2);
        switch (choice2) {
            case 0: /// chargement aléatoire ;
                printf("Chargement Aleatoire du fichier \n");
                printf("Entrer le nombre de donnes a insérer aleatoirement : ");
                scanf("%d", &nbdonnes);
                srand(time(NULL));
                randomLoadingOfFile(&file, nbdonnes);
                if (continuE()) {
                    continue;
                }
                break;
            case 1: /// chargement initiale du fichier new
                initLoad(&file, &_KEY_);

                if (continuE()) {
                    continue;
                }
                break;
            case 2: /// recherche un article a partir de sa clé
                printf("Entrer la clé recherche :");
                scanf("%d", &searchedkey);
                tempKey = searchedkey - 1;
                numBloc = 0;
                pos = 0;
                found = false;
                foundZone = false;
                Search(&file, genKey(&tempKey), &numBloc, &pos, &found, &zone,
                       &foundZone); /// recherche de la clé la position correspond a la position de départ de l'article
                if (foundZone) {
                    /// on a trouvé l'article dans la zone temporaire d'insertion (accées très rapide)
                    printf("L'article correspondant à la cle recherche est trouvé dans la zone tompon à la position (%d) \n",
                           pos);
                    separator();
                    if (continuE()) {
                        continue;
                    }
                } else if (found) {
                    /// clé trouvé dans le fichier physique
                    printf("L'article correspondant à la cle recherche est trouvee dans le fichier au bloc (%d) à la position (%d) \n",
                           numBloc, pos);
                    separator();
                    if (continuE()) {
                        continue;
                    }
                } else {
                    printf("l'article n'existe pas dans le fichier ...\n");
                    separator();
                    if (continuE()) {
                        continue;
                    }
                }
                break;
            case 3: /// Insertion d'un article;
                printf("Entrer l'article à inserer :");
                viderChaine(articleInsert, MAX_ARTICLE_LENGTH);
                fflush(stdin);
                fgets(articleInsert, MAX_ARTICLE_LENGTH, stdin);
                articleInsert[strlen(articleInsert) - 1] = '\0';
                separator();
                printf("Entrer sa cle primaire unique:  \n");
                scanf("%s", KeyChar);
                viderChaine(newFileName, MAX_FILE_NAME);
                if (insert(&file, getKey(KeyChar, 0, 4), articleInsert, &zone, newFileName)) {
                    printf("Insere avec succees \n");
                }
                if (strlen(newFileName) != 0) {
                    /// nouveau fichier creer on ferme le courant et on ouvre le nouveau
                    Fermer(&file);
                    Ouvrir(newFileName, &file, _ANCIEN);
                }
                separator();
                if (continuE()) {
                    continue;
                }
                break;
            case 4:  /// supprimer un article
                viderChaine(KeyChar, 5);
                printf("Entrer la clé de l'enregistrement a supprimer (4 charactere) : ");
                scanf("%s", KeyChar);
                if (delete(&file, KeyChar, &zone)) {
                    printf("Supprime avec success \n");
                }
                if (continuE()) {
                    continue;
                }
                break;
            case 5: /// afficher le contenu d'un bloc du fichier
                printf("Entrer le numéro du bloc à afficher : ");
                scanf("%d", &numBloc);
                affichierBloc(&file, numBloc);
                if (continuE()) {
                    continue;
                }
                break;
            case 6: /// affichage de tous les articles
                printf("Affichage de tous les articles du programme(Fichier/Zone) \n");
                afficherArticles(&file, &zone);
                if (continuE()) {
                    continue;
                }
                break;
            case 7: /// réorganisation manuelle
                printf("Reorganisation manuelle du fichier ... \n");
                reorganiserFichier(&file, newFileName);
                if (newFileName != NULL) {
                    Fermer(&file);
                    Ouvrir(newFileName, &file, _ANCIEN);
                } else {
                    printf("Erreur dans la réorganisation du fichier \n");
                }
                if (continuE()) {
                    continue;
                }
                break;
            case 8 : /// Afficher l'entete du fichier
                afficherEntete(&file);
                if (continuE()) {
                    continue;
                }
                break;
            case 9: /// sauvgarde de l'entete et le fichier
                fflush(file.filePtr); /// ecriture des buffer interne dans le fichier physique
                if (EcrireEntete(&file)) {
                    separator();
                    printf("Sauvegarde de l'entete avec succes ...\n");
                } else {
                    printf("Erreur dans la sauvegarde de l'entete.\n");
                }
                if (continuE()) {
                    continue;
                }
                break;
            case 10: /// renommer le fichier
                viderChaine(newFileName, MAX_FILE_NAME);
                printf("Entrer le nom du nouveau fichier : \n");
                fflush(stdin);
                fgets(newFileName, MAX_FILE_NAME, stdin);
                renameFile(&file, newFileName);
                break;
            case 11: /// Quitter
                quit = true;
                break;
            default:
                printf("Operation inexistante rechoisisez ... \n");
                continue;
        }

        /// verification pour la réorganisation automatique : si nombre d'articles supprimé > 1/2 * nombre d'articles dans le fichier
        if (file.entete.numberDeleted > POURCENTAGE * file.entete.numberArticles) {
            printf("Reorganisation Automatique du fichier ... \n");
            reorganiserFichier(&file, newFileName);
            if (newFileName != NULL) {
                Fermer(&file);
                Ouvrir(newFileName, &file, _ANCIEN);
            } else {
                printf("Erreur dans la réorganisation du fichier \n");
            }
        }

    }
    if (zone.nbElement > 0) {
        /// on doit insérer les element de la zone avant de fermer le fichier
        if (file.entete.numberBlocs == 0) {
            /// fichier vide on insere toute la zone dans ce fichier
            SauvegarderZoneFichierVide(&file, &zone);
        } else {
            /// on a besoin d'un nouveau fichier
            reorganiser(&file, &zone, true, newFileName);
            return 0;/// on sort
        }
    }
    /// pas d'élements inséré dans la zone tompon ==> ferméture simple
    Fermer(&file);
    return 0;
}