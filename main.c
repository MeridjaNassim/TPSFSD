#include "machineAbstraite.h"

int key = 1;
int main() {

    char fileName[] = "data/read.txt";
    FICHIER file1;
    file1.filePtr = NULL;
    file1.entete.modified = false;
    file1.entete.numberBlocs = 10;


    Ouvrir(fileName, &file1, _ANCIEN);

    char *Key = genKey(&key);
    char buff[MAX_ARTICLE_LENGTH];
    char article[] = "Hello";
    char *length = getLength(strlen(article));

    puts(buildString(length, VALID, Key, article));

    Fermer(&file1);
    return 0;
}