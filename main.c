#include "features.h"

#define POURCENTAGE 0.5
int _KEY_ = -1;

void AfficherMenu() {
    separator();
    printf("<==CHOISIR UNE OPERATION==> \n");
    separator();
    printf("1->Chargement initiale d'un nouveau fichier \n");
    separator();
    printf("2->Rechercher un article a partir de sa clé primaire \n");
    separator();
    printf("3->Inserer un nouveau article avec sa cle \n");
    separator();
    printf("4->Supprimer un article à partir de sa cle \n");
    separator();
    printf("5->Afficher un Bloc dans le fichier courant \n");
    separator();
    printf("6->Lister tous les articles \n");
    separator();
    printf("7->Réorganiser le fichier \n");
    separator();
    printf("8->Afficher l'entete du fichier \n");
    separator();
    printf("9->Sauvegarder l'entete du fichier \n");
    separator();
    printf("10->Quitter \n");

}

void askFileName(char *filename) {
    scanf("%s", filename);
}

void affichierMain() {
    separator();
    printf("Programme de manipulation de fichier \n");
    separator();
    separator();
    printf("SELECTION DU FICHIER \n");
    printf("1-Creer un nouveau fichier \n");
    printf("2-Utiliser un fichier existant sur le disque dur \n");
}

void affichierBloc(FICHIER *fichier, int bloc) {
    Buffer *buff = &((*fichier).buffR);
    viderBuffer(buff);
    lireDir(fichier, bloc, buff);
    separator();
    size_t t = strlen(buff->Record);
    if (t != 0) {
        char temp[MAX_BLOC_LENGTH + 1] = "";
        viderChaine(temp, MAX_BLOC_LENGTH + 1);
        strncat(temp, buff->Record, MAX_BLOC_LENGTH);
        strcat(temp, "\0");
        printf("le bloc (%d) contient : %s \n", bloc, temp);
        separator();
    } else {
        printf("le bloc (%d) est vide ", bloc);
    }
}

void afficherArticles(FICHIER *fichier, ZoneTompon *zone) {
    /// affichage de la zone tampon :
    int index = 0;
    char taille[4];
    char eff;
    char key[5];
    char article[MAX_ARTICLE_LENGTH];
    separator();

    printf("Element insere recement ------- \n");
    separator();
    printf("Cle---||---ETAT---||---Taille---||---Article\n");
    separator();
    while (index < zone->nbElement) {
        getNextRecordFromZone(zone, &index, taille, key, &eff, article);
        printf("%s---||---%c---||---%s---||---%s\n", key, eff, taille, article);
        separator();
    }

    /// Affichage des articles dans le fichier
    if (fichier->entete.numberArticles > 0) {
        int pos = 0;
        int bloc = 0;
        char tailleF[4];
        char effF;
        char keyF[5];
        char articleF[MAX_ARTICLE_LENGTH];
        separator();
        printf("Les Articles dans le fichier ----");
        separator();
        printf("Cle---||---ETAT---||---Taille---||---Article\n");
        separator();
        while (bloc < fichier->entete.numberBlocs) {
            getNextRecordInFile(fichier, &bloc, &pos, tailleF, &effF, keyF, articleF);
            printf("%s---||---%c---||---%s---||---%s\n", keyF, effF, tailleF, articleF);
            separator();
        }

    }

}

bool continuE() {

    printf("Continuer le traitement ? [1] ...\n");
    int cont;
    scanf("%d", &cont);
    if (cont == 1) {
        return true;
    } else {
        return false;
    }
}

void SauvegarderZoneFichierVide(FICHIER *fichier, ZoneTompon *zone) {
    Buffer *buffer = &((*fichier).buffW);
    int bloc = 0;
    int nbArticles = 0;
    int nbChar = 0;
    int lastKey = -1;
    for (int i = 0; i < zone->nbElement; ++i) {
        Inserted elt = zone->array[i];
        State state = elt.deleted ? DELETED : VALID;
        if (state == DELETED) continue;
        int KeyM = elt.key - 1;
        char *record = buildString(getLength(elt.articleSize), state, genKey(&KeyM), elt.article);
        nbArticles++;
        nbChar = nbChar + (int) elt.articleSize + 1 + 3 + 4;
        lastKey = KeyM;
        while (!addToBuffer(buffer, record)) {
            chauvocherBuffer(buffer, record);
            EcrireDir(fichier, bloc, buffer);
            bloc++;
            viderBuffer(buffer);
        }

    }
    if (strlen(buffer->Record) != 0) {
        EcrireDir(fichier, bloc, buffer);
    }
    fichier->entete.numberBlocs = bloc;
    fichier->entete.numberDeleted = 0;
    fichier->entete.modified = false;
    fichier->entete.lastKey = lastKey;
    fichier->entete.numberArticles = nbArticles;
    fichier->entete.numberInserted = nbChar;
    EcrireEntete(fichier);

}

//bool ConvertEnteteToString()
int main() {


    /// MAIN PROGRAMME LOOOP
    affichierMain();
    FICHIER file;
    ZoneTompon zone;
    zone.nbElement = 0;
    char fileName[MAX_FILE_NAME];
    bool fileOpened = false;
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
    bool quit = false;
    int choice2 = 0;
    int searchedkey = 0;
    int tempKey;
    int numBloc = 0;
    int pos = 0;
    bool found = false;
    bool foundZone = false;
    char articleInsert[MAX_ARTICLE_LENGTH];
    char newFileName[MAX_FILE_NAME];
    char KeyChar[5];
    while (!quit) {
        AfficherMenu();
        separator();
        printf("Entrer votre choix : ");
        scanf("%d", &choice2);
        switch (choice2) {
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
                Search(&file, genKey(&tempKey), &numBloc, &pos, &found, &zone, &foundZone);
                if (foundZone) {
                    printf("L'article correspondant à la cle recherche est trouvé dans la zone tompon à la position (%d) \n",
                           pos);
                    separator();
                    if (continuE()) {
                        continue;
                    }
                } else if (found) {
                    printf("L'article correspondant à la cle recherche est trouvé dans le fichier au bloc (%d) à la position (%d)",
                           numBloc, pos);
                    separator();
                    if (continuE()) {
                        continue;
                    }
                } else {
                    printf("l'article n'existe pas dans le fichier ...");
                    separator();
                    if (continuE()) {
                        continue;
                    }
                }
                break;
            case 3: /// Insertion d'un article;
                printf("Entrer l'article à inserer :");
                viderChaine(articleInsert, MAX_ARTICLE_LENGTH);
                fgets(articleInsert, MAX_ARTICLE_LENGTH, stdin);
                if (articleInsert[0] == '\n') {
                    fgets(articleInsert, MAX_ARTICLE_LENGTH, stdin);
                }
                printf("Entrer sa cle primaire unique \n");
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
                printf("Entrer la clé de l'enregistrement a supprimer (4 charactere) :");
                scanf("%s", KeyChar);
                if (delete(&file, KeyChar, &zone)) {
                    printf("Supprime avec success \n");
                }
                if (continuE()) {
                    continue;
                }
                break;
            case 5: /// afficher le contenu d'un bloc du fichier
                printf("Entrer le numéro du bloc à afficher :");
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
            case 9: /// sauvgarde de l'entete
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
            case 10:
                quit = true;
                break;
            default:
                printf("Operation inexistante rechoisisez ... \n");
                continue;
        }
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
        }
    }
    Fermer(&file);
    return 0;
}