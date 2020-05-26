/* main.c
   ------

   Créé par : Tattus
   le : 06/04/2015

   Modifié le : 16/04/2015
   ------
   Fonction principale.

*/
#define SDL_MAIN_HANDLED
#include "general.h"

int main(int argc, char** argv)
{
    SDL_SetMainReady();

    Base contexte;
    memset(&contexte, 0x00, sizeof(Base));

    /* Initialisation */
    srand(time(NULL));
    initialisationGenerale(&contexte);

    /* Corps */
    menuPrincipale(&contexte);

    /* Fermeture */
    fermetureGenerale(&contexte, 0);

    return EXIT_SUCCESS;
}
