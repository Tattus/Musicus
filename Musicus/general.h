/* general.h
   ---------

   Créé par : Tattus
   le : 10/04/2015

   Modifié le : 09/08/2016
   ---------
   Structures, Constantes de préprocesseur et prototypes des fonctions du projet.

*/
#ifndef DEF_GENERAL
#define DEF_GENERAL

// Defines de l'OS -- Décommenter la ligne adéquate.
#define WINDOWS
//#define LINUX
//#define MAC

// Includes :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef LINUX
	#include <sys/stat.h>
#endif
#include <FMODStudioLowLevel/fmod.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_messagebox.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "lecture.h"
#include "RcLoadus.h"
#include "collisions.h"

/// Defines :
// Defines globaux :
#define TITRE_MSG_ERREUR_ENG "Error !"
#define MESSAGE_ERREUR_ENG "The software will close.\nFor more details, check the failure file in the folder Error_Report"
#define TITRE_MSG_ERREUR_FRA "Erreur !"
#define MESSAGE_ERREUR_FRA "Le programme va fermer.\nPour plus de détail, consultez le fichier d'erreur dans le dossier Error_Report"
#define VERSION "1.1"
#define FALSE 0
#define TRUE 1
#define XRES 1400
#define YRES 800
#define TAILLE_POLICE_MINI 20
#define TAILLE_POLICE_PETIT 40
#define TAILLE_POLICE_MOYEN 60
#define TAILLE_POLICE_GRAND 100

/// Enumérations :
// Enumérations globales :
enum Langue // Langue du jeu.
{
    ENG,
    FRA
};

enum Taille_Texte // Tailles du texte.
{
    TAILLE_TEXTE_MINI,
    TAILLE_TEXTE_PETIT,
    TAILLE_TEXTE_MOYEN,
    TAILLE_TEXTE_GRAND
};

/// Structures :
// Structures globales :
typedef struct Base Base; // Contexte générale.
struct Base
{
    FILE *my_stderr;
    char errorPath[300];
    FMOD_RESULT verifFMOD;
    FMOD_SYSTEM *system;
    SDL_Window *fenetre;
    SDL_Renderer *rendu;
    int langue;
    TTF_Font *policeGrand;
    TTF_Font *policeMoyen;
    TTF_Font *policePetit;
    TTF_Font *policeMini;
    FMOD_CHANNELGROUP *canalGroupSon;
    //FMOD_CHANNELGROUP *canalGroupMusique;
    //FMOD_CHANNELGROUP *canalGroupVoix;
    FMOD_CHANNEL *canalSon;
    //FMOD_CHANNEL *canalMusique;
    //FMOD_CHANNEL *canalVoix;
};

typedef struct Compteur Compteur; // Compteur de temps.
struct Compteur
{
    long tempsTotal; // Temps total passé.
    long tempsEntreDeux; // Temps passé à partir du dernier temps précédent.
    long tempsPrecedent; // Temps du dernier tour de boucle.
};

/// Prototypes :
#ifdef FILUS_LINUX
    // Prototype mkdir :
    int mkdir(const char *path, mode_t mode);
#endif

// Prototypes startend.c :
void initialisationGenerale(Base *contexte); // Initialisation générale. Appelle les initialisations de base.
void fermetureGenerale(Base *contexte, int codeFermeture); // Fermeture générale. Appelle les fermetures.
void initialisationRedirectErreur(Base *contexte); // Initialisation de la redirection d'erreur.
void initialisationSDL(Base *contexte); // Initialisation de la SDL.
void initialisationFMOD(Base *contexte); // Initialisation de FMOD (création du système et initialisation).
void initialisationSDLTTF(Base *contexte); // Initialisation de SDL_TTF.
void fermetureSDLTTF(Base *contexte, int codeFermeture); // Fermeture de SDL_TTF.
void fermetureFMODInter(Base *contexte, int codeFermeture); // Fermeture intermédiaire de FMOD.
void fermetureFMOD(Base *contexte, int codeFermeture); // Fermeture de FMOD.
void fermetureSDL(Base *contexte, int codeFermeture); // Fermeture de la SDL.
void fermetureRedirectErreur(Base *contexte, int codeFermeture); // Fermeture de la redirection d'erreur et du programme. Si le fichier d'erreur est vide, il est supprimé.
void initialisationFenetre(Base *contexte); // Initialisation de la fenetre.
void initialisationRendu(Base *contexte); // Initialisation du rendu.
void initialisationPolice(Base *contexte); // Initialisation des polices.
void initialisationCanal(Base *contexte); // Initialisation des canaux.
void initialisationLangue(Base *contexte); // Initialisation de la langue.
void fermetureRendu(Base *contexte); // Fermeture du rendu.
void fermetureFenetre(Base *contexte); // Fermeture de la fenetre.
void fermeturePolice(Base *contexte, TTF_Font *police); // Fermeture des polices.
void fermetureSurface(Base *contexte, SDL_Surface *surface); // Ferme la surface reçue.
void fermetureTexture(Base *contexte, SDL_Texture *texture); // Ferme la texture reçue.
void fermetureGroupeCanaux(Base *contexte, FMOD_CHANNELGROUP *canalGroup); // Ferme le canal ou groupe de canaux reçu.
void fermetureSon(Base *contexte, FMOD_SOUND *son); // Ferme le son reçu.

// Prototypes menu.c
void menuPrincipale(Base *contexte); // Fonction du menu principale.

// Prototypes jeu.c
void jeuStart(Base *contexte, int nbNotes); // Fonction de départ du jeu.

// Prototypes credit.c
void initialisationCredits(); // Fonction qui lance la création des fichiers de licences.

#endif // DEF_GENERAL
