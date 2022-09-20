/* jeu.c
   -----
   Fonctions du jeu.

*/
#include "general.h"

/* Structures locales */
typedef struct scoreComplet scoreComplet; // Tous les types de scores.
struct scoreComplet
{
    int n1Score;
    int n2Score;
    int n3Score;
};
/* Fin structures locales */

/* Prototypes locaux */
int selectSonPiano(Base *contexte, int nbNotes); // Fonction qui sélectionne le son a trouver.
void verificationNotes(Base *contexte, int sonATrouver, int nbNotes, int noteUne, int noteDeux, int noteTrois, int *noteUneCorr, int *noteDeuxCorr, int *noteTroisCorr, int *reussite, int *erreur); // Fonction qui vérifie le choix de l'utilisateur.
int majTextesJeu(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes, int nbNotes, int afficherNotes, int score, scoreComplet bestScore); // Mise à jour des textes du jeu.
SDL_Texture *texteJeu(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte, int nbNotes, int afficherNotes, int score, scoreComplet bestScore); // Fonction qui charge les textures de textes du jeu.
int chargementBestScore(Base *contexte, int nbNotes, scoreComplet *bestScore); // Fonction qui charge le meilleur score.
int sauvegardeBestScore(Base *contexte, int nbNotes, const scoreComplet bestScore); // Fonction qui sauvegarde le meilleur score.
int chargementJeu(Base *contexte, SDL_Texture* *touchePiano); // Fonction qui charge les images du jeu.
int chargementSon(Base *contexte, FMOD_SOUND* *sonPiano, int sonATrouver); // Fonction qui charge un son donné du jeu.
SDL_Texture *initTextureJeuRessources(Base *contexte, char *nomImage); // Fonction qui charge une texture à partir d'un fichier de ressources.
FMOD_SOUND *initSonJeuRessources(Base *contexte, char *nomSon); // Fonction qui charge un son à partir d'un fichier de ressources.
void fermetureJeu(Base *contexte, SDL_Texture* *touchePiano, FMOD_SOUND* *sonPiano, SDL_Texture* *texte, int nbTextes); //Fonction qui ferme le jeu.
/* Fin Prototypes locaux */

void jeuStart(Base *contexte, int nbNotes)
{
    /* Variables */
    // Générales
    int i;
    int continuer = TRUE;
    int verifFonction = 0;
    // Textes
    int afficherNotes = FALSE;
    int nbTextes = 17;
    SDL_Texture *textes[17] = {NULL};
    SDL_Rect positionTexte[17] = {{0, 0, 0, 0}};
    // Ressources
    SDL_Texture *touchePiano[12] = {NULL};
    SDL_Rect positionNotes[12] = {{525, YRES / 2, 50, 200}, {560, YRES / 2, 30, 125}, {575, YRES / 2, 50, 200}, {610, YRES / 2, 30, 125}, {625, YRES / 2, 50, 200},
                                    {675, YRES / 2, 50, 200}, {710, YRES / 2, 30, 125}, {725, YRES / 2, 50, 200}, {760, YRES / 2, 30, 125},
                                    {775, YRES / 2, 50, 200}, {810, YRES / 2, 30, 125}, {825, YRES / 2, 50, 200}};

    FMOD_SOUND *sonPiano[1] = {NULL};
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
    // Jeu
    int erreur = FALSE;
    int reussite =FALSE;
    int correction = FALSE;
    int noteUne = FALSE;
    int noteDeux = FALSE;
    int noteTrois = FALSE;
    int noteUneValeur = -1;
    int noteDeuxValeur = -1;
    int noteTroisValeur = -1;
    int noteUneValeurCorr = -1;
    int noteDeuxValeurCorr = -1;
    int noteTroisValeurCorr = -1;
    int score = 0;
    scoreComplet bestScore = {0, 0, 0};
    int sonATrouver = -1;
    /* Fin variables */

    /* Chargement */
    verifFonction = chargementBestScore(contexte, nbNotes, &bestScore);
    if (verifFonction == 1)
    {
        /* Fermeture du jeu */
        fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }
    verifFonction = chargementJeu(contexte, touchePiano);
    if (verifFonction == 1)
    {
        /* Fermeture du jeu */
        fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }

    /* Premier affichage du jeu */
    // Préparation
    verifFonction = majTextesJeu(contexte, textes, positionTexte, nbTextes, nbNotes,afficherNotes, score, bestScore);
    if (verifFonction == 1)
    {
        /* Fermeture du jeu */
        fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }

    // Affichage
    SDL_RenderClear(contexte->rendu);
    for (i = 0 ; i < 8 ; i++)
    {
        SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
    }
    /// Notes
    // DO
    if (noteUneValeur == 0 || noteDeuxValeur == 0 || noteTroisValeur == 0)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[1], NULL, &positionNotes[0]);
    }
    else if (noteUneValeur == 12 || noteDeuxValeur == 12 || noteTroisValeur == 12)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[0]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[0]);
    }
    // DO#
    if (noteUneValeur == 1 || noteDeuxValeur == 1 || noteTroisValeur == 1)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[1]);
    }
    else if (noteUneValeur == 13 || noteDeuxValeur == 13 || noteTroisValeur == 13)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[1]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[1]);
    }
    // RE
    if (noteUneValeur == 2 || noteDeuxValeur == 2 || noteTroisValeur == 2)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[2]);
    }
    else if (noteUneValeur == 14 || noteDeuxValeur == 14 || noteTroisValeur == 14)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[2]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[2]);
    }
    // RE#
    if (noteUneValeur == 3 || noteDeuxValeur == 3 || noteTroisValeur == 3)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[3]);
    }
    else if (noteUneValeur == 15 || noteDeuxValeur == 15 || noteTroisValeur == 15)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[3]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[3]);
    }
    // MI
    if (noteUneValeur == 4 || noteDeuxValeur == 4 || noteTroisValeur == 4)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[7], NULL, &positionNotes[4]);
    }
    else if (noteUneValeur == 16 || noteDeuxValeur == 16 || noteTroisValeur == 16)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[4]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[4]);
    }
    // FA
    if (noteUneValeur == 5 || noteDeuxValeur == 5 || noteTroisValeur == 5)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[1], NULL, &positionNotes[5]);
    }
    else if (noteUneValeur == 17 || noteDeuxValeur == 17 || noteTroisValeur == 17)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[5]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[5]);
    }
    // FA#
    if (noteUneValeur == 6 || noteDeuxValeur == 6 || noteTroisValeur == 6)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[6]);
    }
    else if (noteUneValeur == 18 || noteDeuxValeur == 18 || noteTroisValeur == 18)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[6]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[6]);
    }
    // SOL
    if (noteUneValeur == 7 || noteDeuxValeur == 7 || noteTroisValeur == 7)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[7]);
    }
    else if (noteUneValeur == 19 || noteDeuxValeur == 19 || noteTroisValeur == 19)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[7]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[7]);
    }
    // SOL#
    if (noteUneValeur == 8 || noteDeuxValeur == 8 || noteTroisValeur == 8)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[8]);
    }
    else if (noteUneValeur == 20 || noteDeuxValeur == 20 || noteTroisValeur == 20)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[8]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[8]);
    }
    // LA
    if (noteUneValeur == 9 || noteDeuxValeur == 9 || noteTroisValeur == 9)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[9]);
    }
    else if (noteUneValeur == 21 || noteDeuxValeur == 21 || noteTroisValeur == 21)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[9]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[9]);
    }
    // LA#
    if (noteUneValeur == 10 || noteDeuxValeur == 10 || noteTroisValeur == 10)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[10]);
    }
    else if (noteUneValeur == 22 || noteDeuxValeur == 22 || noteTroisValeur == 22)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[10]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[10]);
    }
    // SI
    if (noteUneValeur == 11 || noteDeuxValeur == 11 || noteTroisValeur == 11)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[7], NULL, &positionNotes[11]);
    }
    else if (noteUneValeur == 23 || noteDeuxValeur == 23 || noteTroisValeur == 23)
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[11]);
    }
    else
    {
        SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[11]);
    }
    if (afficherNotes == TRUE)
    {
        for (i = 8 ; i < 15 ; i++)
        {
            SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
        }
    }
    SDL_RenderPresent(contexte->rendu);

    /* Premier son a trouver */
    sonATrouver = selectSonPiano(contexte, nbNotes);
    verifFonction = chargementSon(contexte, sonPiano, sonATrouver);
    if (verifFonction == 1)
    {
        /* Fermeture du jeu */
        fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
        /* Fermeture du programme */
        fermetureGenerale(contexte, 1);
    }
    FMOD_System_PlaySound(contexte->system, sonPiano[0], NULL, 0, &contexte->canalSon);
    FMOD_Channel_SetChannelGroup(contexte->canalSon, contexte->canalGroupSon);

    /* Contrôles */
    while (continuer)
    {
        SDL_WaitEvent(&event);

        /// Traitement des évènements
        // Quitter
        if (event.type == SDL_QUIT)
        {
            /* Fermeture du jeu */
            fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
            /* Fermeture du programme */
            fermetureGenerale(contexte, 0);
        }

        // Retour au menu
        if (etatClavier[SDL_SCANCODE_ESCAPE])
        {
            /* Fermeture du jeu */
            fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
            /* Retour au menu */
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
                    if (collisionPoint_AABB(mouseX, mouseY, positionTexte[4])) // Clic sur "Afficher note"
                    {
                        if (afficherNotes == FALSE)
                        {
                            afficherNotes = TRUE;
                        }
                        else
                        {
                            afficherNotes = FALSE;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[5]) && erreur == FALSE && reussite == FALSE) // Clic sur "Rejouer note"
                    {
                        FMOD_System_PlaySound(contexte->system, sonPiano[0], NULL, 0, &contexte->canalSon);
                        FMOD_Channel_SetChannelGroup(contexte->canalSon, contexte->canalGroupSon);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[6]) && erreur == FALSE && reussite == FALSE) // Clic sur "Valider"
                    {
                        verificationNotes(contexte, sonATrouver, nbNotes, noteUneValeur, noteDeuxValeur, noteTroisValeur, &noteUneValeurCorr, &noteDeuxValeurCorr, &noteTroisValeurCorr, &reussite, &erreur);
                        if (reussite == TRUE)
                        {
                            score++;
                            switch (nbNotes)
                            {
                                case 1:
                                    if (bestScore.n1Score < score)
                                    {
                                        bestScore.n1Score++;
                                        sauvegardeBestScore(contexte, nbNotes, bestScore);
                                    }
                                    break;
                                case 2:
                                    if (bestScore.n2Score < score)
                                    {
                                        bestScore.n1Score++;
                                        sauvegardeBestScore(contexte, nbNotes, bestScore);
                                    }
                                    break;
                                case 3:
                                    if (bestScore.n3Score < score)
                                    {
                                        bestScore.n1Score++;
                                        sauvegardeBestScore(contexte, nbNotes, bestScore);
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                        else
                        {
                            correction = TRUE;
                            score = 0;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[7])) // Clic sur "Retour"
                    {
                        /* Fermeture du jeu */
                        fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
                        /* Retour au menu */
                        menuPrincipale(contexte);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[15]) && erreur == TRUE && reussite == FALSE) // Clic sur le texte de mauvaise réponse.
                    {
                        erreur = FALSE;
                        correction = FALSE;
                        noteUne = FALSE;
                        noteUneValeur = -1;
                        noteUneValeurCorr = -1;
                        noteDeux = FALSE;
                        noteDeuxValeur = -1;
                        noteDeuxValeurCorr = -1;
                        noteTrois = FALSE;
                        noteTroisValeur = -1;
                        noteTroisValeurCorr = -1;
                        fermetureSon(contexte, sonPiano[0]);
                        sonATrouver = selectSonPiano(contexte, nbNotes);
                        verifFonction = chargementSon(contexte, sonPiano, sonATrouver);
                        if (verifFonction == 1)
                        {
                            /* Fermeture du jeu */
                            fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
                            /* Fermeture du programme */
                            fermetureGenerale(contexte, 1);
                        }
                        FMOD_System_PlaySound(contexte->system, sonPiano[0], NULL, 0, &contexte->canalSon);
                        FMOD_Channel_SetChannelGroup(contexte->canalSon, contexte->canalGroupSon);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionTexte[16]) && reussite == TRUE && erreur == FALSE) // Clic sur le texte de bonne réponse.
                    {
                        reussite = FALSE;
                        noteUne = FALSE;
                        noteUneValeur = -1;
                        noteUneValeurCorr = -1;
                        noteDeux = FALSE;
                        noteDeuxValeur = -1;
                        noteDeuxValeurCorr = -1;
                        noteTrois = FALSE;
                        noteTroisValeur = -1;
                        noteTroisValeurCorr = -1;
                        fermetureSon(contexte, sonPiano[0]);
                        sonATrouver = selectSonPiano(contexte, nbNotes);
                        verifFonction = chargementSon(contexte, sonPiano, sonATrouver);
                        if (verifFonction == 1)
                        {
                            /* Fermeture du jeu */
                            fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
                            /* Fermeture du programme */
                            fermetureGenerale(contexte, 1);
                        }
                        FMOD_System_PlaySound(contexte->system, sonPiano[0], NULL, 0, &contexte->canalSon);
                        FMOD_Channel_SetChannelGroup(contexte->canalSon, contexte->canalGroupSon);
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[1]) && erreur == FALSE && reussite == FALSE) // Clic sur "DO#"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 1;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 1)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 1)
                                {
                                    noteUneValeur = 1;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 1)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 1;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 1)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 1 && noteTroisValeur != 1)
                                {
                                    noteUneValeur = 1;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 1)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 1)
                                {
                                    noteDeuxValeur = 1;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 1)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 1;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 1)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[3]) && erreur == FALSE && reussite == FALSE) // Clic sur "RE#"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 3;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 3)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 3)
                                {
                                    noteUneValeur = 3;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 3)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 3;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 3)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 3 && noteTroisValeur != 3)
                                {
                                    noteUneValeur = 3;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 3)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 3)
                                {
                                    noteDeuxValeur = 3;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 3)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 3;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 3)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[6]) && erreur == FALSE && reussite == FALSE) // Clic sur "FA#"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 6;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 6)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 6)
                                {
                                    noteUneValeur = 6;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 6)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 6;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 6)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 6 && noteTroisValeur != 6)
                                {
                                    noteUneValeur = 6;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 6)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 6)
                                {
                                    noteDeuxValeur = 6;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 6)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 6;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 6)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[8]) && erreur == FALSE && reussite == FALSE) // Clic sur "SOL#"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 8;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 8)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 8)
                                {
                                    noteUneValeur = 8;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 8)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 8;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 8)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 8 && noteTroisValeur != 8)
                                {
                                    noteUneValeur = 8;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 8)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 8)
                                {
                                    noteDeuxValeur = 8;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 8)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 8;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 8)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[10]) && erreur == FALSE && reussite == FALSE) // Clic sur "LA#"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 10;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 10)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 10)
                                {
                                    noteUneValeur = 10;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 10)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 10;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 10)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 10 && noteTroisValeur != 10)
                                {
                                    noteUneValeur = 10;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 10)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 10)
                                {
                                    noteDeuxValeur = 10;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 10)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 10;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 10)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[0]) && erreur == FALSE && reussite == FALSE) // Clic sur "DO"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 0;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 0)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 0)
                                {
                                    noteUneValeur = 0;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 0)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 0;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 0)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 0 && noteTroisValeur != 0)
                                {
                                    noteUneValeur = 0;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 0)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 0)
                                {
                                    noteDeuxValeur = 0;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 0)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 0;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 0)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[2]) && erreur == FALSE && reussite == FALSE) // Clic sur "RE"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 2;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 2)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 2)
                                {
                                    noteUneValeur = 2;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 2)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 2;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 2)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 2 && noteTroisValeur != 2)
                                {
                                    noteUneValeur = 2;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 2)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 2)
                                {
                                    noteDeuxValeur = 2;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 2)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 2;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 2)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[4]) && erreur == FALSE && reussite == FALSE) // Clic sur "MI"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 4;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 4)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 4)
                                {
                                    noteUneValeur = 4;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 4)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 4;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 4)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 4 && noteTroisValeur != 4)
                                {
                                    noteUneValeur = 4;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 4)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 4)
                                {
                                    noteDeuxValeur = 4;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 4)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 4;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 4)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[5]) && erreur == FALSE && reussite == FALSE) // Clic sur "FA"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 5;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 5)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 5)
                                {
                                    noteUneValeur = 5;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 5)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 5;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 5)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 5 && noteTroisValeur != 5)
                                {
                                    noteUneValeur = 5;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 5)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 5)
                                {
                                    noteDeuxValeur = 5;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 5)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 5;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 5)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[7]) && erreur == FALSE && reussite == FALSE) // Clic sur "SOL"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 7;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 7)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 7)
                                {
                                    noteUneValeur = 7;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 7)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 7;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 7)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 7 && noteTroisValeur != 7)
                                {
                                    noteUneValeur = 7;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 7)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 7)
                                {
                                    noteDeuxValeur = 7;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 7)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 7;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 7)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[9]) && erreur == FALSE && reussite == FALSE) // Clic sur "LA"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 9;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 9)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 9)
                                {
                                    noteUneValeur = 9;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 9)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 9;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 9)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 9 && noteTroisValeur != 9)
                                {
                                    noteUneValeur = 9;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 9)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 9)
                                {
                                    noteDeuxValeur = 9;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 9)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 9;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 9)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    else if (collisionPoint_AABB(mouseX, mouseY, positionNotes[11]) && erreur == FALSE && reussite == FALSE) // Clic sur "SI"
                    {
                        switch (nbNotes)
                        {
                            case 1:
                                if (noteUne == FALSE)
                                {
                                    noteUneValeur = 11;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 11)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                break;
                            case 2:
                                if (noteUne == FALSE && noteDeuxValeur != 11)
                                {
                                    noteUneValeur = 11;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 11)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE)
                                {
                                    noteDeuxValeur = 11;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 11)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                break;
                            case 3:
                                if (noteUne == FALSE && noteDeuxValeur != 11 && noteTroisValeur != 11)
                                {
                                    noteUneValeur = 11;
                                    noteUne = TRUE;
                                }
                                else if (noteUneValeur == 11)
                                {
                                    noteUneValeur = -1;
                                    noteUne = FALSE;
                                }
                                else if (noteDeux == FALSE && noteUne == TRUE && noteTroisValeur != 11)
                                {
                                    noteDeuxValeur = 11;
                                    noteDeux = TRUE;
                                }
                                else if (noteDeuxValeur == 11)
                                {
                                    noteDeuxValeur = -1;
                                    noteDeux = FALSE;
                                }
                                else if (noteTrois == FALSE && noteDeux == TRUE && noteUne == TRUE)
                                {
                                    noteTroisValeur = 11;
                                    noteTrois = TRUE;
                                }
                                else if (noteTroisValeur == 11)
                                {
                                    noteTroisValeur = -1;
                                    noteTrois = FALSE;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;
        }

        /* Affichage du jeu */
        // Préparation
        verifFonction = majTextesJeu(contexte, textes, positionTexte, nbTextes, nbNotes,afficherNotes, score, bestScore);
        if (verifFonction == 1)
        {
            /* Fermeture du jeu */
            fermetureJeu(contexte, touchePiano, sonPiano, textes, nbTextes);
            /* Fermeture du programme */
            fermetureGenerale(contexte, 1);
        }

        // Affichage
        SDL_RenderClear(contexte->rendu);
        for (i = 0 ; i < 8 ; i++)
        {
            SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
        }
        /// Notes
        // DO
        if (noteUneValeur == 0 || noteDeuxValeur == 0 || noteTroisValeur == 0)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[1], NULL, &positionNotes[0]);
        }
        else if (noteUneValeur == 12 || noteDeuxValeur == 12 || noteTroisValeur == 12)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[0]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[0]);
        }
        // DO#
        if (noteUneValeur == 1 || noteDeuxValeur == 1 || noteTroisValeur == 1)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[1]);
        }
        else if (noteUneValeur == 13 || noteDeuxValeur == 13 || noteTroisValeur == 13)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[1]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[1]);
        }
        // RE
        if (noteUneValeur == 2 || noteDeuxValeur == 2 || noteTroisValeur == 2)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[2]);
        }
        else if (noteUneValeur == 14 || noteDeuxValeur == 14 || noteTroisValeur == 14)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[2]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[2]);
        }
        // RE#
        if (noteUneValeur == 3 || noteDeuxValeur == 3 || noteTroisValeur == 3)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[3]);
        }
        else if (noteUneValeur == 15 || noteDeuxValeur == 15 || noteTroisValeur == 15)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[3]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[3]);
        }
        // MI
        if (noteUneValeur == 4 || noteDeuxValeur == 4 || noteTroisValeur == 4)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[7], NULL, &positionNotes[4]);
        }
        else if (noteUneValeur == 16 || noteDeuxValeur == 16 || noteTroisValeur == 16)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[4]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[4]);
        }
        // FA
        if (noteUneValeur == 5 || noteDeuxValeur == 5 || noteTroisValeur == 5)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[1], NULL, &positionNotes[5]);
        }
        else if (noteUneValeur == 17 || noteDeuxValeur == 17 || noteTroisValeur == 17)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[5]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[5]);
        }
        // FA#
        if (noteUneValeur == 6 || noteDeuxValeur == 6 || noteTroisValeur == 6)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[6]);
        }
        else if (noteUneValeur == 18 || noteDeuxValeur == 18 || noteTroisValeur == 18)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[6]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[6]);
        }
        // SOL
        if (noteUneValeur == 7 || noteDeuxValeur == 7 || noteTroisValeur == 7)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[7]);
        }
        else if (noteUneValeur == 19 || noteDeuxValeur == 19 || noteTroisValeur == 19)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[7]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[7]);
        }
        // SOL#
        if (noteUneValeur == 8 || noteDeuxValeur == 8 || noteTroisValeur == 8)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[8]);
        }
        else if (noteUneValeur == 20 || noteDeuxValeur == 20 || noteTroisValeur == 20)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[8]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[8]);
        }
        // LA
        if (noteUneValeur == 9 || noteDeuxValeur == 9 || noteTroisValeur == 9)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[4], NULL, &positionNotes[9]);
        }
        else if (noteUneValeur == 21 || noteDeuxValeur == 21 || noteTroisValeur == 21)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[9]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[9]);
        }
        // LA#
        if (noteUneValeur == 10 || noteDeuxValeur == 10 || noteTroisValeur == 10)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[10], NULL, &positionNotes[10]);
        }
        else if (noteUneValeur == 22 || noteDeuxValeur == 22 || noteTroisValeur == 22)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[10]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[10]);
        }
        // SI
        if (noteUneValeur == 11 || noteDeuxValeur == 11 || noteTroisValeur == 11)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[7], NULL, &positionNotes[11]);
        }
        else if (noteUneValeur == 23 || noteDeuxValeur == 23 || noteTroisValeur == 23)
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[11]);
        }
        else
        {
            SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[11]);
        }
        /// Notes Corrigées
        if (correction)
        {
            // DO
            if (noteUneValeurCorr == 12 || noteDeuxValeurCorr == 12 || noteTroisValeurCorr == 12)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[0]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[0]);
            }
            // DO#
            if (noteUneValeurCorr == 13 || noteDeuxValeurCorr == 13 || noteTroisValeurCorr == 13)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[1]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[1]);
            }
            // RE
            if (noteUneValeurCorr == 14 || noteDeuxValeurCorr == 14 || noteTroisValeurCorr == 14)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[2]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[2]);
            }
            // RE#
            if (noteUneValeurCorr == 15 || noteDeuxValeurCorr == 15 || noteTroisValeurCorr == 15)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[3]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[3]);
            }
            // MI
            if (noteUneValeurCorr == 16 || noteDeuxValeurCorr == 16 || noteTroisValeurCorr == 16)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[4]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[4]);
            }
            // FA
            if (noteUneValeurCorr == 17 || noteDeuxValeurCorr == 17 || noteTroisValeurCorr == 17)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[2], NULL, &positionNotes[5]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[0], NULL, &positionNotes[5]);
            }
            // FA#
            if (noteUneValeurCorr == 18 || noteDeuxValeurCorr == 18 || noteTroisValeurCorr == 18)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[6]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[6]);
            }
            // SOL
            if (noteUneValeurCorr == 19 || noteDeuxValeurCorr == 19 || noteTroisValeurCorr == 19)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[7]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[7]);
            }
            // SOL#
            if (noteUneValeurCorr == 20 || noteDeuxValeurCorr == 20 || noteTroisValeurCorr == 20)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[8]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[8]);
            }
            // LA
            if (noteUneValeurCorr == 21 || noteDeuxValeurCorr == 21 || noteTroisValeurCorr == 21)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[5], NULL, &positionNotes[9]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[3], NULL, &positionNotes[9]);
            }
            // LA#
            if (noteUneValeurCorr == 22 || noteDeuxValeurCorr == 22 || noteTroisValeurCorr == 22)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[11], NULL, &positionNotes[10]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[9], NULL, &positionNotes[10]);
            }
            // SI
            if (noteUneValeurCorr == 23 || noteDeuxValeurCorr == 23 || noteTroisValeurCorr == 23)
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[8], NULL, &positionNotes[11]);
            }
            else
            {
                SDL_RenderCopy(contexte->rendu, touchePiano[6], NULL, &positionNotes[11]);
            }
        }
        if (afficherNotes == TRUE)
        {
            for (i = 8 ; i < 15 ; i++)
            {
                SDL_RenderCopy(contexte->rendu, textes[i], NULL, &positionTexte[i]);
            }
        }
        if (erreur == TRUE)
        {
            SDL_RenderCopy(contexte->rendu, textes[15], NULL, &positionTexte[15]);
        }
        if (reussite == TRUE)
        {
            SDL_RenderCopy(contexte->rendu, textes[16], NULL, &positionTexte[16]);
        }
        SDL_RenderPresent(contexte->rendu);
    }
}

int selectSonPiano(Base *contexte, int nbNotes)
{
    int sonATrouve = -1;

    switch (nbNotes)
    {
        case 1:
            sonATrouve = rand() % 24;
            break;
        case 2:
            sonATrouve = rand() % 132;
            sonATrouve += 24;
            break;
        case 3:
            sonATrouve = rand() % 440;
            sonATrouve += 156;
            break;
    }

    return sonATrouve;
}

void verificationNotes(Base *contexte, int sonATrouver, int nbNotes, int noteUne, int noteDeux, int noteTrois, int *noteUneCorr, int *noteDeuxCorr, int *noteTroisCorr, int *reussite, int *erreur)
{
    int i = 0;

    switch(nbNotes)
    {
        case 1:
            for (i = 0 ; i < 12; i++)
            {
                if (sonATrouver == i || sonATrouver == i + 12)
                {
                    if (noteUne == i)
                    {
                        *reussite = TRUE;
                    }
                    else
                    {
                        *noteUneCorr = i + 12;
                        *erreur = TRUE;
                    }
                }
            }
            break;
        case 2:
            switch (sonATrouver)
            {
                case 24:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 25:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 26:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 27:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 28:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 29:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 30:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 31:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 32:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 33:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 34:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 35:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 36:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 37:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 38:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 39:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 40:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 41:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 42:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 43:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 44:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 45:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 46:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 47:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 48:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 49:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 50:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 51:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 52:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 53:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 54:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 55:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 56:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 57:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 58:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 59:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 60:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 61:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 62:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 63:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 64:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 65:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 66:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 67:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 68:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 69:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 70:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 71:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 72:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 73:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 74:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 75:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 76:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 77:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 78:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 79:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 80:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 81:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 82:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 83:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 84:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 85:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 86:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 87:
                    if (noteUne == 9 || noteDeux == 9)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 88:
                    if (noteUne == 9 || noteDeux == 9)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 89:
                    if (noteUne == 10 || noteDeux == 10)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 10 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 10 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 90:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 91:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 92:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 93:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 94:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 95:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 96:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 97:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 98:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 99:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 100:
                    if (noteUne == 0 || noteDeux == 0)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 101:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 102:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 103:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 104:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 105:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 106:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 107:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 108:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 109:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 110:
                    if (noteUne == 1 || noteDeux == 1)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 111:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 112:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 113:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 114:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 115:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 116:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 117:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 118:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 119:
                    if (noteUne == 2 || noteDeux == 2)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 120:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 121:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 122:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 123:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 124:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 125:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 126:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 127:
                    if (noteUne == 3 || noteDeux == 3)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 128:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 129:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 130:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 131:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 132:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 133:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 134:
                    if (noteUne == 4 || noteDeux == 4)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 135:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 136:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 137:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 138:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 139:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 140:
                    if (noteUne == 5 || noteDeux == 5)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 141:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 142:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 143:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 144:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 145:
                    if (noteUne == 6 || noteDeux == 6)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 146:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 147:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 148:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 149:
                    if (noteUne == 7 || noteDeux == 7)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 150:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 151:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 152:
                    if (noteUne == 8 || noteDeux == 8)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 153:
                    if (noteUne == 9 || noteDeux == 9)
                    {
                        if (noteUne == 10 || noteDeux == 10)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 154:
                    if (noteUne == 9 || noteDeux == 9)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 155:
                    if (noteUne == 10 || noteDeux == 10)
                    {
                        if (noteUne == 11 || noteDeux == 11)
                        {
                            *reussite = TRUE;
                        }
                        else
                        {
                            *noteUneCorr = 10 + 12;
                            *noteDeuxCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 10 + 12;
                        *noteDeuxCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 3:
            switch (sonATrouver)
            {
                case 156:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 2 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 157:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 158:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 159:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 160:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 161:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 162:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 163:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 164:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 165:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 166:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 167:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 168:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 169:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 170:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 171:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 172:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 173:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 174:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 175:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 176:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 177:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 178:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 179:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 180:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 181:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 182:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 183:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 184:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 185:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 186:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 187:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 188:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 189:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 190:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 191:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 192:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 193:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 194:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 195:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 196:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 197:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 198:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 199:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 200:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 201:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 202:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 203:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 204:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 205:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 206:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 207:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 208:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 209:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 210:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 211:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 212:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 213:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 214:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 215:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 216:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 217:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 218:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *erreur = TRUE;
                    }
                    break;
                case 219:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 220:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 221:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 222:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 223:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 224:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 225:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 226:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 227:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 228:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 229:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 230:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 231:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 232:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 233:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 234:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 235:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 236:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 237:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 238:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 239:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 240:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 241:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 242:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 243:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 244:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 245:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 246:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 247:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 248:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 249:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 250:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 251:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 252:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 253:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 254:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 255:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 256:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 257:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 258:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 259:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 260:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 261:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 262:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 263:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 264:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 265:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 266:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 267:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 268:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 269:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 270:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 271:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 272:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 273:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 274:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 275:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 276:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 277:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 278:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 279:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 280:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 281:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 282:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 283:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 284:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 285:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 286:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 287:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 288:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 289:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 290:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 291:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 292:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 293:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 294:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 295:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 296:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 297:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 298:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 299:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 300:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 301:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 302:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 303:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 304:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 305:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 306:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 307:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 308:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 309:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 310:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 311:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 312:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 313:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 314:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 315:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 316:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 317:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 318:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 319:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 320:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 321:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 322:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 323:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 324:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 325:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 326:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 327:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 328:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 329:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 330:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 331:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 332:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 333:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 334:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 335:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 336:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 337:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 338:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 339:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 340:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 341:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 342:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 343:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 344:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 345:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 346:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 347:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 348:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 349:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 350:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 351:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 352:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 353:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 354:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 355:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 356:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 357:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 358:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 359:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 360:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 361:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 362:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 363:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 364:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 365:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 366:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 367:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 368:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 369:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 370:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 371:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 372:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 373:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 374:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 375:
                    if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 9 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 376:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 2 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 2 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 2 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 377:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 378:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 379:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 380:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 381:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 382:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 383:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 384:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 385:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 1 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 1 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 1 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 386:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 387:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 388:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 389:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 390:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 391:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 392:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 393:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 394:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 395:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 396:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 397:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 398:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 399:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 400:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 401:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 402:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 403:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 404:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 405:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 406:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 407:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 408:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 409:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 410:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 411:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 412:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 413:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 414:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 415:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 416:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 417:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 418:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 419:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 420:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 421:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 422:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 423:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 424:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 425:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 426:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 427:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 428:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 429:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 430:
                    if (noteUne == 0 || noteDeux == 0 || noteTrois == 0)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 0 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 0 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 0 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 431:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 3 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 3 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 3 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 432:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 433:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 434:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 435:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 436:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 437:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 438:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *erreur = TRUE;
                    }
                    break;
                case 439:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 2 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 2 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 2 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 440:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 441:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 442:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 443:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 444:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 445:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 446:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 447:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 448:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 449:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 450:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 451:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 452:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 453:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 454:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 455:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 456:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 457:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 458:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 459:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 460:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 461:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 462:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 463:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 464:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 465:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 466:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 467:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 468:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 469:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 470:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 471:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 472:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 473:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 474:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 475:
                    if (noteUne == 1 || noteDeux == 1 || noteTrois == 1)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 1 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 1 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 1 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 476:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 4 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 4 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 4 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 477:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 478:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 479:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 480:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 481:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 482:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 483:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 3 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 3 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 3 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 484:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 485:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 486:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 487:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 488:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 489:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 490:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 491:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 492:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 493:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 494:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 495:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 496:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 497:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 498:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 499:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 500:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 501:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 502:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 503:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 504:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 505:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 506:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 507:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 508:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 509:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 510:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 511:
                    if (noteUne == 2 || noteDeux == 2 || noteTrois == 2)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 2 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 2 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 2 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 512:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 5 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 5 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 5 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 513:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 514:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 515:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 516:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 517:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 518:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 4 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 4 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 4 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 519:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 520:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 521:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 522:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 523:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 524:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 525:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 526:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 527:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 528:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 529:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 530:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 531:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 532:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 533:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 534:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 535:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 536:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 537:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 538:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 539:
                    if (noteUne == 3 || noteDeux == 3 || noteTrois == 3)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 3 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 3 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 3 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 540:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 6 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 6 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 6 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 541:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 542:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 543:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 544:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 545:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 5 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 5 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 5 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 546:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 547:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 548:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 549:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 550:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 551:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 552:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 553:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 554:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 555:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 556:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 557:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 558:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 559:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 560:
                    if (noteUne == 4 || noteDeux == 4 || noteTrois == 4)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 4 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 4 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 4 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 561:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 7 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 7 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 7 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 562:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 563:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 564:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 565:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 6 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 6 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 6 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 566:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 567:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 568:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 569:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 570:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 571:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 572:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 573:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 574:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 575:
                    if (noteUne == 5 || noteDeux == 5 || noteTrois == 5)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 5 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 5 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 5 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 576:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 8 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 8 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 8 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 577:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 578:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 579:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 7 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 7 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 7 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 580:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 581:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 582:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 583:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 584:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 585:
                    if (noteUne == 6 || noteDeux == 6 || noteTrois == 6)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 6 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 6 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 6 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 586:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 9 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 9 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 9 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 587:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 588:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 8 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 8 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 8 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 589:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 590:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 591:
                    if (noteUne == 7 || noteDeux == 7 || noteTrois == 7)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 7 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 7 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 7 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 592:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 10 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 10 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 10 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 593:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 9 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 9 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 9 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 594:
                    if (noteUne == 8 || noteDeux == 8 || noteTrois == 8)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 8 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 8 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 8 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                case 595:
                    if (noteUne == 9 || noteDeux == 9 || noteTrois == 9)
                    {
                        if (noteUne == 10 || noteDeux == 10 || noteTrois == 10)
                        {
                            if (noteUne == 11 || noteDeux == 11 || noteTrois == 11)
                            {
                                *reussite = TRUE;
                            }
                            else
                            {
                                *noteUneCorr = 9 + 12;
                                *noteDeuxCorr = 10 + 12;
                                *noteTroisCorr = 11 + 12;
                                *erreur = TRUE;
                            }
                        }
                        else
                        {
                            *noteUneCorr = 9 + 12;
                            *noteDeuxCorr = 10 + 12;
                            *noteTroisCorr = 11 + 12;
                            *erreur = TRUE;
                        }
                    }
                    else
                    {
                        *noteUneCorr = 9 + 12;
                        *noteDeuxCorr = 10 + 12;
                        *noteTroisCorr = 11 + 12;
                        *erreur = TRUE;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

int chargementBestScore(Base *contexte, int nbNotes, scoreComplet *bestScore)
{
    scoreComplet initScores = {0, 0, 0};
    FILE *fichier = NULL;

    /* Création du fichier de score */
    fichier = fopen("score.sc", "rb");
    if (fichier == NULL)
    {
        fichier = fopen("score.sc", "wb+");
        if (fichier == NULL)
        {
            switch(contexte->langue)
            {
                case ENG:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                    fprintf(contexte->my_stderr, "File opening failure.\n");
                    break;
                case FRA:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                    fprintf(contexte->my_stderr, "Echec de l'ouverture d'un fichier.\n");
                    break;
                default:
                    break;
            }
            return 1;
        }
        fwrite(&initScores, sizeof(scoreComplet), 1, fichier);

        fclose(fichier);
    }
    else
    {
        fread(bestScore, sizeof(scoreComplet), 1, fichier);
    }
    fclose(fichier);
    return 0;
}

int sauvegardeBestScore(Base *contexte, int nbNotes, const scoreComplet bestScore)
{
    FILE *fichier;

    fichier = fopen("score.sc", "wb+");
    if (fichier == NULL)
    {
        switch(contexte->langue)
            {
                case ENG:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                    fprintf(contexte->my_stderr, "File opening failure.\n");
                    break;
                case FRA:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                    fprintf(contexte->my_stderr, "Echec de l'ouverture d'un fichier.\n");
                    break;
                default:
                    break;
            }
            return 1;
    }
    fwrite(&bestScore, sizeof(scoreComplet), 1, fichier);
    fclose(fichier);

    return 0;
}

int majTextesJeu(Base *contexte, SDL_Texture* *textes, SDL_Rect *positionTexte, int nbTextes, int nbNotes, int afficherNotes, int score, scoreComplet bestScore)
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
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_GRAND, nbNotes, afficherNotes, score, bestScore);
                break;
            case 1:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            case 2:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            case 3:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            case 4:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            case 8:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 9:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 10:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 11:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 12:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 13:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 14:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MINI, nbNotes, afficherNotes, score, bestScore);
                break;
            case 15:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            case 16:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_PETIT, nbNotes, afficherNotes, score, bestScore);
                break;
            default:
                textes[i] = texteJeu(contexte, &positionTexte[i], i, TAILLE_TEXTE_MOYEN, nbNotes, afficherNotes, score, bestScore);
                break;
        }
        if (textes[i] == NULL) {return 1;}
    }

    return 0;
}

SDL_Texture *texteJeu(Base *contexte, SDL_Rect *positionTexte, int numTexte, int tailleTexte, int nbNotes, int afficherNotes, int score, scoreComplet bestScore)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *textureTexte = NULL;
    SDL_Color couleurTexte = {255, 255, 255};
    SDL_Color couleurTexteNoir = {0, 0, 0};
    SDL_Color couleurbg = {70, 70, 70};
    char texte[50] = "";

    /* Préparation du texte */
    switch(numTexte)
    {
        case 0:
            snprintf(texte, 50, "Musicus");
            break;
        case 1:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Score : %d", score);
                    break;
                case FRA:
                    snprintf(texte, 50, "Score : %d", score);
                    break;
                default:
                    break;
            }
            break;
        case 2:
            switch (nbNotes)
            {
                case 1:
                    switch(contexte->langue)
                    {
                        case ENG:
                            snprintf(texte, 50, "Best score : %d", bestScore.n1Score);
                            break;
                        case FRA:
                            snprintf(texte, 50, "Meilleur score : %d", bestScore.n1Score);
                            break;
                        default:
                            break;
                    }
                    break;
                case 2:
                    switch(contexte->langue)
                    {
                        case ENG:
                            snprintf(texte, 50, "Best score : %d", bestScore.n2Score);
                            break;
                        case FRA:
                            snprintf(texte, 50, "Meilleur score : %d", bestScore.n2Score);
                            break;
                        default:
                            break;
                    }
                    break;
                case 3:
                    switch(contexte->langue)
                    {
                        case ENG:
                            snprintf(texte, 50, "Best score : %d", bestScore.n3Score);
                            break;
                        case FRA:
                            snprintf(texte, 50, "Meilleur score : %d", bestScore.n3Score);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case 3:
            if (nbNotes == 1)
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "Find the note which is played");
                        break;
                    case FRA:
                        snprintf(texte, 50, "Quelle est la note jouée ?");
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
                        snprintf(texte, 50, "Find all the notes which are played");
                        break;
                    case FRA:
                        snprintf(texte, 50, "Quelles sont les notes jouées ?");
                        break;
                    default:
                        break;
                }
            }
            break;
        case 4:
            if (afficherNotes == TRUE)
            {
                switch(contexte->langue)
                {
                    case ENG:
                        snprintf(texte, 50, "[X] Show notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[X] Afficher notes");
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
                        snprintf(texte, 50, "[   ] Show notes");
                        break;
                    case FRA:
                        snprintf(texte, 50, "[   ] Afficher notes");
                        break;
                    default:
                        break;
                }
            }
            break;
        case 5:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Replay note");
                    break;
                case FRA:
                    snprintf(texte, 50, "Rejouer note");
                    break;
                default:
                    break;
            }
            break;
        case 6:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Submit");
                    break;
                case FRA:
                    snprintf(texte, 50, "Valider");
                    break;
                default:
                    break;
            }
            break;
        case 7:
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
        case 8:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "C");
                    break;
                case FRA:
                    snprintf(texte, 50, "DO");
                    break;
                default:
                    break;
            }
            break;
        case 9:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "D");
                    break;
                case FRA:
                    snprintf(texte, 50, "RE");
                    break;
                default:
                    break;
            }
            break;
        case 10:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "E");
                    break;
                case FRA:
                    snprintf(texte, 50, "MI");
                    break;
                default:
                    break;
            }
            break;
        case 11:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "F");
                    break;
                case FRA:
                    snprintf(texte, 50, "FA");
                    break;
                default:
                    break;
            }
            break;
        case 12:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "G");
                    break;
                case FRA:
                    snprintf(texte, 50, "SOL");
                    break;
                default:
                    break;
            }
            break;
        case 13:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "A");
                    break;
                case FRA:
                    snprintf(texte, 50, "LA");
                    break;
                default:
                    break;
            }
            break;
        case 14:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "B");
                    break;
                case FRA:
                    snprintf(texte, 50, "SI");
                    break;
                default:
                    break;
            }
            break;
        case 15:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Wrong answer, clic here to continue.");
                    break;
                case FRA:
                    snprintf(texte, 50, "Mauvaise réponse, cliquez ici pour continuer.");
                    break;
                default:
                    break;
            }
            break;
        case 16:
            switch(contexte->langue)
            {
                case ENG:
                    snprintf(texte, 50, "Great ! Clic here to continue.");
                    break;
                case FRA:
                    snprintf(texte, 50, "Bonne réponse ! Cliquez ici pour continuer.");
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
            surface = TTF_RenderUTF8_Shaded(contexte->policeMini, texte, couleurTexteNoir, couleurbg);
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

    if (surface == NULL)
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
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 70, 70, 70));
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
            positionTexte->x = 20;
            positionTexte->y = 250;
            break;
        case 2:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 20;
            positionTexte->y = 250 + surface->h;
            break;
        case 3:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = 250;
            break;
        case 4:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES - 75 - surface->w;
            positionTexte->y = YRES / 2 - surface->h / 2 + 100;
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
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES - 150;
            break;
        case 7:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES - 50 - surface->w;
            positionTexte->y = YRES - 150;
            break;
        case 8:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 525 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 9:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 575 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 10:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 625 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 11:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 675 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 12:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 725 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 13:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 775 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 14:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = 825 + 25 - surface->w / 2;
            positionTexte->y = YRES / 2 + 150;
            break;
        case 15:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES / 2 - surface->h - 30;
            break;
        case 16:
            positionTexte->w = surface->w;
            positionTexte->h = surface->h;
            positionTexte->x = XRES / 2 - surface->w / 2;
            positionTexte->y = YRES / 2 - surface->h - 30;
            break;
        default:
            break;
    }

    fermetureSurface(contexte, surface);

    return textureTexte;
}

int chargementJeu(Base *contexte, SDL_Texture* *touchePiano)
{
    int i = 0;
    char nomImage[10] = "";

    for (i = 0 ; i < 12 ; i++)
    {
        snprintf(nomImage, 10, "%d.png", i);
        touchePiano[i] = initTextureJeuRessources(contexte, nomImage);

        if (touchePiano[i] == NULL)
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
            return 1;
        }
    }
    return 0;
}

int chargementSon(Base *contexte, FMOD_SOUND* *sonPiano, int sonATrouver)
{
    char nomSon[10] = "";

    if (sonATrouver < 10)
    {
        snprintf(nomSon, 10, "00%d.ogg", sonATrouver);
    }
    else if (sonATrouver < 100)
    {
        snprintf(nomSon, 10, "0%d.ogg", sonATrouver);
    }
    else
    {
        snprintf(nomSon, 10, "%d.ogg", sonATrouver);
    }
    sonPiano[0] = initSonJeuRessources(contexte, nomSon);
    if (sonPiano[0] == NULL)
    {
        return 1;
    }
    return 0;
}

SDL_Texture *initTextureJeuRessources(Base *contexte, char *nomImage)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    surface = IMG_RcLoad(nomImage);
    if (surface != NULL)
    {
        texture = SDL_CreateTextureFromSurface(contexte->rendu, surface);

        SDL_FreeSurface(surface);
    }

    return texture;
}

FMOD_SOUND *initSonJeuRessources(Base *contexte, char *nomSon)
{
    FMOD_SOUND *son = NULL;

    contexte->verifFMOD = FMOD_System_CreateRcSound(contexte->system, nomSon, FMOD_CREATESAMPLE, 0, &son);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Sound loading failure. Error code : %d\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors du chargement d'un son. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        return NULL;
    }

    return son;
}

void fermetureJeu(Base *contexte, SDL_Texture* *touchePiano, FMOD_SOUND* *sonPiano, SDL_Texture* *texte, int nbTextes)
{
    int i;

    for (i = 0 ; i < nbTextes ; i++)
    {
        if (texte[i] != NULL)
        {
            fermetureTexture(contexte, texte[i]);
        }
    }

    for (i = 0 ; i < 12 ; i++)
    {
        if (touchePiano[i] != NULL)
        {
            fermetureTexture(contexte, touchePiano[i]);
        }
    }

    if (sonPiano[0] != NULL)
    {
        fermetureSon(contexte, sonPiano[0]);
    }
}
