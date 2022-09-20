/* lecture.h
   ---------
   Prototypes des fonctions de lecture.c

*/
#ifndef LECTURE_H_INCLUDED
#define LECTURE_H_INCLUDED

// Includes :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototypes :
void viderBuffer(void); // Fonction qui vide le buffer.
void viderBufferFichier(FILE *fichier); // Fonction qui finit de lire la ligne.
int lire(char *chaine, int longueur); // Fonction qui lit une chaîne de caractères saisie au clavier. Elle renvoie 1 si la fonction s'est bien dérouléé et 0 s'il y a eu une erreur.
int lireFichier(char *chaine, int longueur, FILE *fichier); // Fonction qui lit une chaîne de caractères dans un fichier. Elle renvoie 1 si la fonction s'est bien déroulée et 0 s'il y a eu une erreur.
long lireLong(void); // Fonction qui lit un nombre entier saisie au clavier. Elle renvoie le nombre entier sous forme de long.
long lireLongFichier(FILE *fichier); // Fonction qui lit un nombre entier dans un fichier. Elle renvoie le nombre entier sous forme de long.
double lireDouble(void); // Fonction qui lit un nombre décimal saisie au clavier(comprend le point ET la virgule). Elle renvoie un nombre décimal sous forme de double.
double lireDoubleFichier(FILE *fichier); // Fonction qui lit un nombre décimal dans un fichier (comprend le point ET la virgule). Elle renvoie un nombre décimal sous forme de double.
void copieVerif(char *copie, size_t tailleCopie, const char *source); // Fonction qui vérifie si la copie peut avoir lieu en comparant les tailles de la source et de la destination.

#endif // LECTURE_H_INCLUDED
