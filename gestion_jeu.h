#ifndef GESTION_JEU_H_INCLUDED
#define GESTION_JEU_H_INCLUDED

void finPartie(SDL_Rect * positionBalle,SDL_Rect * positionPlateau,Vitesse * Balle,SDL_Surface * ecran);
void niveau_sup(SDL_Surface * ecran);
void init_surfaces();
void optimize_surface(SDL_Surface * non_optimized);

#endif
