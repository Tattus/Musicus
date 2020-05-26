/* fichierCredit.c
   ---------------

   Créé par : Tattus
   le : 15/04/2015

   Modifié le : 16/04/2017
   ---------------
   Fonctions de créations des crédits.

*/
#include "general.h"

/* Prototypes locaux */
void creditsFR(void); // Création du fichier de credits français.
void creditsEN(void); //Création du fichier de credits anglais.
void versionsFR(void); // Créé le fichier détaillant les versions en français.
void versionsEN(void); // Créé le fichier détaillant les versions en anglais.
void licenceSDL(void); // Création du fichier de licence de la SDL.
void licenceSDLTTF(void); // Création du fichier de licence de la SDL-TTF.
void licenceSDLImage(void); // Création du fichier de licence de la SDL-Image.
void liberationFontLicence(void); // Création du fichier de licence de la police Libération Font.
/* Fin Prototypes locaux */

void initialisationCredits()
{
    #ifdef LINUX
        mkdir("./Licenses", 0770);
    #else
        mkdir("./Licenses");
    #endif
    #ifdef LINUX
        mkdir("./Documentations", 0770);
    #else
        mkdir("./Documentations");
    #endif
    creditsFR();
    creditsEN();
    versionsFR();
    versionsEN();
    licenceSDL();
    licenceSDLTTF();
    licenceSDLImage();
    liberationFontLicence();
}

void creditsFR(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/FR_Credit-Musicus.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\n*** CREDITS ***\n\n");
        fprintf(fichier, "*** Musicus v%s ***\n\n", VERSION);
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nDéveloppement :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "\nGraphismes :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "\nSons :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nPolices :\n\n");
        fprintf(fichier, "_ LiberationMono-Bold.ttf (https://fedorahosted.org/liberation-fonts/) sous licence \"SIL Open Font License\".\n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nBibliothèques utilisées :\n\n");;
        fprintf(fichier, "_ SDL 2.0 sous licence zlib\n");
        fprintf(fichier, "_ SDL TTF sous licence zlib\n");
        fprintf(fichier, "_ zlib (via SDL TTF) sous licence zlib\n");
        fprintf(fichier, "_ SDL image sous licence zlib\n");
        fprintf(fichier, "_ PNG (via SDL Image) sous licence libpng\n");
        fprintf(fichier, "_ freetype (via SDL TTF) sous licence FreeType\n");
        fprintf(fichier, "copyright © 2013 The FreeType Project (www.freetype.org).  All rights reserved.\n");
        fprintf(fichier, "_ FMOD Studio low level\n");
        fprintf(fichier, "www.fmod.org/sales/\n");
        fprintf(fichier, "FMOD Studio, copyright © Firelight Technologies Pty, Ltd., 2012-2016.\n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nRedistribution :\n\n");
        fprintf(fichier, "La redistribution gratuite et sans modifications de Musicus v%s est autorisée.\n", VERSION);
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void creditsEN(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/EN_Credit-Musicus.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\n*** CREDITS ***\n\n");
        fprintf(fichier, "*** Musicus v%s ***\n\n", VERSION);
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nDevelopment :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "\nGraphics :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "\nSounds :\n\n");
        fprintf(fichier, "_ Tattus \n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nFonts :\n\n");
        fprintf(fichier, "_ LiberationMono-Bold.ttf (https://fedorahosted.org/liberation-fonts/) under \"SIL Open Font License\" license.\n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nLibraries used :\n\n");;
        fprintf(fichier, "_ SDL 2.0 under zlib license\n");
        fprintf(fichier, "_ SDL TTF under zlib license\n");
        fprintf(fichier, "_ zlib (via SDL TTF) under zlib license\n");
        fprintf(fichier, "_ SDL image under zlib license\n");
        fprintf(fichier, "_ PNG (via SDL Image) under libpng license\n");
        fprintf(fichier, "_ freetype (via SDL TTF) under FreeType licence\n");
        fprintf(fichier, "copyright © 2013 The FreeType Project (www.freetype.org).  All rights reserved.\n");
        fprintf(fichier, "_ FMOD Studio low level\n");
        fprintf(fichier, "www.fmod.org/sales/\n");
        fprintf(fichier, "FMOD Studio, copyright © Firelight Technologies Pty, Ltd., 2012-2016.\n\n");
        fprintf(fichier, "--------------------\n");
        fprintf(fichier, "\nRedistribution :\n\n");
        fprintf(fichier, "Free redistribution without modifications of Musicus v%s is allowed.\n", VERSION);
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void versionsFR(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Documentations/FR_Versions.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\n*** Version %s ***\n", VERSION);
        fprintf(fichier, "--------------------\n\n");
        fprintf(fichier, "=> Optimisations :\n");
        fprintf(fichier, "\t-> Temps de chargement réduit\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void versionsEN(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Documentations/EN_Versions.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\n*** Version %s ***\n", VERSION);
        fprintf(fichier, "--------------------\n\n");
        fprintf(fichier, "=> Optimizations :\n");
        fprintf(fichier, "\t-> Loading time reduction\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void licenceSDL(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/README-SDL.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\nPlease distribute this file with the SDL runtime environment:\n\n");
        fprintf(fichier, "The Simple DirectMedia Layer (SDL for short) is a cross-platfrom library\n");
        fprintf(fichier, "designed to make it easy to write multi-media software, such as games and\n");
        fprintf(fichier, "emulators.\n\n");
        fprintf(fichier, "The Simple DirectMedia Layer library source code is available from:\n");
        fprintf(fichier, "http://www.libsdl.org/\n\n");
        fprintf(fichier, "This library is distributed under the terms of the zlib license:\n");
        fprintf(fichier, "http://www.zlib.net/zlib_license.html\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void licenceSDLTTF(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/README-SDL_TTF.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\nSDL_ttf:  A companion library to SDL for working with TrueType (tm) fonts\n");
        fprintf(fichier, "Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>\n\n");
        fprintf(fichier, "https://www.libsdl.org/projects/SDL_ttf/\n\n");
        fprintf(fichier, "Portions of this software are copyright © 2013 The FreeType Project (www.freetype.org).  All rights reserved.\n");
        fprintf(fichier, "\thttp://www.freetype.org/\n\n");
        fprintf(fichier, "This library is distributed under the terms of the zlib license:\n");
        fprintf(fichier, "http://www.zlib.net/zlib_license.html\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void licenceSDLImage(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/README-SDL_Image.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "\nSDL_image:  An example image loading library for use with SDL\n");
        fprintf(fichier, "Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>\n\n");
        fprintf(fichier, "https://www.libsdl.org/projects/SDL_image/\n\n");
        fprintf(fichier, "This library is distributed under the terms of the zlib license:\n");
        fprintf(fichier, "http://www.zlib.net/zlib_license.html\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}

void liberationFontLicence(void)
{
    FILE *fichier = NULL;

    fichier = fopen("Licenses/Liberation_Fonts-ttf.License.txt", "w+");
    if (fichier != NULL)
    {
        // Rédaction du fichier.
        fprintf(fichier, "Digitized data copyright (c) 2010 Google Corporation\n");
        fprintf(fichier, "	with Reserved Font Arimo, Tinos and Cousine.\n");
        fprintf(fichier, "Copyright (c) 2012 Red Hat, Inc.\n");
        fprintf(fichier, "	with Reserved Font Name Liberation.\n");
        fprintf(fichier, "This Font Software is licensed under the SIL Open Font License,\n");
        fprintf(fichier, "Version 1.1.\n\n");
        fprintf(fichier, "This license is copied below, and is also available with a FAQ at:\n");
        fprintf(fichier, "http://scripts.sil.org/OFL\n\n");
        fprintf(fichier, "SIL OPEN FONT LICENSE Version 1.1 - 26 February 2007\n\n");
        fprintf(fichier, "PREAMBLE The goals of the Open Font License (OFL) are to stimulate\n");
        fprintf(fichier, "worldwide development of collaborative font projects, to support the font\n");
        fprintf(fichier, "creation efforts of academic and linguistic communities, and to provide\n");
        fprintf(fichier, "a free and open framework in which fonts may be shared and improved in\n");
        fprintf(fichier, "partnership with others.\n\n");
        fprintf(fichier, "The OFL allows the licensed fonts to be used, studied, modified and\n");
        fprintf(fichier, "redistributed freely as long as they are not sold by themselves.\n");
        fprintf(fichier, "The fonts, including any derivative works, can be bundled, embedded,\n");
        fprintf(fichier, "redistributed and/or sold with any software provided that any reserved\n");
        fprintf(fichier, "names are not used by derivative works.  The fonts and derivatives,\n");
        fprintf(fichier, "however, cannot be released under any other type of license.  The\n");
        fprintf(fichier, "requirement for fonts to remain under this license does not apply to\n");
        fprintf(fichier, "any document created using the fonts or their derivatives.\n\n\n\n");
        fprintf(fichier, "DEFINITIONS\n");
        fprintf(fichier, "\"Font Software\" refers to the set of files released by the Copyright\n");
        fprintf(fichier, "Holder(s) under this license and clearly marked as such.\n");
        fprintf(fichier, "This may include source files, build scripts and documentation.\n\n");
        fprintf(fichier, "\"Reserved Font Name\" refers to any names specified as such after the\n");
        fprintf(fichier, "copyright statement(s).\n\n");
        fprintf(fichier, "\"Original Version\" refers to the collection of Font Software components\n");
        fprintf(fichier, "as distributed by the Copyright Holder(s).\n\n");
        fprintf(fichier, "\"Modified Version\" refers to any derivative made by adding to, deleting,\n");
        fprintf(fichier, "or substituting ? in part or in whole ?\n");
        fprintf(fichier, "any of the components of the Original Version, by changing formats or\n");
        fprintf(fichier, "by porting the Font Software to a new environment.\n\n");
        fprintf(fichier, "\"Author\" refers to any designer, engineer, programmer, technical writer\n");
        fprintf(fichier, "or other person who contributed to the Font Software.\n\n\n");
        fprintf(fichier, "PERMISSION & CONDITIONS\n\n");
        fprintf(fichier, "Permission is hereby granted, free of charge, to any person obtaining a\n");
        fprintf(fichier, "copy of the Font Software, to use, study, copy, merge, embed, modify,\n");
        fprintf(fichier, "redistribute, and sell modified and unmodified copies of the Font\n");
        fprintf(fichier, "Software, subject to the following conditions:\n\n");
        fprintf(fichier, "1) Neither the Font Software nor any of its individual components,in\n");
        fprintf(fichier, "   Original or Modified Versions, may be sold by itself.\n\n");
        fprintf(fichier, "2) Original or Modified Versions of the Font Software may be bundled,\n");
        fprintf(fichier, "   redistributed and/or sold with any software, provided that each copy\n");
        fprintf(fichier, "   contains the above copyright notice and this license. These can be\n");
        fprintf(fichier, "   included either as stand-alone text files, human-readable headers or\n");
        fprintf(fichier, "   in the appropriate machine-readable metadata fields within text or\n");
        fprintf(fichier, "   binary files as long as those fields can be easily viewed by the user.\n\n");
        fprintf(fichier, "3) No Modified Version of the Font Software may use the Reserved Font\n");
        fprintf(fichier, "   Name(s) unless explicit written permission is granted by the\n");
        fprintf(fichier, "   corresponding Copyright Holder. This restriction only applies to the\n");
        fprintf(fichier, "   primary font name as presented to the users.\n\n");
        fprintf(fichier, "4) The name(s) of the Copyright Holder(s) or the Author(s) of the Font\n");
        fprintf(fichier, "   Software shall not be used to promote, endorse or advertise any\n");
        fprintf(fichier, "   Modified Version, except to acknowledge the contribution(s) of the\n");
        fprintf(fichier, "   Copyright Holder(s) and the Author(s) or with their explicit written\n");
        fprintf(fichier, "   permission.\n\n");
        fprintf(fichier, "5) The Font Software, modified or unmodified, in part or in whole, must\n");
        fprintf(fichier, "   be distributed entirely under this license, and must not be distributed\n");
        fprintf(fichier, "   under any other license. The requirement for fonts to remain under\n");
        fprintf(fichier, "   this license does not apply to any document created using the Font\n");
        fprintf(fichier, "   Software.\n\n\n\n");
        fprintf(fichier, "TERMINATION\n");
        fprintf(fichier, "This license becomes null and void if any of the above conditions are not met.\n\n\n\n");
        fprintf(fichier, "DISCLAIMER\n");
        fprintf(fichier, "THE FONT SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n");
        fprintf(fichier, "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OF\n");
        fprintf(fichier, "MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT\n");
        fprintf(fichier, "OF COPYRIGHT, PATENT, TRADEMARK, OR OTHER RIGHT.  IN NO EVENT SHALL THE\n");
        fprintf(fichier, "COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n");
        fprintf(fichier, "INCLUDING ANY GENERAL, SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL\n");
        fprintf(fichier, "DAMAGES, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING\n");
        fprintf(fichier, "FROM, OUT OF THE USE OR INABILITY TO USE THE FONT SOFTWARE OR FROM OTHER\n");
        fprintf(fichier, "DEALINGS IN THE FONT SOFTWARE.\n\n\n");
    }
    else
    {
        fprintf(stderr, "Erreur d'initialisation.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fichier);
}
