/* RcLoadus.c
   ----------

   Créé par : Tattus
   le : 13/04/2015

   Modifié le : 30/04/2015
   ----------
   Fonctions qui gère le chargement des ressources.
   Peut nécessiter des bibliothèques tierces : SDL2, SDL2_images, SDL_TTF, FMOD_STUDIO_LOWLEVEL
*/
#include <stdlib.h>
#include "RcLoadus.h"

#define DEFAUT_RESOURCE_FILE_NAME "resource.dat"
static char gs_pNomFichierResources[FILENAME_MAX] = DEFAUT_RESOURCE_FILE_NAME;
static int	gs_nRcLoadusErrorCode;
#define DEFAUT_RC_TEMP_PREFIX "rc_"
static char gs_rcTempPrefix[9] = DEFAUT_RC_TEMP_PREFIX;
static char gs_pTempFileName[FILENAME_MAX] = DEFAUT_RC_TEMP_PREFIX;
static int  gs_bIsCrypted;

// Prototypes locaux :
int bIsRcNameFound(FILE *fp, char *a_pResourceName, int *a_nRcOffset, int *a_nRcSize, int a_nOrder); // Fonction qui va rechercher dans le fichier RC si une ressource existe.
char *getTempResFile(char *a_pResName, int *a_pnSize, int *a_pnOffset); // Fonction qui va creer le fichier temporaire de la ressource.
int freadRcCrypted(unsigned char *a_buffer, int a_size, int a_nbElem, FILE *a_fp); // Fonction qui lit un fichier de ressources crypté.

/* Fonctions : */
//**********************************************************************************************************************************
// getRcFile :
//****************
//  cette fonction prend en arguments :
//		le nom d'une ressource contenant un fichier,texte ou binaire . par exemple "monfic.txt".
//		un pointeur sur un int qui sera renvoyé/chargé avec la taille utilisable des données
//   Elle va créer un fichier temporaire "rc_monfic.txt", puis allouer (malloc) une zone de mémoire équivalente à la taille
//   du fichier, puis charger le contenu du fichier temporaire dans cette zone de mémoire allouée, + 1 octet pour un 0x00 à la fin.
//   le fichier temporaire rc_monfichier.txt est immédiatement détruit.
//  La fonction renvoie le pointeur sur cette mémoire.
//  Il appartient au programme appelant de faire free( monpointeur ) sur le pointeur retourné par cette présente fonction.
//**********************************************************************************************************************************
char *getRcFile(char *a_pResName, int *a_pnSize)
{
    char    *ptrdata;
	char 	*tempFileName;
	int     size;
	FILE    *fp;

	tempFileName = getTempResFile((char *)a_pResName, &size, NULL);
	if(!tempFileName)
    {
        return NULL;
    }
	*a_pnSize = size;
	fp = fopen(tempFileName, "rb");
	if(!fp)
    {
        return NULL;
    }
    ptrdata = (char*)malloc(size + 1);
	fread((unsigned char *)ptrdata, size, 1, fp);
	ptrdata[size] = 0x00;
	fclose(fp);
	remove(tempFileName);
	return ptrdata;
}

#ifdef DEF_RCFMODCORE
FMOD_RESULT FMOD_System_CreateRcSound(FMOD_SYSTEM *a_system, const char *a_rcname, FMOD_MODE a_mode, FMOD_CREATESOUNDEXINFO *a_finfo, FMOD_SOUND **a_ppsound)
{
	char *tempFileName;
	FMOD_RESULT	result;

	tempFileName = getTempResFile((char *) a_rcname, NULL, NULL);
	if( !tempFileName )
    {
        return FMOD_ERR_FILE_NOTFOUND;
    }
	result = FMOD_System_CreateSound(a_system, tempFileName, a_mode, a_finfo, a_ppsound);
    remove(tempFileName);
    return result;
}
FMOD_RESULT FMOD_System_CreateRcSoundNoDel(FMOD_SYSTEM *a_system, const char *a_rcname, FMOD_MODE a_mode, FMOD_CREATESOUNDEXINFO *a_finfo, FMOD_SOUND **a_ppsound)
{
	char *tempFileName;
	FMOD_RESULT	result;

	tempFileName = getTempResFile((char *) a_rcname, NULL, NULL);
	if( !tempFileName )
    {
        return FMOD_ERR_FILE_NOTFOUND;
    }
	result = FMOD_System_CreateSound(a_system, tempFileName, a_mode, a_finfo, a_ppsound);
    remove(tempFileName);
    return result;
}
#endif //DEF_RCFMODCORE

#ifdef DEF_RCSDL2TTF
TTF_Font *TTF_OpenRcFontNoDel(const char *a_FontName, int ptsize)
{
    TTF_Font *fontsToReturn;
	char *tempFileName;

	tempFileName = getTempResFile((char *) a_FontName, NULL, NULL);
	if (!tempFileName)
    {
        return NULL;
    }
	fontsToReturn = TTF_OpenFont(tempFileName, ptsize);
	return fontsToReturn;
}
#endif // DEF_RCSDL2FONTS

#ifdef DEF_RCSDL2IMAGE
SDL_Surface *IMG_RcLoad(const char *a_ImgName)
{
    SDL_Surface *surfaceToReturn;
	char *tempFileName;

	tempFileName = getTempResFile((char *) a_ImgName, NULL, NULL);
	if (!tempFileName)
    {
        return NULL;
    }
	surfaceToReturn = IMG_Load(tempFileName);
    remove(tempFileName);
    return surfaceToReturn;
}
#endif // DEF_RCSDL2IMAGE

#ifdef DEF_RCSDL2
SDL_Surface *LoadRcBitmap(const char* a_BmpName)
{
    SDL_Surface *surfaceToReturn;
	char *tempFileName;

	tempFileName = getTempResFile((char *) a_BmpName, NULL, NULL);
	if (!tempFileName)
    {
        return NULL;
    }
    surfaceToReturn = SDL_LoadBMP(tempFileName);
    remove(tempFileName);
    return surfaceToReturn;
}
#endif // DEF_RCSDL2

void setResourceContenerName(char *a_RcFileName)
{
    strcpy(gs_pNomFichierResources, a_RcFileName);
}

char *getResourceContenerName(void)
{
    return gs_pNomFichierResources;
}

void setPrefix(char *a_pNewPrefix)
{
    if (strlen(a_pNewPrefix) < 9)
    {
        strcpy(gs_rcTempPrefix, a_pNewPrefix);
    }
}

char *getPrefix(void)
{
    return(gs_rcTempPrefix);
}

int getErrorCode(void)
{
    return gs_nRcLoadusErrorCode;
}

//*******************************************************************************************************************
// fonction: bIsRcNameFound
//    objet: va rechercher dans le fichier RC si une ressource existe.
//        La ressource est connue avec son nom original de fichier, par exemple "monfic.bmp"
//    Arguments:
//        FILE *fp              : Le fichier de ressources doit être ouvert (fopen déjà effectué)
//        char *a_pResourceName : Le nom de la ressource à trouver, par exemple "monfic.bmp"
//        int *a_nRcOffset      : Pointeur sur un int. la fonction mettra dedans l'offset dans le fichier de ressources
//                                  correspondant aux data de la ressource concernée
//        int *a_nRcSize        : Contiendra la taille de la partie data de la ressource si elle est trouvée
//        a_nOrder              : Le numéro d'ordre de la ressource concernée, commence à zéro
//*******************************************************************************************************************
int bIsRcNameFound(FILE *fp, char *a_pResourceName, int *a_nRcOffset, int *a_nRcSize, int a_nOrder)
{
    int     offsetRcStruct;  // Contiendra le int qui définit l'offset dans le fichier de ressources pour trouver l'entête de la ressource considérée
    int     resourceSize;    // Contendra le nombre d'octets (la taille) de la ressource
    int     resourceNameLength; // Longeur du nom de la ressource sans le zéro final. par exemple, si c'est toto.bmp, contiendra 8
    char    resourceName[FILENAME_MAX];  //** Buffer qui recevra le nom de la ressource
    int     rc;
    // Se positionner au début du pointeur d'offset dans le fichier de ressources pour ce N° de ressource
    // fseek retourne zéro si c'est ok. Sinon je fais un return( 0 ) pour dire que l'on ne l'a pas trouvé
    //if( fseek( fp, (4+(4*a_nOrder)), SEEK_SET ) ) return( 0 );
    rc = fseek(fp, (4 + (4 * a_nOrder)) + 1, SEEK_SET);
    if (rc != 0) return 0;
    // Lire le déplacement de cette ressource
    rc = freadRcCrypted((unsigned char *)&offsetRcStruct, sizeof(int), 1, fp);
    if (rc != 1) return 0;
    // Se déplacer au début du nom de cette ressource dans le fichier de ressources
    rc = fseek(fp, offsetRcStruct, SEEK_SET);
    if (rc != 0) return 0;
    // Nous pointons maintenant sur la taille des datas de cette structure. La lire
    rc = freadRcCrypted( (unsigned char *)&resourceSize, sizeof(int), 1, fp );
    if (rc != 1) return 0;
    // Lire maintenant la taille du nom de cette ressource
    rc = freadRcCrypted( (unsigned char *)&resourceNameLength, sizeof(int), 1, fp );
    if (rc != 1) return 0;
    if (resourceNameLength >=  (int)(FILENAME_MAX - strlen(gs_rcTempPrefix))) return 0; // - strlen(gs_rcTempPrefix) car je vais préfixer avec "gs_rcTempPrefix" en créant le fichier temporaire
    // Lire maintenant le nom de ectte ressource
    rc = freadRcCrypted((unsigned char *)resourceName, resourceNameLength, 1, fp);
    if (rc != 1) return 0;
    // Mettre un zéro binaire pour terminer le string
    resourceName[resourceNameLength] = 0x00;
    // Comparer avec le nom de ressource demandé
    if (!strcmp(resourceName, a_pResourceName))
    {
        // Oui c'est la bonne ressource
        // Garder le déplacement sur las data de cette ressource
        *a_nRcOffset = ftell(fp);
        *a_nRcSize = resourceSize;
        return 1;
    }
    else
    {
        //** Non, le nom ne correspond pas
        *a_nRcOffset = -1;
        return 0;

    }
    return 1;
}

//***************************************************************************************************************************************************
// getTempResFile :
//********************
//        Fonction interne.
//          Elle va chercher dans le fichier de ressources gs_NomFichierResources et essayer de trouver la ressource dont le nom est dans a_pResName.
//        Arguments:
//          a_pResName  : Nom de la ressource, par exemple mymage.bmp
//          a_pnSize    : Pointeur sur un int, peut être NULL. Si pas NULL, ej mets dedans la taille trouvée pour al ressource
//          a_pnOffset  : Pointeur sur un int, offset de la ressource par rapport au début du fichier de ressources. peut être NULL
//
//        Valeurs de retour :
//              NULL        = Ressource non trouvée
//              non NULL    = Nom du fichier temporaire créé (et chargé et non-ouvert) si la ressource est trouvée
//***************************************************************************************************************************************************
char *getTempResFile(char *a_pResName, int *a_pnSize, int *a_pnOffset)
{
    FILE    *fp;
    int     nbFilesInRc;
    int     i;
    int     rcOffset;
    int     bFlagRcFound;
    int     rcSize;
    void    *ptrdata;
	FILE	*fpTemp;

    fp = fopen(gs_pNomFichierResources, "rb");
    if (!fp)
    {
        gs_nRcLoadusErrorCode = RC_FILE_NOT_FOUND;
        return NULL;
    }
    // Lire le 1er octet pour savoir si c'est crypté
    gs_bIsCrypted = 0;
    freadRcCrypted((unsigned char *)&gs_bIsCrypted, 1, 1, fp);
    // Combien de ressources dans ce fichier de ressources ?
    freadRcCrypted((unsigned char *)&nbFilesInRc, sizeof(int), 1, fp);

    // Si pas de fichiers, RC_FILE_EMPTY
    if (!nbFilesInRc)
    {
        fclose(fp);
        gs_nRcLoadusErrorCode = RC_FILE_EMPTY;
        return NULL;
    }
    // Balayer les noms des ressources
    bFlagRcFound  = 0;
    for (i = 0 ; i < nbFilesInRc ; i++)
    {
         if (bIsRcNameFound(fp, a_pResName, &rcOffset, &rcSize, i))
         {
             bFlagRcFound = 1;
             if(a_pnSize)
             {
                 *a_pnSize   = rcSize;
             }
             if(a_pnOffset)
             {
                 *a_pnOffset = rcOffset;
             }
             break;
         }
    }
    if (!bFlagRcFound)
    {
        fclose(fp);
        gs_nRcLoadusErrorCode = RC_RESOURCE_UNKNOWN;
        return NULL;
    }
    // Ici la ressource a été trouvée, rcOffset est le déplacement dans le fichier de ressources des data de la ressource cherchée
    // En attendant de faire mieux, je vais créer un fichier temporaire pour cette ressource, la charger, puis effacer le fichier
	strcpy(gs_pTempFileName, gs_rcTempPrefix);
    strcat(gs_pTempFileName, a_pResName);
    fpTemp = fopen(gs_pTempFileName, "w+b");
    ptrdata = malloc(rcSize);
    freadRcCrypted((unsigned char *)ptrdata, rcSize, 1, fp);
    fwrite(ptrdata, rcSize, 1, fpTemp);
    fclose(fpTemp);
    fclose(fp);
    free(ptrdata);
	return gs_pTempFileName;
}

//***************************************************************************************************************************************************
// closeRcFile :
//********************
//          Elle supprime les fichiers de ressources résiduels.
//        Arguments:
//          a_pResName  : Nom de la ressource, par exemple mymage.bmp // Passer NULL si on veut supprimer tout les fichiers résiduels.
//
//***************************************************************************************************************************************************
void closeRcFile(char *a_pResName)
{
    struct dirent *ent = NULL;
    DIR *dir = opendir(".");

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (a_pResName == NULL)
            {
                if (strncmp(ent->d_name, gs_rcTempPrefix, strlen(gs_rcTempPrefix)) == 0)
                {
                    remove(ent->d_name);
                }
            }
            else
            {
                strcpy(gs_pTempFileName, gs_rcTempPrefix);
                strcat(gs_pTempFileName, a_pResName);
                remove(gs_pTempFileName);
            }
        }
    }
}

int freadRcCrypted(unsigned char *a_buffer, int a_size, int a_nbElem, FILE *a_fp)
{
    int i;
	unsigned char temp;
	unsigned char l_cX;
	int  crypted;
	int  rc;

    crypted  = gs_bIsCrypted;
	rc = fread((void *)a_buffer, a_size, a_nbElem, a_fp);
    if (crypted > 0)
    {
        for (i = 0 ; i < (a_size * a_nbElem) ; i++)
        {
            l_cX = a_buffer[i]<<6;
            temp = a_buffer[i]>>2;
            a_buffer[i] = l_cX | temp;
        }
    }
    return rc;
}
