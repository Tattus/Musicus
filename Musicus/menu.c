/* menu.c
   ------

   Créé par : Tattus
   le : 26/07/2016

   Modifié le : 03/08/2016
   ----------
   Fonctions des menus.

*/
#include "general.h"

/* Prototypes locaux */
int majTextesMenuPrincipale(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes,int nbNotes); // Mise à jour des textes du menu principale.
SDL_Texture *texteMenuPrincipale(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte, int nbNotes); // Fonction qui charge les textures de textes du menu principale.
void menuCredit(Base *contexte); // Fonction du menu crédit.
int majTextesMenuCredit(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes); // Mise à jour des textes du menu crédit.
SDL_Texture *texteMenuCredit(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte); // Fonction qui charge les textures de textes du menu crédit.
void fermetureMenu(Base *contexte, SDL_Texture* *texteMenu, int nbTextes); // Fonction qui ferme le menu principale.
/* Fin Prototypes locaux */

void menuPrincipale(Base *contexte)
{
    /* Variables */
    // Générales
    int i;
    int continuer = TRUE;
    int verifFonction = 0;
    // Contrôles
    SDL_Event event;
    const Uint8* etatClavier = SDL_GetKeyboardState(NULL);
    int mouseX = 0;
    int mouseY = 0;
    Uint32 mouse = SDL_GetMouseState(&mouseX, &mouseY);
    int mouseStart = TRUE;
    if (mouse == SDL_BUTTON_LEFT)
    {
        mouseStart = FALSE;
    }
    // Eléments du menu
    int nbNotes = 1;
    int nbTextes = 7;
    SDL_Texture *textes[7] = {NULL};
    SDL_Rect positionTexte[7] = {{0, 0, 0, 0}};
    /* Fin variables */

    /* Premier affichage du menu */
    // Préparation
    verifFonction = majTextesMenuPrincipale(contexte, textes, positionTexte, nbTextes, nbNotes);
    if (verifFonction == 1)
    {
        /* Fermeture du menu */
        fermetureMenu(contexte, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }

    // Affichage
    SDL_RenderClear(contexte->rendu);
    for (i = 0 ; i < nbTextes ; i++)
    {
        SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
    }
    SDL_RenderPresent(contexte->rendu);

    /* Contrôles */
    while (continuer)
    {
        SDL_WaitEvent(&event);

        /// Traitement des évènements
        // Quitter
        if (event.type == SDL_QUIT || etatClavier[SDL_SCANCODE_ESCAPE])
        {
            /* Fermeture du menu */
            fermetureMenu(contexte, textes, nbTextes);
            /* Fermeture du programme */
            fermetureGenerale(contexte, 0);
        }

        // Mise à jour de l'état de la souris
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        // Clic de la souris
        switch(event.type)
        {
            case SDL_MOUSEBUTTONUP:
                if (mouseStart == FALSE)
                {
                    mouseStart = TRUE;
                }
                else
                {
                    if (collisionPoint_AABB(mouseX, mouseY, positionTexte[1])) // Clic sur "Une note"
                    {
                        nbNotes = 1;
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[2])) // Clic sur "Deux note"
                    {
                        nbNotes = 2;
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[3])) // Clic sur "Trois note"
                    {
                        nbNotes = 3;
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[4])) // Clic sur "Jouer"
                    {
                        jeuStart(contexte, nbNotes);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[5])) // Clic sur "Crédits"
                    {
                        /* Fermeture du menu Principale */
                        fermetureMenu(contexte, textes, nbTextes);
                        /* Ouverture du menu Crédit */
                        menuCredit(contexte);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[6])) // Clic sur "Quitter"
                    {
                        /* Fermeture du menu */
                        fermetureMenu(contexte, textes, nbTextes);
                        /* Fermeture du programme */
                        fermetureGenerale(contexte, 0);
                    }
                }
                break;
            default:
                break;
        }

        // Préparations du menu
        verifFonction = majTextesMenuPrincipale(contexte, textes, positionTexte, nbTextes, nbNotes);
        if (verifFonction == 1)
        {
            /* Fermeture du menu */
            fermetureMenu(contexte, textes, nbTextes);
            /* Fermeture du programme */
            fermetureGenerale(contexte, 1);
        }

        // Affichage
        SDL_RenderClear(contexte->rendu);
        for (i = 0 ; i < nbTextes ; i++)
        {
            SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
        }
        SDL_RenderPresent(contexte->rendu);
    }
}

int majTextesMenuPrincipale(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes, int nbNotes)
{
    int i;

    for (i = 0 ; i < nbTextes ; i++)
    {
        if (textes[i] != NULL)
        {
            fermetureTexture(contexte, textes[i]);
        }
    }

    for (i = 0 ; i < nbTextes ; i++)
    {
        switch(i)
        {
            case 0:
                textes[i] = texteMenuPrincipale(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND, nbNotes);
                break;
            case 4:
                textes[i] = texteMenuPrincipale(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND, nbNotes);
                break;
            case 5:
                textes[i] = texteMenuPrincipale(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND, nbNotes);
                break;
            case 6:
                textes[i] = texteMenuPrincipale(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND, nbNotes);
                break;
            default:
                textes[i] = texteMenuPrincipale(contexte, &positionTexte[i], i, TAILLE_TEXTE_MOYEN, nbNotes);
                break;
        }
        if (textes[i] == NULL) {return 1;}
    }

    return 0;
}

SDL_Texture *texteMenuPrincipale(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte, int nbNotes)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *textureTexte = NULL;
    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Color couleurbg = {0, 0, 0};
    char texte[50] = "";

    /* Préparation du texte */
    switch(numTexte)
    {
        case 0:
            snprintf(texte, 50, "Musicus");
            break;
        case 1:
            if (nbNotes == 1)
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[X] Single note");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[X] Une note");
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[   ] Single note");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[   ] Une note");
                        break;
                    default:
                        break;
                }
            }
            break;
        case 2:
            if (nbNotes == 2)
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[X] Two notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[X] Deux notes");
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[   ] Two notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[   ] Deux notes");
                        break;
                    default:
                        break;
                }
            }
            break;
        case 3:
            if (nbNotes == 3)
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[X] Three notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[X] Trois notes");
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[   ] Three notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[   ] Trois notes");
                        break;
                    default:
                        break;
                }
            }
            break;
        case 4:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Play");
                    break;
                case FRA:
                    snprintf(texte, 50, "Jouer");
                    break;
                default:
                    break;
            }
            break;
        case 5:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Credits");
                    break;
                case FRA:
                    snprintf(texte, 50, "Crédits");
                    break;
                default:
                    break;
            }
            break;
        case 6:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Leave");
                    break;
                case FRA:
                    snprintf(texte, 50, "Quitter");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    /* Création de la texture */
    switch(tailleTexte)
    {
        case TAILLE_TEXTE_MINI:
            surface = TTF_RenderUTF8_Shaded(contexte->policeMini, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_PETIT:
            surface = TTF_RenderUTF8_Shaded(contexte->policePetit, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_MOYEN:
            surface = TTF_RenderUTF8_Shaded(contexte->policeMoyen, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_GRAND:
            surface = TTF_RenderUTF8_Shaded(contexte->policeGrand, texte, couleurTexte, couleurbg);
            break;
        default:
            break;
    }

    if(surface == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Picture loading failure. %s \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de l'image. %s \n", SDL_GetError());
                break;
            default:
                break;
        }
        return NULL;
    }
    textureTexte = SDL_CreateTextureFromSurface(contexte->rendu, surface);
    if (textureTexte == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Texture creation failure. %s \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec de la création de la texture. %s \n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureSurface(contexte, surface);
        return NULL;
    }

    // Position du texte.
    switch(numTexte)
    {
        case 0:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = 100;
            break;
        case 1:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - 175;
            positionTexte->y = 250 + surface->h * numTexte + 5 * numTexte;
            break;
        case 2:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - 175;
            positionTexte->y = 250 + surface->h * numTexte + 5 * numTexte;
            break;
        case 3:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - 175;
            positionTexte->y = 250 + surface->h * numTexte + 5 * numTexte;
            break;
        case 4:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES - 150;
            break;
        case 5:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 50;
            positionTexte->y = YRES - 150;
            break;
        case 6:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES - 50 - surface->w;
            positionTexte->y = YRES - 150;
            break;
        default:
            break;
    }

    fermetureSurface(contexte, surface);

    return textureTexte;
}

void menuCredit(Base *contexte)
{
    /* Variables */
    // Générales
    int i;
    int continuer = TRUE;
    int verifFonction = 0;
    // Contrôles
    SDL_Event event;
    const Uint8* etatClavier = SDL_GetKeyboardState(NULL);
    int mouseX = 0;
    int mouseY = 0;
    Uint32 mouse = SDL_GetMouseState(&mouseX, &mouseY);
    int mouseStart = TRUE;
    if (mouse == SDL_BUTTON_LEFT)
    {
        mouseStart = FALSE;
    }
    // Eléments du menu
    int nbTextes = 6;
    SDL_Texture *textes[6] = {NULL};
    SDL_Rect positionTexte[6] = {{0, 0, 0, 0}};
    /* Fin variables */

    /* Premier affichage du menu */
    // Préparation
    verifFonction = majTextesMenuCredit(contexte, textes, positionTexte, nbTextes);
    if (verifFonction == 1)
    {
        /* Fermeture du menu */
        fermetureMenu(contexte, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }

    // Affichage
    SDL_RenderClear(contexte->rendu);
    for (i = 0 ; i < nbTextes ; i++)
    {
        SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
    }
    SDL_RenderPresent(contexte->rendu);

    /* Contrôles */
    while (continuer)
    {
        SDL_WaitEvent(&event);

        /// Traitement des évènements
        // Quitter
        if (event.type == SDL_QUIT)
        {
            /* Fermeture du menu */
            fermetureMenu(contexte, textes, nbTextes);
            /* Fermeture du programme */
            fermetureGenerale(contexte, 0);
        }
        // Retour
        if (etatClavier[SDL_SCANCODE_ESCAPE])
        {
            /* Fermeture du menu Crédit */
            fermetureMenu(contexte, textes, nbTextes);
            /* Ouverture du menu Principale */
            menuPrincipale(contexte);
        }

        // Mise à jour de l'état de la souris
        mouse = SDL_GetMouseState(&mouseX, &mouseY);
        // Clic de la souris
        switch(event.type)
        {
            case SDL_MOUSEBUTTONUP:
                if (mouseStart == FALSE)
                {
                    mouseStart = TRUE;
                }
                else
                {
                    if (collisionPoint_AABB(mouseX, mouseY, positionTexte[5])) // Clic sur "Retour"
                    {

                        /* Fermeture du menu Crédit */
                        fermetureMenu(contexte, textes, nbTextes);
                        /* Ouverture du menu Principale */
                        menuPrincipale(contexte);
                    }
                }
                break;
            default:
                break;
        }
    }
}

int majTextesMenuCredit(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes)
{
    int i;

    for (i = 0 ; i < nbTextes ; i++)
    {
        if (textes[i] != NULL)
        {
            fermetureTexture(contexte, textes[i]);
        }
    }

    for (i = 0 ; i < nbTextes ; i++)
    {
        switch(i)
        {
            case 0:
                textes[i] = texteMenuCredit(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND);
                break;
            case 3:
                textes[i] = texteMenuCredit(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT);
                break;
            case 4:
                textes[i] = texteMenuCredit(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT);
                break;
            case 5:
                textes[i] = texteMenuCredit(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND);
                break;
            default:
                textes[i] = texteMenuCredit(contexte, &positionTexte[i], i, TAILLE_TEXTE_MOYEN);
                break;
        }
        if (textes[i] == NULL) {return 1;}
    }

    return 0;
}

SDL_Texture *texteMenuCredit(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *textureTexte = NULL;
    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Color couleurbg = {0, 0, 0};
    char texte[100] = "";

    /* Préparation du texte */
    switch(numTexte)
    {
        case 0:
            snprintf(texte, 100, "Musicus");
            break;
        case 1:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Credits");
                    break;
                case FRA:
                    snprintf(texte, 50, "Crédits");
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Musicus's version : %s", VERSION);
                    break;
                case FRA:
                    snprintf(texte, 50, "Version de Musicus : %s", VERSION);
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 100, "This game has been created by Tattus");
                    break;
                case FRA:
                    snprintf(texte, 100, "Ce jeu a été créé par Tattus");
                    break;
                default:
                    break;
            }
            break;
        case 4:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 100, "Free redistribution without modifications of Musicus v%s is allowed", VERSION);
                    break;
                case FRA:
                    snprintf(texte, 100, "La redistribution gratuite et sans modifications de Musicus v%s est autorisée.", VERSION);
                    break;
                default:
                    break;
            }
            break;
        case 5:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Back");
                    break;
                case FRA:
                    snprintf(texte, 50, "Retour");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }


    /* Création de la texture */
    switch(tailleTexte)
    {
        case TAILLE_TEXTE_MINI:
            surface = TTF_RenderUTF8_Shaded(contexte->policeMini, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_PETIT:
            surface = TTF_RenderUTF8_Shaded(contexte->policePetit, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_MOYEN:
            surface = TTF_RenderUTF8_Shaded(contexte->policeMoyen, texte, couleurTexte, couleurbg);
            break;
        case TAILLE_TEXTE_GRAND:
            surface = TTF_RenderUTF8_Shaded(contexte->policeGrand, texte, couleurTexte, couleurbg);
            break;
        default:
            break;
    }

    if(surface == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Picture loading failure. %s \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de l'image. %s \n", SDL_GetError());
                break;
            default:
                break;
        }
        return NULL;
    }
    textureTexte = SDL_CreateTextureFromSurface(contexte->rendu, surface);
    if (textureTexte == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Texture creation failure. %s \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec de la création de la texture. %s \n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureSurface(contexte, surface);
        return NULL;
    }

    // Position du texte.
    switch(numTexte)
    {
        case 0:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = 100;
            break;
        case 1:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = 250;
            break;
        case 2:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = 250 + surface->h + 10;
            break;
        case 3:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES / 2 - surface->h + 100;
            break;
        case 4:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES / 2 + surface->h + 100;
            break;
        case 5:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES - 150;
            break;
        default:
            break;
    }

    fermetureSurface(contexte, surface);

    return textureTexte;
}

void fermetureMenu(Base *contexte, SDL_Texture* *texteMenu, int nbTextes)
{
    int i;

    for (i = 0 ; i < nbTextes ; i++)
    {
        if (texteMenu[i] != NULL)
        {
            fermetureTexture(contexte, texteMenu[i]);
        }
    }
}
