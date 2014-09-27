#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include "Jeu.h"


void finPartie(SDL_Rect * positionBalle,SDL_Rect * positionPlateau,Vitesse * Balle,SDL_Surface * ecran){

    Game_assets.gameover = IMG_Load("image/Gameover.png");
    optimize_surface(Game_assets.gameover);
    positionBalle->x = positionPlateau->x+40-(positionBalle->w/2);      //On reinitialise la position de la balle
    positionBalle->y = 728;                                             // et de certaines variable apres la mort
    Partie.lancer = 0;
    Partie.bonus = 0;

    if (Partie.vie == 1){
        SDL_BlitSurface(Game_assets.gameover,NULL,ecran,NULL);
        SDL_Flip(ecran);
        ecran_niveau(ecran);
        SDL_Quit();
    }else{
        Partie.vie -= 1;
    }

    Balle->x = (rand()%3)+1;
    Balle->y = sqrt(13 - pow(Balle->x,2));
    if ((rand()%2) == 0){
        Balle->x *= -1;
    }
}

void niveau_sup(SDL_Surface * ecran){       //Fonction similaire a jeu_deplacement mais adapte pour les niveaux autre que 1
    int continuer = 1,
        tempsPrecedent = 0,
        i,j;

    SDL_Rect positionPlateau;   //Rectangle du plateau
    SDL_Rect positionBalle;     //rectangle pour la balle
    SDL_Rect positionJeu;
    SDL_Rect positionVie;

    SDL_Event event;

    for (i=0;i<47;i++){
        for(j=0;j<63;j++){
            tab_Collision[j][i]=0;
        }
    }

    positionPlateau.x = (1024 / 2) - (80 / 2);      //Position au milieu de l'ecran
    positionPlateau.y = 736;
    positionPlateau.h = 16;
    positionPlateau.w = 80;
    positionBalle.x = (1024.0 / 2.0) - (8.0 / 2.0);         //Position au milieu de l'ecran
    positionBalle.y = 728.0;
    positionBalle.h = 10;
    positionBalle.w = 10;
    positionJeu.x = 16;
    positionJeu.y = 16;
    positionJeu.h = 752;
    positionJeu.w = 992;
    positionVie.x = 16;
    positionVie.y = 760;

    srand(time(NULL));

    Vitesse Balle;          //vitesse de la balle
        Balle.x = (rand()%3)+1;
        Balle.y = sqrt(13 - pow(Balle.x,2));
        if ((rand()%2) == 0){
            Balle.x *= -1;
        }

        Game_assets.fond = IMG_Load("image/accueil.png");
        optimize_surface(Game_assets.fond);
        Game_assets.plateau = IMG_Load("image/PlateauT.png");
        optimize_surface(Game_assets.plateau);                          //Chargement des images sur les surface
        Game_assets.balle = IMG_Load("image/BalleNeon.png");
        optimize_surface(Game_assets.balle);
        Game_assets.casebonus = IMG_Load("image/BonusGrand.png");
        optimize_surface(Game_assets.casebonus);
        Game_assets.imagevie = IMG_Load("image/vie.png");
        optimize_surface(Game_assets.imagevie);
        Game_assets.niveau = IMG_Load("image/EcranJeu.png");
        optimize_surface(Game_assets.niveau);

    ListBalle_sup[1].existe = 0;
    Partie.lancer = 0;

    demarrer(ecran);                                            //Affiche toutes les briques du bord du mur
    SDL_BlitSurface(Game_assets.plateau, NULL, ecran, &positionPlateau);    //Affiche la nouveau plateau
    SDL_BlitSurface(Game_assets.balle, NULL, ecran, &positionBalle);        //Affiche la nouvelle balle
    initialise_fichier();

    charger_niveau(ecran);

    SDL_Flip(ecran);

    for (i=0;i<47;i++){
        for(j=0;j<64;j++){
            printf("%d",tab_Collision[j][i]);
        }
        printf("\n");
    }


    SDL_EnableKeyRepeat(7, 7);
    int tempsActuel = 0;
    while (continuer)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            continuer = 0;
                            break;
                        case SDLK_RIGHT: // Flèche droite
                            if ((positionPlateau.x+positionPlateau.w+3) <= 1008){
                                positionPlateau.x += 3;
                                if (Partie.lancer == 0){       //Si la balle n'est pas lancee posBalle = posPlateau
                                positionBalle.x += 3;
                                }
                            }
                            break;
                        case SDLK_LEFT: // Flèche gauche
                            if ((positionPlateau.x-3)>= 16){
                                positionPlateau.x -= 3;
                                if (Partie.lancer == 0){       //Si la balle n'est pas lancee posBalle = posPlateau
                                positionBalle.x -= 3;
                                }
                            }
                            break;
                        case SDLK_UP:
                            Partie.lancer = 1; //On lance la balle
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 12) /* Si 12 ms se sont écoulées depuis le dernier tour de boucle */
        {
            if (Partie.lancer == 1){
                DeplaBalle(&positionBalle,&Balle,&positionPlateau,ecran); //toute les 15sec la balle boug
            }

            if (ListBalle_sup[1].existe == 1){
                DeplaBalle(&ListBalle_sup[1].positionBalle_sup,&ListBalle_sup[1].Balle,&positionPlateau,ecran);
                if (ListBalle_sup[1].positionBalle_sup.y > 760){
                    ListBalle_sup[1].existe = 0;
                }
            }
            if (Partie.bonus == 1 && positionPlateau.w != 160){
                SDL_FreeSurface(Game_assets.plateau);
                Game_assets.plateau = IMG_Load("image/PlateauTg.png");
                optimize_surface(Game_assets.plateau);
                positionPlateau.w = 160;
            }else if(positionPlateau.w != 80 && Partie.bonus != 1 && Partie.bonus != 6){
                SDL_FreeSurface(Game_assets.plateau);
                Game_assets.plateau = IMG_Load("image/PlateauT.png");
                optimize_surface(Game_assets.plateau);
                positionPlateau.w = 80;
            }

            if(Partie.bonus == 2){
                if (Partie.plateaustick == 0){
                    SDL_FreeSurface(Game_assets.plateau);
                    Game_assets.plateau = IMG_Load("image/PlateauTs.png");
                    optimize_surface(Game_assets.plateau);
                    Partie.plateaustick = 1;
                }
            }else if (Partie.plateaustick == 1){
                    SDL_FreeSurface(Game_assets.plateau);
                    Game_assets.plateau = IMG_Load("image/PlateauT.png");
                    optimize_surface(Game_assets.plateau);
                    Partie.plateaustick = 0;

            }

            if(Partie.bonus == 4){
                if (Partie.balleflamme == 0){
                    SDL_FreeSurface(Game_assets.balle);
                    Game_assets.balle = IMG_Load("image/BalleNeonF.png");
                    optimize_surface(Game_assets.balle);
                    Partie.balleflamme = 1;
                }
            }else if(Partie.balleflamme == 1){
                    SDL_FreeSurface(Game_assets.balle);
                    Game_assets.balle = IMG_Load("image/BalleNeon.png");
                    optimize_surface(Game_assets.balle);
                    Partie.balleflamme = 0;

            }

            if (Partie.bonus == 3){
                ListBalle_sup[1].positionBalle_sup.x = positionBalle.x;
                ListBalle_sup[1].positionBalle_sup.y = positionBalle.y;
                ListBalle_sup[1].Balle.x = -Balle.x;
                ListBalle_sup[1].Balle.y = Balle.y;
                Partie.bonus = 0;
                Partie.nbr_balle += 1;
            }

            if (Partie.bonus == 6){
                SDL_FreeSurface(Game_assets.plateau);
                Game_assets.plateau = IMG_Load("image/PlateauTp.png");
                optimize_surface(Game_assets.plateau);
                positionPlateau.w = 48;
            }else if(positionPlateau.w != 80 && Partie.bonus != 1){
                SDL_FreeSurface(Game_assets.plateau);
                Game_assets.plateau = IMG_Load("image/PlateauT.png");
                optimize_surface(Game_assets.plateau);
                positionPlateau.w = 80;
            }
            if (Partie.bonus == 7){
                finPartie(&positionBalle,&positionPlateau,&Balle,ecran);
                Partie.bonus = 0;
            }

            if(Partie.bonus == 5 || Partie.bonus == 8){     //Ici on accelere ou on freine la balle selon les bonus et des limites
                if(Partie.bonus == 5){
                    Balle.x = Balle.x/sqrt(1.5);
                    Balle.y = Balle.y/sqrt(1.5);
                    Partie.vitesse = 0;
                    Partie.bonus = 0;
                }else if(Partie.bonus == 8){
                    Balle.x = Balle.x*sqrt(1.5);
                    Balle.y = Balle.y*sqrt(1.5);
                    Partie.vitesse = 2;
                    Partie.bonus = 0;
                }
            }

            if(Partie.bonus != 5 && Partie.bonus != 8){     //Ici on accelere ou on freine la balle selon les bonus et des limites
                if(Partie.vitesse == 0){
                    Balle.x = Balle.x*sqrt(1.5);
                    Balle.y = Balle.y*sqrt(1.5);
                    Partie.vitesse = 1;
                }else if(Partie.vitesse == 2){
                    Balle.x = Balle.x/sqrt(1.5);
                    Balle.y = Balle.y/sqrt(1.5);
                    Partie.vitesse = 1;
                }
            }

            if (Partie.bonus == 7){
                finPartie(&positionBalle,&positionPlateau,&Balle,ecran);
            }

            SDL_BlitSurface(Game_assets.niveau,&positionJeu,ecran,&positionJeu);

            DeplaBonus(&positionPlateau,&Balle);

            tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */

            charger_niveau(ecran);
            SDL_BlitSurface(Game_assets.plateau, NULL, ecran, &positionPlateau);            //On affiche les evenements mise a jours : balle et plateau
            SDL_BlitSurface(Game_assets.balle, NULL, ecran, &positionBalle);

            if (ListBalle_sup[1].existe == 1){
                SDL_BlitSurface(Game_assets.balle, NULL, ecran, &ListBalle_sup[1].positionBalle_sup);
            }

            for (i=0;i<Partie.nbr_bonus;i++){
                if(ListBonus[i].positionBonus.x != 0){
                    SDL_BlitSurface(Game_assets.casebonus,&ListBonus[i].positionSprite,ecran,&ListBonus[i].positionBonus);
                }
            }

            for(i=0 ; i<Partie.vie ; i++){
                SDL_BlitSurface(Game_assets.imagevie,NULL,ecran,&positionVie);
                positionVie.x += 16;
            }
            positionVie.x = 16;

            SDL_Flip(ecran); //Toutes les 15sec mise a jour de l'ecran
        }else{
            SDL_Delay(1);
        }

        if(charger_niveau(ecran) == 1){
            SDL_FreeSurface(Game_assets.fond);
            SDL_FreeSurface(Game_assets.plateau);                          //Chargement des images sur les surface
            SDL_FreeSurface(Game_assets.balle);
            SDL_FreeSurface(Game_assets.casebonus);
            SDL_FreeSurface(Game_assets.imagevie);
            return;
        }
    }
    for (i=0;i<47;i++){
        for(j=0;j<64;j++){
            printf("%d",tab_Collision[j][i]);
        }
        printf("\n");
    }
    SDL_FreeSurface(ecran);
    SDL_Quit();

}

void ecran_niveau(SDL_Surface *ecran){      //Cette fonction demande juste a l'utilisateur une action avant de continuer une boucle
    int continuer;
    continuer = 1;

    SDL_Event event;

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
                    case SDLK_SPACE: /* Appui sur la touche Echap, on arrête le programme */
                            continuer = 0;
                            break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void init_surfaces(){
    Game_assets.ecran = malloc(sizeof(SDL_Surface*));
    Game_assets.imageDeFond = malloc(sizeof(SDL_Surface*));
    Game_assets.brique1 = malloc(sizeof(SDL_Surface*));      //Initialisation des surfaces affichant les diverses briques
    Game_assets.brique2 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique3 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique4 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique6 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique7 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique8 = malloc(sizeof(SDL_Surface*));
    Game_assets.brique9 = malloc(sizeof(SDL_Surface*));
    Game_assets.gameover = malloc(sizeof(SDL_Surface*));
    Game_assets.plateau = malloc(sizeof(SDL_Surface*));
    Game_assets.casebonus = malloc(sizeof(SDL_Surface*));
    Game_assets.fond = malloc(sizeof(SDL_Surface*));
    Game_assets.balle = malloc(sizeof(SDL_Surface*));
    Game_assets.imagevie = malloc(sizeof(SDL_Surface*));
    Game_assets.niveau = malloc(sizeof(SDL_Surface*));
    Game_assets.fin = malloc(sizeof(SDL_Surface*));
}

void optimize_surface(SDL_Surface * non_optimized){
    SDL_Surface * optimized = NULL;
    optimized = SDL_DisplayFormat(non_optimized);
    SDL_FreeSurface(optimized);
}
