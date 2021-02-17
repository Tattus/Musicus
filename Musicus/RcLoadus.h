/* RcLoadus.h
   ----------

   Créé par : Tattus
   le : 13/04/2015

   Modifié le : 14/04/2015
   ----------
   Prototypes et structures des fonction de RcLoadus.c
   Peut nécessiter des bibliothèques tierces : SDL2, SDL2_images, SDL2_TTF, FMOD_STUDIO_LOWLEVEL.
   SDL2_images et SDL2_TTF nécéssite obligatoirement SDL2.
*/
/* Paramétrage */
// Commenter la ligne suivante si pas d'utilisation de FMOD_STUDIO_LOWLEVEL
#define DEF_RCFMODSTUDIOLOW

// Commenter la ligne suivante si pas d'utilisation de SDL2
#define DEF_RCSDL2

// Commenter la ligne suivante si pas d'utilisation de SDL2_Image
#define DEF_RCSDL2IMAGE

// Commenter la ligne suivante si pas d'utilisation de SDL2_ttf
#define DEF_RCSDL2TTF

/* Fin Paramétrage */

#ifndef DEF_RCLOADUS
#define DEF_RCLOADUS

// Includes :
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

// Defines :
#define RCLOADUSVERSION "1.0" /* Lecture des BMP en SDL2,
                                 Lecture des formats images de SDL2_image
                                 Lecture des polices de SDL2_TTF
                                 Lecture des sons par Fmod Studio Low
                                 Effacement des fichiers resources résiduels (NoDel) */

// Enumérations :
enum ErrorCode // Liste des codes d'erreurs
{
    RC_FILE_NOT_FOUND = 10,
    RC_FILE_EMPTY,
    RC_RESOURCE_UNKNOWN
};

// Prototypes :
char *getRcFile(char * a_pResName, int *a_pnSize); // Charge un fichier texte ou binaire en mémoire depuis un conteneur de ressources et renvoie un pointeur sur cette mémoire.
void setResourceContenerName(char *a_RcFileName); // Fonction qui permet d'indiquer le nom du fichier contenant les ressources envoyé en paramètre.
char *getResourceContenerName(void); // Fonction qui permet d'obtenir le nom du fichier contenant les ressources.
void setPrefix(char *a_pNewPrefix); // Fonction qui permet d'indiquer le préfixe au nom des fichiers ressources temporaires.
char *getPrefix(void); // Fonction qui permet d'obtenir le préfixe au nom des fichiers ressources temporaires.
int getErrorCode(void); // Fonction qui permet d'obtenir le code de l'erreur.
void closeRcFile(char *a_pResName); // Fonction qui supprime le fichier temporaire résiduel indiqué en paramètre. Supprime tous les fichiers temporaires résiduels en envoyant NULL en paramètres.

// Particularitées selon le paramétrage :
/* FMOD Studio Low activé */
#ifdef DEF_RCFMODSTUDIOLOW
    #include <FMODStudioLowLevel_1_09_04/fmod.h>
    FMOD_RESULT FMOD_System_CreateRcSound(FMOD_SYSTEM *a_system, const char *a_rcname, FMOD_MODE a_mode, FMOD_CREATESOUNDEXINFO *a_finfo, FMOD_SOUND **a_ppsound); // Charge un son provenant du conteneur de ressources via FMOD Studio Low. Elle renvoie FMOD_RESULT.
    FMOD_RESULT FMOD_System_CreateRcSoundNoDel(FMOD_SYSTEM *a_system, const char *a_rcname, FMOD_MODE a_mode, FMOD_CREATESOUNDEXINFO *a_finfo, FMOD_SOUND **a_ppsound); // Charge un son provenant du conteneur de ressources via FMOD Studio Low. Elle renvoie FMOD_RESULT. La fonction ne supprime pas le fichier temporaire créé pour la lecture.
#endif // DEF_RCFMODSTUDIOLOW

/* SDL2_TTF activé */
#ifdef DEF_RCSDL2TTF
    #ifndef DEF_RCSDL2
        #define DEF_RCSDL2
    #endif // DEF_RCSDL2

    #include <SDL2/SDL_ttf.h>
    TTF_Font *TTF_OpenRcFontNoDel(const char *a_FontName, int ptsize); // Charge une police provenant du conteneur de ressources via SDL_TTF. Elle renvoie la police chargée.
#endif // DEF_RCSDL2TTF

/* SDL2_image activé */
#ifdef DEF_RCSDL2IMAGE
    #ifndef DEF_RCSDL2
        #define DEF_RCSDL2
    #endif // DEF_RCSDL2

    #include <SDL2/SDL_image.h>
    SDL_Surface *IMG_RcLoad(const char *a_ImgName); // Charge une image provenant du conteneur de ressources via SDL2_image. Elle renvoie l'image chargée.
#endif // DEF_RCSDL2IMAGE

/* SDL2 activé */
#ifdef DEF_RCSDL2
    #include <SDL2/SDL.h>
    SDL_Surface *LoadRcBitmap(const char* a_BmpName); // Charge une image provenant du conteneur de ressources au format BMP par la SDL2. Elle renvoie l'image chargée.
#endif // DEF_RCSDL2

#endif // DEF_RCLOADUS
