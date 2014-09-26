#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Jeu.h"


int main(int argc, char *argv[])
{

    int continuer = 1;
    SDL_Event event;
    SDL_Rect positionFond;

    positionFond.x = 0;
    positionFond.y = 0;

    Partie.niveau = 1;
    Partie.is_loaded = 0;

    init_surfaces();

    SDL_Init(SDL_INIT_VIDEO);

    Game_assets.ecran = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Retro Breaker", NULL);
    Game_assets.imageDeFond = IMG_Load("image/accueil.png");
    SDL_BlitSurface(Game_assets.imageDeFond, NULL, Game_assets.ecran, &positionFond);
    Partie.vie = 3;


    SDL_Flip(Game_assets.ecran); /* Mise à jour de l'écran avec sa nouvelle couleur */

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE: /* Appui sur la touche Echap, on arrête le programme */
                        continuer = 0;
                        break;

                    case SDLK_SPACE: /* Appui sur la touche Echap, on arrête le programme */
                        jeu_deplacement(Game_assets.ecran);
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}
