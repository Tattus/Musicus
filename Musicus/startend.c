/* startend.c
   ----------

   Créé par : Tattus
   le : 10/04/2015

   Modifié le : 16/04/2017
   ----------
   Fonction d'initialisations et de fermetures.

*/
#include "general.h"

void initialisationGenerale(Base *contexte)
{
    setResourceContenerName("musicus.res");
    setPrefix("rc_");
    initialisationCredits();
    initialisationRedirectErreur(contexte);
    initialisationLangue(contexte);
    initialisationSDL(contexte);
    initialisationFMOD(contexte);
    initialisationSDLTTF(contexte);
    initialisationFenetre(contexte);
    initialisationRendu(contexte);
    initialisationPolice(contexte);
    initialisationCanal(contexte);
}

void fermetureGenerale(Base *contexte, int codeFermeture)
{
    fermetureGroupeCanaux(contexte, contexte->canalGroupSon);
    //fermetureCanaux(contexte, NULL, contexte->canalGroupMusique);
    //fermetureCanaux(contexte, NULL, contexte->canalGroupVoix);
    fermeturePolice(contexte, contexte->policeGrand);
    fermeturePolice(contexte, contexte->policeMoyen);
    fermeturePolice(contexte, contexte->policePetit);
    fermeturePolice(contexte, contexte->policeMini);
    closeRcFile(NULL);
    fermetureRendu(contexte);
    fermetureFenetre(contexte);
    fermetureSDLTTF(contexte, codeFermeture);
}

void initialisationRedirectErreur(Base *contexte)
{
    time_t timestamp = time(NULL);

    // Récupération de la date d'éxécution.
    strftime(contexte->errorPath, sizeof(contexte->errorPath), "Error_Report/%Y-%m-%d_error_report.txt", localtime(&timestamp));

    /* Redirection d'erreur */
    #ifdef LINUX
        mkdir("./Error_Report", 0770);
    #else
        mkdir("./Error_Report");
    #endif
    contexte->my_stderr = freopen(contexte->errorPath, "a", stderr);
    if (contexte->my_stderr == NULL)
    {
        exit(EXIT_FAILURE);
    }
}

void initialisationSDL(Base *contexte)
{
    if (SDL_Init(SDL_INIT_VIDEO != 0))
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "SDL initialization failure. (%s)\n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors de l'initialisation de la SDL. (%s)\n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureRedirectErreur(contexte, 1);
    }
}

void initialisationFMOD(Base *contexte)
{
    contexte->verifFMOD = FMOD_System_Create(&contexte->system);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "FMOD Studio initialization failure. Error code : (%d)\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors de l'initialisation de FMOD Studio. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        fermetureSDL(contexte, 1);
    }
    contexte->verifFMOD = FMOD_System_Init(contexte->system, 1, FMOD_INIT_NORMAL, 0);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "FMOD Studio initialization failure. Error code : %d\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors de l'initialisation de FMOD Studio. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        fermetureFMOD(contexte, 1);
    }
}

void initialisationSDLTTF(Base *contexte)
{
    if (TTF_Init() == -1)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "SDL_TTF initialization failure. (%s)\n", TTF_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors de l'initialisation de SDL_TTF. (%s)\n", TTF_GetError());
                break;
            default:
                break;
        }
        fermetureFMODInter(contexte, 1);
    }
}

void fermetureSDLTTF(Base *contexte, int codeFermeture)
{
    TTF_Quit();
    fermetureFMODInter(contexte, codeFermeture);
}

void fermetureFMODInter(Base *contexte, int codeFermeture)
{
    FMOD_System_Close(contexte->system);
    fermetureFMOD(contexte, codeFermeture);
}

void fermetureFMOD(Base *contexte, int codeFermeture)
{
    FMOD_System_Release(contexte->system);
    fermetureSDL(contexte, codeFermeture);
}

void fermetureSDL(Base *contexte, int codeFermeture)
{
    SDL_Quit();
    fermetureRedirectErreur(contexte, codeFermeture);
}

void fermetureRedirectErreur(Base *contexte, int codeFermeture)
{
    FILE *errorFile = NULL;
    long posCurseurLecture = 0;

    // Fermeture de la redirection d'erreur.
    fclose(contexte->my_stderr);

    // Suppression du fichier d'erreur s'il est vide.
    errorFile = fopen(contexte->errorPath, "r");
    if (errorFile != NULL)
    {
        fseek(errorFile, 0, SEEK_END);
        posCurseurLecture= ftell(errorFile);
        fclose(errorFile);
        if (posCurseurLecture == 0)
        {
            remove(contexte->errorPath);
        }
    }

    // Fermeture du programme.
    if (codeFermeture == 1)
    {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void initialisationFenetre(Base *contexte)
{
    SDL_Surface *icone = NULL;

    contexte->fenetre = SDL_CreateWindow("Musicus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, XRES, YRES, SDL_WINDOW_SHOWN);
    if (contexte->fenetre == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr,"Window creation failure. (%s) \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr,"Echec lors de la création de la fenêtre. (%s) \n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    icone = LoadRcBitmap("icone.bmp");
    if (icone == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr,"Window's icon loading failure. %s \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr,"Echec lors du chargement de l'icone de la fenêtre. %s \n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    SDL_SetColorKey(icone, SDL_TRUE, SDL_MapRGB(icone->format, 255, 255, 255));
    SDL_SetWindowIcon(contexte->fenetre, icone);
    fermetureSurface(contexte, icone);
    //SDL_ShowCursor(SDL_DISABLE);
}

void initialisationRendu(Base *contexte)
{
    contexte->rendu = SDL_CreateRenderer(contexte->fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (contexte->rendu == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Renderer initialization failure. (%s) \n", SDL_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec lors de l'initialisation du rendu. (%s) \n", SDL_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    SDL_SetRenderDrawColor(contexte->rendu, 0, 0, 0, 255);
}

void initialisationPolice(Base *contexte)
{
    contexte->policeGrand = TTF_OpenRcFontNoDel("LiberationSerif-Regular.ttf", TAILLE_POLICE_GRAND);
    if (contexte->policeGrand == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Loading font failure. (%s)\n", TTF_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de la police de caractère. (%s)\n", TTF_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    contexte->policeMoyen = TTF_OpenRcFontNoDel("LiberationSerif-Regular.ttf", TAILLE_POLICE_MOYEN);
    if (contexte->policeMoyen == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Loading font failure. (%s)\n", TTF_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de la police de caractère. (%s)\n", TTF_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    contexte->policePetit = TTF_OpenRcFontNoDel("LiberationSerif-Regular.ttf", TAILLE_POLICE_PETIT);
    if (contexte->policePetit == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Loading font failure. (%s)\n", TTF_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de la police de caractère. (%s)\n", TTF_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    contexte->policeMini = TTF_OpenRcFontNoDel("LiberationSerif-Regular.ttf", TAILLE_POLICE_MINI);
    if (contexte->policeMini == NULL)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Loading font failure. (%s)\n", TTF_GetError());
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec du chargement de la police de caractère. (%s)\n", TTF_GetError());
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
}

void initialisationCanal(Base *contexte)
{
    contexte->verifFMOD = FMOD_System_CreateChannelGroup(contexte->system, NULL, &contexte->canalGroupSon);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Channel group initialization failure. Error code : %d\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec de l'initialisation du groupe de canaux. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }
    /*contexte->verifFMOD = FMOD_System_CreateChannelGroup(contexte->system, NULL, &contexte->canalGroupMusique);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Channel group initialization failure. Error code : %d\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec de l'initialisation du groupe de canaux. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }*/
    /*contexte->verifFMOD = FMOD_System_CreateChannelGroup(contexte->system, NULL, &contexte->canalGroupVoix);
    if (contexte->verifFMOD != FMOD_OK)
    {
        switch(contexte->langue)
        {
            case ENG:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_ENG, MESSAGE_ERREUR_ENG, NULL);
                fprintf(contexte->my_stderr, "Channel group initialization failure. Error code : %d\n", contexte->verifFMOD);
                break;
            case FRA:
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, TITRE_MSG_ERREUR_FRA, MESSAGE_ERREUR_FRA, NULL);
                fprintf(contexte->my_stderr, "Echec de l'initialisation du groupe de canaux. Code erreur : %d\n", contexte->verifFMOD);
                break;
            default:
                break;
        }
        fermetureGenerale(contexte, 1);
    }*/
}

void initialisationLangue(Base *contexte)
{
    FILE *fichierLangue = NULL;
    int codeLangage = 0;

    fichierLangue = fopen("Language.txt", "r");
    if (fichierLangue == NULL)
    {
        contexte->langue = ENG;
    }
    codeLangage = lireLongFichier(fichierLangue);

    switch(codeLangage)
    {
        case ENG:
            contexte->langue = ENG;
            break;
        case FRA:
            contexte->langue = FRA;
            break;
        default:
            contexte->langue = ENG;
            break;
    }

    fclose(fichierLangue);
}

void fermetureRendu(Base *contexte)
{
    if (contexte->rendu != NULL)
    {
        SDL_DestroyRenderer(contexte->rendu);
    }
}

void fermetureFenetre(Base *contexte)
{
    if (contexte->fenetre != NULL)
    {
        SDL_DestroyWindow(contexte->fenetre);
    }
}

void fermeturePolice(Base *contexte, TTF_Font *police)
{
    if (police != NULL)
    {
        TTF_CloseFont(police);
    }
}

void fermetureSurface(Base *contexte, SDL_Surface *surface)
{
    if (surface != NULL)
    {
        SDL_FreeSurface(surface);
    }
}

void fermetureTexture(Base *contexte, SDL_Texture *texture)
{
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
    }
}

void fermetureSon(Base *contexte, FMOD_SOUND *son)
{
    if (son != NULL)
    {
        FMOD_Sound_Release(son);
    }
}

void fermetureGroupeCanaux(Base *contexte, FMOD_CHANNELGROUP *canalGroup)
{
    if (canalGroup != NULL)
    {
        FMOD_ChannelGroup_Release(canalGroup);
    }
}
