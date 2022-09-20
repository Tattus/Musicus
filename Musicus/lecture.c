/* lecture.c
   ---------
   Fonctions de lecture sécurisée de chaînes de caractères, nombres entiers et nombres décimaux saisis par l'utilisateur oû luent dans un fichier.

*/
#include "lecture.h"

/* Fonction qui vide le buffer. */
void viderBuffer(void)
{
    int c = 0;
    while (c != '\n' && c != EOF) // Lecture dans le buffer caractère par caractère jusqu'à ce que "l'Entrée" est récupéré ou le symbole EOF, qui signifient tous deux que l'on est arrivé à la fin du buffer.
    {
        c = getchar();
    }
}

/* Fonction qui finit de lire la ligne. */
void viderBufferFichier(FILE *fichier)
{
    int c = 0;
    while (c != '\n' && c != EOF) // Lecture dans le fichier caractère par caractère jusqu'à ce que "l'Entrée" est récupéré ou le symbole EOF, qui signifient tous deux que l'on est arrivé à la fin du buffer.
    {
        c = fgetc(fichier);
    }
}

/* Fonction qui lit une chaîne de caractères saisie au clavier. */
int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    // Lecture du texte saisi au clavier.
    if (fgets(chaine, longueur, stdin) != NULL) // Si la saisie se fait sans erreur.
    {
        positionEntree = strchr(chaine, '\n'); // Recherche du caractère "Entrée" (Retour à la ligne).
        if (positionEntree != NULL) // Si le retour à la ligne a été trouvé.
        {
            *positionEntree = '\0'; // Le retour à la ligne est remplacé par le caractère de fin de chaîne (\0).
        }
        else // Si le caractère \n n'a pas été trouvé (la saisie est trop longue).
        {
            viderBuffer(); // On vide le buffer du résidue de la chaîne trop longue.
        }
        return 1; // La fonction renvoie 1 si elle s'est déroulée sans erreur.
    }
    else // S'il y a eu une erreur à la saisie.
    {
        viderBuffer(); // On vide le buffer par sécurité.
        return 0; // La fonction renvoie 0 s'il y a eu une erreur.
    }
}

/* Fonction qui lit une chaîne de caractères dans un fichier. */
int lireFichier(char *chaine, int longueur, FILE *fichier) //
{
    char *retourLigne = NULL;

    // Lecture du texte saisi au clavier.
    if (fgets(chaine, longueur, fichier) != NULL) // S'il n'y a pas d'erreur.
    {
        retourLigne = strchr(chaine, '\n'); // Recherche du caractère "Retour à la ligne".
        if (retourLigne != NULL) // Si le retour à la ligne a été trouvé.
        {
            *retourLigne = '\0'; // Le retour à la ligne est remplacé par le caractère de fin de chaîne (\0).
        }
        else
        {
            viderBufferFichier(fichier); // Le reste de la ligne est lue.
        }
        return 1; // La fonction renvoie 1 si elle s'est déroulée sans erreur.
    }
    else // S'il y a eu une erreur.
    {
        viderBufferFichier(fichier); // Le reste de la ligne est lue
        return 0; // La fonction renvoie 0 s'il y a eu une erreur.
    }
}

/* Fonction qui lit un nombre entier saisie au clavier. */
long lireLong(void)
{
    char nombreTexte[100] = {0}; // Création d'un tableau de char de 100 cases (100 pour qu'il y ai de la marge dans l'écriture d'un nombre).

    if (lire(nombreTexte, 100)) // Si la lecture du texte est correcte.
    {
        return strtol(nombreTexte, NULL, 10); // Conversion du nombre en long. Le long est renvoyé.
    }
    else // Sinon, 0 est renvoyé.
    {
        return 0;
    }
}

/* Fonction qui lit un nombre entier dans un fichier. */
long lireLongFichier(FILE *fichier)
{
    char nombreTexte[100] = {0}; // Création d'un tableau de char de 100 cases (100 pour qu'il y ai de la marge dans la lecture d'un nombre).

    if (lireFichier(nombreTexte, 100, fichier)) // Si la lecture du texte est correcte.
    {
        return strtol(nombreTexte, NULL, 10); // Conversion du nombre en long. Le long est renvoyé.
    }
    else // Sinon, 0 est renvoyé.
    {
        return 0;
    }
}

/* Fonction qui lit un nombre décimal saisie au clavier. */
double lireDouble(void)
{
    char decimalTexte[100] = {0}; // Création d'un tableau de char de 100 cases (100 pour qu'il y ai de la marge dans l'écriture d'un nombre décimal).
    char *virgule = NULL; // Création d'un pointeur qui va pointer sur l'adresse de la virgule d'un nombre décimal.

    if (lire(decimalTexte, 100)) // Si la lecture du texte est correcte.
    {
        virgule = strchr(decimalTexte, ','); // Recherche de la présence d'une virgule.
        if (virgule != NULL) // Si une virgule est trouvée
        {
            *virgule = '.'; // Remplacement de la virgule par un "point" décimal.
        }
        return strtod(decimalTexte, NULL); // Conversion du nombre en double. Le double est renvoyé.
    }
    else // Sinon, 0 est renvoyé.
    {
        return 0;
    }
}

/* Fonction qui lit un nombre décimal dans un fichier. */
double lireDoubleFichier(FILE *fichier)
{
    char decimalTexte[100] = {0}; // Création d'un tableau de char de 100 cases (100 pour qu'il y ai de la marge dans la lecture d'un nombre décimal).
    char *virgule = NULL; // Création d'un pointeur qui va pointer sur l'adresse de la virgule d'un nombre décimal.

    if (lireFichier(decimalTexte, 100, fichier)) // Si la lecture du texte est correcte.
    {
        virgule = strchr(decimalTexte, ','); // Recherche de la présence d'une virgule.
        if (virgule != NULL) // Si une virgule est trouvée
        {
            *virgule = '.'; // Remplacement de la virgule par un "point" décimal.
        }
        return strtod(decimalTexte, NULL); // Conversion du nombre en double. Le double est renvoyé.
    }
    else // Sinon, 0 est renvoyé.
    {
        return 0;
    }
}

/* Fonction qui vérifie une copie de chaîne de caractères. */
void copieVerif(char *copie, size_t tailleCopie, const char *source)
{
    if (strlen(source) + 1 > tailleCopie) // Si la longueur de la source +1 (pour compter le caracère '\0' est supérieur à la taille totale de la copie, la copie ne peut avoir lieu.
    {
        printf("Erreur dans la copie de la chaine. Le programme va fermer.");
        exit(0);
    }
    strcpy(copie, source);
}
