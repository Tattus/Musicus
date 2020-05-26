/* collisions.c
   ------------

   Créé par Tattus
   le : 21/03/2015

   Modifié le : 05/04/2015
   ------------
   Fonctions qui gèrent les collisions.
   Nécessite la SDL 2.
*/
#include "collisions.h"

/* Fonction qui gère la collision d'un point sur une surface. */
int collisionPoint_AABB(int point_x, int point_y, SDL_Rect AABB)
{
    if (point_x >= AABB.x // Si le point se trouve a droite du bord gauche de la surface...
        && point_x < AABB.x + AABB.w // ... mais aussi à gauche du bord droit de la surface...
        && point_y >= AABB.y // ... et que le point se trouve en dessous du bord haut de la surface...
        && point_y < AABB.y + AABB.h) // ainsi qu'au dessus du bord bas de la surface.
    {
        return 1; // Touche (VRAI).
    }
    else
    {
        return 0; // Touche pas (FAUX).
    }
}

/* Fonction qui gère la collision d'une surface sur une surface (Rectangle et Rectangle). */
int collisionAABB_AABB(SDL_Rect AABB1, SDL_Rect AABB2)
{
    if (AABB2.x >= AABB1.x + AABB1.w // Si trop à droite...
        || AABB2.x + AABB2.w <= AABB1.x // ...ou trop à gauche.
        || AABB2.y >= AABB1.y + AABB1.h // ...ou trop en bas.
        || AABB2.y + AABB2.h <= AABB1.y) // ...ou trop en haut.
    {
        return 0; // Touche pas (FAUX).
    }
    else
    {
        return 1; // Touche (VRAI).
    }
}

/* Fonction qui gère la collision d'un point dans un cercle. */
int collisionPoint_Cercle(int x, int y, Cercle cercle)
{
    int distance2 = (x - cercle.x) * (x - cercle.x) + (y - cercle.y) * (y - cercle.y); // Calcul de la distance au carré entre le point et le centre du cercle.
    if (distance2 > cercle.rayon * cercle.rayon) // Si la distance au carré est supérieure au rayon au carré...
    {
        return 0; // Touche pas (FAUX).
    }
    else
    {
        return 1; // Touche (VRAI).
    }
}

/* Fonction qui gère la collision d'un cerle sur un autre cercle. */
int collisionCercle_Cercle(Cercle cercle1, Cercle cercle2)
{
    int distance2 = (cercle1.x - cercle2.x) * (cercle1.x - cercle2.x) + (cercle1.y - cercle2.y) * (cercle1.y - cercle2.y); // Calcul de la distance au carré entre les centres des cercles.
    if (distance2 > (cercle1.rayon + cercle2.rayon) * (cercle1.rayon + cercle2.rayon)) // Si la distance au carré est supérieure à la somme des rayons des deux cercles au carré.
    {
        return 0; // Touche pas (FAUX).
    }
    else
    {
        return 1; // Touche (VRAI).
    }
}
