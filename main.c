#include "features.h"

int _KEY_ = 1;

bool delete(FICHIER *fichier, char *key) {
    /// fonction qui supprime logiquement un enregistrement dans le fichier a partir de sa clé
    int bloc = 0;
    int pos = 0;
    bool found = false;
    Search(fichier, key, &bloc, &pos,
           &found); /// on recherche la clé dans le fichier ==> si trouvé alors on a le bloc et la position dans le bloc
    if (found) {
        Buffer *buffer = &((*fichier).buffR);
        viderBuffer(buffer); /// on vide le buffer pour ne pas avoir des erreurs
        if (pos >=
            1) { /// si la position est supérieur a 1 donc le char effécé est dans le meme bloc à la position pos -1
            lireDir(fichier, bloc, buffer);
            buffer->Record[pos - 1] = 'D'; /// on met a jour le char effacé
            EcrireDir(fichier, bloc, buffer);
        } else { /// sinon le char effacé se trouve dans le bloc précédant à la derniere position
            lireDir(fichier, bloc - 1, buffer);
            buffer->Record[MAX_BLOC_LENGTH - 1] = 'D';
            EcrireDir(fichier, bloc - 1, buffer);
        }
        return true; /// suppression avec succée;
    }
    return false; /// on a pas trouvé la clé donc on supprime rien
}
//bool ConvertEnteteToString()
int main() {

    char fileName[] = "data/read.txt";
    FICHIER file1;
    file1.filePtr = NULL;


    Ouvrir(fileName, &file1, _ANCIEN);
    int bloc = 0;
    int pos = 0;
    bool found = false;
    file1.entete.numberBlocs = 1;
    file1.entete.lastKey = 1;

    Search(&file1, "0005", &bloc, &pos, &found);


    Fermer(&file1);


    return 0;
}