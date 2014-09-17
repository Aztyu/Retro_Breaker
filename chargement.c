#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Jeu.h"

void initialise_fichier(){

    char lvl[15] = {0};

    Partie.nbr_bonus = 0;                   //Reinitialisation du nombre de bonus tombes

    ListBonus[0].positionBonus.h = 14;       //Un bonus a une taille de 32 par 14 pixels
    ListBonus[0].positionBonus.h = 32;

    FILE*fichier=NULL;

    snprintf(lvl,30, "niveau/lvl%d.txt", Partie.niveau);   //Selon la variable niveau on initialise un fichier different
    fichier = fopen(lvl,"r");

    if (fichier!=NULL){
        int i,j=0,buffer,x=0,y=0;
        for (i=0;i<44;i++)
        {
            y++;
            for(j=0;j<32;j++){                  //Double boucle qui initialise les case du tableau des collisions selon le fichier txt
                buffer=fgetc(fichier);
                switch (buffer){
                case '1':
                    x++;
                    tab_Collision [x][y] = 1;
                    x++;
                    tab_Collision [x][y] = 1;
                    Partie.brique += 1;
                    break;
                case '2':
                    x++;
                    tab_Collision [x][y] = 2;
                    x++;
                    tab_Collision [x][y] = 2;
                    Partie.brique += 1;
                    break;
                case '3':
                    x++;
                    tab_Collision [x][y] = 3;
                    x++;
                    tab_Collision [x][y] = 3;
                    Partie.brique += 1;
                    break;
                case '4':
                    x++;
                    tab_Collision [x][y] = 4;
                    x++;
                    tab_Collision [x][y] = 4;
                    Partie.brique += 1;
                    break;
                case '6':
                    x++;
                    tab_Collision [x][y] = 6;
                    x++;
                    tab_Collision [x][y] = 6;
                    Partie.brique += 1;
                    break;
                case '7':
                    x++;
                    tab_Collision [x][y] = 7;
                    x++;
                    tab_Collision [x][y] = 7;
                    Partie.brique += 1;
                    break;
                case '8':
                    x++;
                    tab_Collision [x][y] = 8;
                    x++;
                    tab_Collision [x][y] = 8;
                    Partie.brique += 1;
                    break;
                case '9':
                    x++;
                    tab_Collision [x][y] = 9;
                    x++;
                    tab_Collision [x][y] = 9;
                    break;
                default:
                    x++;
                    x++;
                    break;
                }

            }
            x = 0;
        }
    }else{
        printf("Chargement impossible...\n");
        printf("Demarrage d'une nouvelle partie...\n");
    }
    fclose(fichier);
}

int charger_niveau(SDL_Surface * ecran){
    int y = 0,i = 0,j = 0;
    SDL_Surface *brique1 = NULL,        //Initialisation des surfaces affichant les diverses briques
                *brique2 = NULL,
                *brique3 = NULL,
                *brique4 = NULL,
                *brique6 = NULL,
                *brique7 = NULL,
                *brique8 = NULL,
                *brique9 = NULL;

    SDL_Rect positionBrique;

    Partie.brique = 0;

    positionBrique.x = 16;              //L'aire de jeu pouvant etre modifiee commence a 16/16
    positionBrique.y = 16;

    brique1 = IMG_Load("brique/1.png");     //Chargement des image en .png dans les surfaces
    brique2 = IMG_Load("brique/2.png");
    brique3 = IMG_Load("brique/3.png");
    brique4 = IMG_Load("brique/4.png");
    brique6 = IMG_Load("brique/6.png");
    brique7 = IMG_Load("brique/7.png");
    brique8 = IMG_Load("brique/8.png");
    brique9 = IMG_Load("brique/9.png");

    for (i=1;i<45;i++)
    {
        y++;
        positionBrique.x = 16;
        for(j=1;j<63;j++){
            switch(tab_Collision [j][i]){           //Selon la valeur contenu dans le tableau on affiche la brique correspondante
            case 1:
                SDL_BlitSurface(brique1, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 2:
                SDL_BlitSurface(brique2, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 3:
                SDL_BlitSurface(brique3, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 4:
                SDL_BlitSurface(brique4, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 6:
                SDL_BlitSurface(brique6, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 7:
                SDL_BlitSurface(brique7, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 8:
                SDL_BlitSurface(brique8, NULL, ecran, &positionBrique);
                Partie.brique += 1;
                break;
            case 9:
                SDL_BlitSurface(brique9, NULL, ecran, &positionBrique);
                break;
            default:
                break;
            }
            positionBrique.x += 32;  //Une case du tableau est egale a 16*16 mais une brique vaut 32*16
            j++;
        }
        positionBrique.x = 0;       //Retour a la ligne
        positionBrique.y += 16;
    }
    if (Partie.brique == 0){
        Partie.niveau += 1;
        return 1;
    }
    SDL_FreeSurface(brique1);
    SDL_FreeSurface(brique2);
    SDL_FreeSurface(brique3);
    SDL_FreeSurface(brique4);
    SDL_FreeSurface(brique6);
    SDL_FreeSurface(brique7);
    SDL_FreeSurface(brique8);
    SDL_FreeSurface(brique9);
    return 0;
}
