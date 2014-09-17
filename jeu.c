#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <time.h>
#include "Jeu.h"
#include "chargement.h"
#include "gestion_jeu.h"


void demarrer(SDL_Surface * ecran){
    int i = 0;
    SDL_Surface *mur = NULL,*fond = NULL;
    SDL_Rect positionMur;      //creation du rectangle utilise pour le placement des murs

    //Partie.bonus = 4;

    positionMur.x = 0;
    positionMur.y = 0;

    fond = IMG_Load("image/ecranjeu.png");
    //SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    mur = IMG_Load("brique/Briquelim.png"); //charge l'image de la brique

    for (i=0;i<48;i++){                                                    //boucles qui permettent de placer toutes les briques autour de l'ecran
        SDL_BlitSurface(mur, NULL, ecran, &positionMur);
        positionMur.y += 16;
        tab_Collision [0][i]=1;
    }
    positionMur.y = 0;
    for (i=0;i<63;i++){
        SDL_BlitSurface(mur, NULL, ecran, &positionMur);
        positionMur.x += 16;
        tab_Collision [i][0]=1;
    }
    for (i=0;i<48;i++){
        SDL_BlitSurface(mur, NULL, ecran, &positionMur);
        positionMur.y += 16;
        tab_Collision [63][i]=1;
    }
    SDL_FreeSurface(fond);
    SDL_FreeSurface(mur);
}



void jeu_deplacement(SDL_Surface * ecran){
    int continuer = 1,
        tempsPrecedent = 0,
        tempsActuel = 0,
        i,j;

    SDL_Rect positionPlateau;   //Rectangle du plateau
    SDL_Rect positionBalle;     //rectangle pour la balle
    SDL_Rect positionJeu;
    SDL_Rect positionVie;


    SDL_Surface *plateau = NULL,
                *casebonus = NULL,
                *fond = NULL,
                *balle = NULL,
                *imagevie = NULL,
                *niveau = NULL,
                *fin = NULL;

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
    //printf("%lf\n",(pow(Balle.x,2)+pow(Balle.y,2)));

    ListBalle_sup[1].existe = 0;

        plateau = IMG_Load("image/PlateauT.png");                          //Chargement des images sur les surface
        balle = IMG_Load("image/BalleNeon.png");
        casebonus = IMG_Load("image/BonusGrand.png");
        imagevie = IMG_Load("image/vie.png");
        fond = IMG_Load("image/ecranjeu.png");
        niveau = IMG_Load("image/imageniveau.png");
        fin = IMG_Load("image/fin.png");

    demarrer(ecran);                                            //Affiche toutes les briques du bord du mur
    SDL_BlitSurface(plateau, NULL, ecran, &positionPlateau);    //Affiche la nouveau plateau
    SDL_BlitSurface(balle, NULL, ecran, &positionBalle);        //Affiche la nouvelle balle
    initialise_fichier();

    charger_niveau(ecran);

    SDL_Flip(ecran);


    SDL_EnableKeyRepeat(7, 7);  //Enfoncer une touche compte comme un appui

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
                            if ((positionPlateau.x+positionPlateau.w+4) <= 1008){          //On definit les limites de la position barre
                                positionPlateau.x += 4;                             //Alors on deplace la barre
                                if (Partie.lancer == 0){       //Si la balle n'est pas lancee posBalle = posPlateau
                                positionBalle.x += 4;
                                }
                            }
                            break;
                        case SDLK_LEFT: // Flèche gauche
                            if ((positionPlateau.x-4)>= 16){
                                positionPlateau.x -= 4;
                                if (Partie.lancer == 0){       //Si la balle n'est pas lancee posBalle = posPlateau
                                positionBalle.x -= 4;
                                }
                            }
                            break;
                        case SDLK_UP:
                            Partie.lancer = 1; //On lance la balle
                            break;
                        case SDLK_F1:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =1;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F2:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype = 2;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F3:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =3;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F4:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =4;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F5:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =1;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 5;
                            break;
                        case SDLK_F6:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype = 6;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F7:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =7;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                        case SDLK_F8:
                            ListBonus[Partie.nbr_bonus].positionBonus.x = 512;
                            ListBonus[Partie.nbr_bonus].positionBonus.y = 200;
                            ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
                            ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


                            ListBonus[Partie.nbr_bonus].bonustype =8;
                            ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
                            ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
                            Partie.nbr_bonus += 1;
                            break;
                    }
                    break;
            }
        }

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent > 12) /* Si 12 ms se sont écoulées depuis le dernier tour de boucle */
        {
            if (Partie.lancer == 1){
                DeplaBalle(&positionBalle,&Balle,&positionPlateau,ecran); //toute les 15sec la balle boug
            }

            if (ListBalle_sup[1].existe == 1){      //Si la balle a ete initialisee apres contact avec un bonus
                DeplaBalle(&ListBalle_sup[1].positionBalle_sup,&ListBalle_sup[1].Balle,&positionPlateau,ecran); //Alors on la deplace
                if (ListBalle_sup[1].positionBalle_sup.y > 760){ //Si elle sort de l'ecran ,on la supprime
                    ListBalle_sup[1].existe = 0;
                    //printf("test\n");
                }
            }
            if (Partie.bonus == 1 && positionPlateau.w != 160){ //Si le bonus grand plateau est actif est le plateau trop petit alors
                SDL_FreeSurface(plateau);
                plateau = IMG_Load("image/PlateauTg.png");   //On change le sprite et la variable
                positionPlateau.w = 160;
                //printf("aggrandi\n");
            }else if(positionPlateau.w != 80 && Partie.bonus != 1 && Partie.bonus != 6){
                SDL_FreeSurface(plateau);
                plateau = IMG_Load("image/PlateauT.png");          //Si le bonus n'est plus actif, alors la variable redevient normale
                positionPlateau.w = 80;
                //printf("retrecit/n");
            }

            if(Partie.bonus == 2){
                if (Partie.plateaustick == 0){      //Si la balle colante est active on change le sprite
                    SDL_FreeSurface(plateau);
                    plateau = IMG_Load("image/PlateauTs.png");
                    Partie.plateaustick = 1;
                }
            }else if (Partie.plateaustick == 1){
                    SDL_FreeSurface(plateau);
                    plateau = IMG_Load("image/PlateauT.png");
                    Partie.plateaustick = 0;

            }

            if(Partie.bonus == 4){
                if (Partie.balleflamme == 0){           //Selon si la balle est en flamme(explosive) ou non on change le sprite
                    SDL_FreeSurface(balle);
                    balle = IMG_Load("image/BalleNeonF.png");
                    Partie.balleflamme = 1;
                }
            }else if(Partie.balleflamme == 1){
                    SDL_FreeSurface(balle);
                    balle = IMG_Load("image/BalleNeon.png");
                    Partie.balleflamme = 0;

            }

            if (Partie.bonus == 3){         //On initialise la Balle supplementaire
                ListBalle_sup[1].positionBalle_sup.x = positionBalle.x;
                ListBalle_sup[1].positionBalle_sup.y = positionBalle.y;
                ListBalle_sup[1].Balle.x = -Balle.x;
                ListBalle_sup[1].Balle.y = Balle.y;
                ListBalle_sup[1].existe = 1;
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

            if (Partie.bonus == 6){             //Selon le bonus ,on change la taille de la barre
                SDL_FreeSurface(plateau);
                plateau = IMG_Load("image/PlateauTp.png");
                positionPlateau.w = 48;
            }else if(positionPlateau.w != 80 && Partie.bonus != 1){
                SDL_FreeSurface(plateau);
                plateau = IMG_Load("image/PlateauT.png");
                positionPlateau.w = 80;
            }
            if (Partie.bonus == 7){             //Si le bonus est l'instakill on lance la perte d'une vie
                finPartie(&positionBalle,&positionPlateau,&Balle,ecran);
                Partie.bonus = 0;
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
            tempsPrecedent = tempsActuel; /* Le temps "actuel" devient le temps "precedent" pour nos futurs calculs */

            DeplaBonus(&positionPlateau,&Balle);        //On deplace tous les bonus affiches a l'ecran
            SDL_BlitSurface(fond,&positionJeu,ecran,&positionJeu);      //On reaffiche tous l'ecran
            charger_niveau(ecran);
            SDL_BlitSurface(plateau, NULL, ecran, &positionPlateau);            //On affiche les evenements mise a jours : balle et plateau
            SDL_BlitSurface(balle, NULL, ecran, &positionBalle);

            if (ListBalle_sup[1].existe == 1){
                SDL_BlitSurface(balle, NULL, ecran, &ListBalle_sup[1].positionBalle_sup);
            }

            for (i=0;i<Partie.nbr_bonus;i++){
                if(ListBonus[i].positionBonus.x != 0){
                    SDL_BlitSurface(casebonus,&ListBonus[i].positionSprite,ecran,&ListBonus[i].positionBonus);
                }
            }

            for(i=0 ; i<Partie.vie ; i++){
                SDL_BlitSurface(imagevie,NULL,ecran,&positionVie);
                positionVie.x += 16;
            }
            positionVie.x = 16;

            if (Partie.niveau > 1){         // Cette fonction gere les niveaux

                SDL_FreeSurface(fond);
                SDL_FreeSurface(plateau);
                SDL_FreeSurface(balle);
                SDL_FreeSurface(casebonus);
                SDL_FreeSurface(imagevie);      //Dechargement des images

                SDL_BlitSurface(niveau,NULL,ecran,NULL); //ecran de fin de niveau
                SDL_Flip(ecran);
                ecran_niveau(ecran);                    //Continuation des niveaux
                niveau_sup(ecran);

                SDL_BlitSurface(niveau,NULL,ecran,NULL);
                SDL_Flip(ecran);
                ecran_niveau(ecran);
                niveau_sup(ecran);

                SDL_BlitSurface(niveau,NULL,ecran,NULL);
                SDL_Flip(ecran);
                ecran_niveau(ecran);
                niveau_sup(ecran);

                SDL_BlitSurface(niveau,NULL,ecran,NULL);
                SDL_Flip(ecran);
                ecran_niveau(ecran);
                niveau_sup(ecran);

                SDL_BlitSurface(fin,NULL,ecran,NULL);
                SDL_Flip(ecran);
                ecran_niveau(ecran);
                SDL_Quit();

            }

            SDL_Flip(ecran); //Toutes les 15sec mise a jour de l'ecran
        }else{
            SDL_Delay(1);
        }
    }
    SDL_FreeSurface(ecran);
    SDL_Quit();

}



void DeplaBalle(SDL_Rect * positionBalle,Vitesse * Balle,SDL_Rect * positionPlateau,SDL_Surface * ecran){
    int testy ,testx,testyopp,testxopp;
       //Cette colision ne marche que sur les cotes ,il faut en trouver une autre pour des brique aleatoire sur l'ecran


    testy = positionBalle->y - Balle->y;  //future position de la balle en y
    testx = positionBalle->x - Balle->x;  //future position de la balle en x
    testyopp = positionBalle->y - Balle->y + positionBalle->h;
    testxopp = positionBalle->x - Balle->x + positionBalle->w;

    if(positionBalle->y > 740){     //Ici la balle tombe trop bas
        finPartie(&*positionBalle,&*positionPlateau,&*Balle,ecran);
    }else{
        int sommecollision = 0;
        if(verifCollision(testx,testy)){        //On verifie la collision de tous les angles de la balle
            sommecollision += 1;
        }
        if(verifCollision(testxopp,testy)){
            sommecollision += 10;
        }
        if(verifCollision(testxopp,testyopp)){
            sommecollision += 100;
        }
        if(verifCollision(testx,testyopp)){
            sommecollision += 1000;
        }

        if (sommecollision != 0){
            switch(sommecollision){         //Selon la somme des collisions ,on change le deplacement de la balle
            case 1100:
                Balle->y *= -1.0;
                casseBrique(testx,testyopp,testxopp,testyopp,ecran);
                break;
            case 11:
                Balle->y *= -1.0;
                casseBrique(testx,testy,testxopp,testy,ecran);
                break;
            case 110:
                Balle->x *= -1.0;
                casseBrique(testxopp,testy,testxopp,testyopp,ecran);
                break;
            case 1001:
                Balle->x *= -1.0;
                casseBrique(testx,testy,testx,testyopp,ecran);
                break;
            case 1:
                if (Balle->x > 0){
                    if (Balle->y > 0){
                        Balle->x *= -1.0;
                    }else{
                        Balle->x *= -1.0;
                        Balle->y *= -1.0;
                    }
                }else{
                    Balle->y *= -1.0;
                }
                casseBrique(testx,testy,testx,testy,ecran);
                break;
            case 10:
                if (Balle->x > 0){
                    Balle->y *= -1.0;
                }else{
                    if (Balle->y > 0){
                        Balle->y *= -1.0;
                        Balle->x *= -1.0;
                    }else{
                        Balle->x *= -1.0;
                    }

                }
                casseBrique(testxopp,testy,testxopp,testy,ecran);
                Partie.brique -= 1;
                break;
            case 100:
                if (Balle->x > 0){
                    Balle->y *= -1.0;
                }else{
                    if (Balle->y > 0){
                        Balle->x *= -1.0;
                    }else{
                        Balle->x *= -1.0;
                        Balle->y *= -1.0;
                    }

                }
                casseBrique(testxopp,testyopp,testxopp,testyopp,ecran);
                Partie.brique -= 1;
                break;
            case 1000:
                if (Balle->x > 0){
                    if (Balle->y > 0){
                        Balle->x *= -1.0;
                        Balle->y *= -1.0;
                    }else{
                        Balle->x *= -1.0;
                    }
                }else{
                    Balle->y *= -1.0;
                }
                casseBrique(testx,testyopp,testx,testyopp,ecran);
                break;
            default:
                Balle->x *= -1.0;
                Balle->y *= -1.0;
                casseBrique(testx,testy,testx,testyopp,ecran);
                break;
            }
        }
        double ecart,vitesse,powx,vmax;
        if ((testy > 728) && ((testx <= (positionPlateau->x + positionPlateau->w )) && (testxopp >= positionPlateau->x))) //Detection de collision avec le plateau
        {
            vitesse = pow(Balle->x,2) + pow(Balle->y,2);    //Ici on definit la vitesse de la balle
            vmax = vitesse - 1.5;                    //et la vitesse maximale en un axe

            if (((testx + ((positionBalle->w)/2)) - positionPlateau->x ) >= (positionPlateau->w / 2)){  //Premiere moitie de la barre
                ecart = (testx + ((positionBalle->w)/2) - positionPlateau->x -(positionPlateau->w / 2));
                ecart *= 0.2/(positionPlateau->w / 2);
                ecart += 0.88;
                Balle->x *= ecart;      //On cqlcule la nouvelle vitesse en x
                powx = pow(Balle->x,2);
                if (powx >= vmax){          //Si on sort des limites alors il faut baisser la vitesse
                    Balle->x = sqrt(vmax);
                    powx = pow(Balle->x,2);
                }

                if(Balle->x > 0){
                        Balle->x *= -1;
                }

                Balle->y = sqrt(vitesse-powx);
                if (Balle->y <= 0){
                    Balle->y *= -1;
                }
            }else{                  //Deuxieme moitie
                ecart = (testx + positionBalle-> w/2) - positionPlateau->x -(positionPlateau->w / 2);
                ecart *= -1.0;
                ecart *= 0.2/(positionPlateau->w / 2);
                ecart += 0.88;
                Balle->x *= ecart;
                powx = pow(Balle->x,2);
                if (powx >= vmax){
                    Balle->x = -sqrt(vmax);
                    powx = pow(Balle->x,2);
                }

                if(Balle->x < 0){
                        Balle->x *= -1;
                }

                Balle->y = sqrt(vitesse-powx);
                if (Balle->y <= 0){
                    Balle->y *=-1;
                }
            }
            if (Partie.bonus == 2){
                positionBalle->y = 728;
                Partie.lancer = 0;
                return;
            }
        }

        positionBalle->x -= (Balle->x);    //Changement de la position de la balle selon les infos mise a jours
        if ((Balle->y < 1) && (Balle->y >= 0)){
            positionBalle->y -= ((Balle->y)-1);
        }else if ((Balle->y > (-1)) && (Balle->y < 0)){
            positionBalle->y -= ((Balle->y)+1);
        }else{
            positionBalle->y -= (Balle->y);
        }

    }
}


int verifCollision(int x,int y){
    int i,j;
    i = x/16;
    j = y/16;       //On convertit la position en x et y en case dans le tableau
    if ((tab_Collision[i][j]==1) || (tab_Collision[i][j]==2) || (tab_Collision[i][j]==3) || (tab_Collision[i][j]==4) || (tab_Collision[i][j]==6) || (tab_Collision[i][j]==7) || (tab_Collision[i][j]==8) || (tab_Collision[i][j]==9)){
        return 1;
    }else{
        return 0;
    }

}

void casseBrique(int x ,int y,int x2,int y2,SDL_Surface * ecran){

    //SDL_Surface * fond;

    //fond = IMG_Load("Fond.png");

    int i,j,i2,j2;
    i = x/16;
    j = y/16;
    i2 = x2/16;
    j2 = y2/16;
    if ((i2 != i)|| (j2 != j)){     //Si on touche 2 brique on choisit au hasard celle qui se casse
        int tiragebrique;
        srand(time(NULL));
        tiragebrique=(rand()%2);
        if (tiragebrique == 0){
                if (  i > 0 && i < 63 && j > 0 && j < 47 )
                {

                    if ((i % 2) == 0){      //Les briques sont sur 2 case donc selon la case touchee on adapte le code
                        switch(TypeCollision(&i,&j))        //Selon le type de brique l'action est differente
                        {
                        case 1:
                            tab_Collision [i][j]=0;
                            i--;
                            VerifBonus(&i,&j);
                            tab_Collision [i][j]=0;
                            break;
                        case 2:
                            tab_Collision [i][j]-=1;
                            i--;
                            tab_Collision [i][j]-=1;
                            break;
                        case 3:
                            break;
                        }
                    }else{
                        switch(TypeCollision(&i,&j))
                        {
                        case 1:
                            VerifBonus(&i,&j);
                            tab_Collision [i][j]=0;
                            i++;
                            tab_Collision [i][j]=0;
                            break;
                        case 2:
                            tab_Collision [i][j]-=1;
                            i++;
                            tab_Collision [i][j]-=1;
                            break;
                        case 3:
                            break;
                        }
                    }
                }

        }else{      //Meme que celle du dessus mais si l'autre brique est a casser
            if ( i2 > 0 && i2 < 63 && j2 > 0 && j2 < 47 )
            {
                if ((i2 % 2) == 0){
                    switch(TypeCollision(&i2,&j2))
                        {
                        case 1:
                            tab_Collision [i2][j2]=0;
                            i2--;
                            VerifBonus(&i2,&j2);
                            tab_Collision [i2][j2]=0;
                            break;
                        case 2:
                            tab_Collision [i2][j2]-=1;
                            i2--;
                            tab_Collision [i2][j2]-=1;
                            break;
                        case 3:
                            break;
                        }
                }else{
                    switch(TypeCollision(&i2,&j2))
                    {
                    case 1:
                        VerifBonus(&i2,&j2);
                        tab_Collision [i2][j2]=0;
                        i2++;
                        tab_Collision [i2][j2]=0;
                        break;
                    case 2:
                        tab_Collision [i2][j2]-=1;
                        i2++;
                        tab_Collision [i2][j2]-=1;
                        break;
                    case 3:
                        break;
                    }
                }
            }

        }

    }else{
        if ( i > 0 && i < 63 && j > 0 && j < 47 )
        {
            if ((i % 2) == 0){
                switch(TypeCollision(&i,&j))
                    {
                    case 1:
                        tab_Collision [i][j]=0;
                        i--;
                        VerifBonus(&i,&j);
                        tab_Collision [i][j]=0;
                        break;
                    case 2:
                        tab_Collision [i][j]-=1;
                        i--;
                        tab_Collision [i][j]-=1;
                        break;
                    case 3:
                        break;
                    }
            }else{
                switch(TypeCollision(&i,&j))
                    {
                    case 1:
                        VerifBonus(&i,&j);
                        tab_Collision [i][j]=0;
                        i++;
                        tab_Collision [i][j]=0;
                        break;
                    case 2:
                        tab_Collision [i][j]-=1;
                        i++;
                        tab_Collision [i][j]-=1;
                        break;
                    case 3:
                        break;
                    }
            }
        }
    }

    //SDL_FreeSurface(fond);
}

void DeplaBonus(SDL_Rect * positionPlateau,Vitesse * Balle){


    if (Partie.frame >= 3){     //Partie frame permet de ralentir les animations des bonus pour donner le temps de voir la rotation
        int i;
        Partie.frame = 0;
        for (i=0;i<Partie.nbr_bonus;i++){  //On effectue les operations pour tous les bonus
            if(ListBonus[i].positionBonus.x != 0){
                if(ListBonus[i].positionBonus.y <= 752){        //Si le bonus est dans la zone de jeu
                    if(ListBonus[i].positionBonus.y >= 729 && ListBonus[i].positionBonus.x > positionPlateau->x-16 && ListBonus[i].positionBonus.x < positionPlateau->x+positionPlateau->w+16 ){
                            Partie.bonus = ListBonus[i].bonustype;
                            ListBonus[i].positionBonus.x = 0;
                            ListBonus[i].positionBonus.y = 0;
                }
            }
                if (ListBonus[i].positionSprite.x >= 192){      //on reinitialise l'animation des bonus
                    ListBonus[i].positionSprite.x = 0;
                }else{
                    ListBonus[i].positionSprite.x += 32;
                }
                ListBonus[i].positionBonus.y += 6;      //On fait descendre les bonus
            }else{
                ListBonus[i].positionBonus.x = 0;
                ListBonus[i].positionBonus.y = 0;
                }

            }

    }else{
        Partie.frame += 1;
    }
}

void VerifBonus (int * i,int * j){
    int testi = 0,testj = 0;

    if (tab_Collision[*i][*j] == 2){        //Si la brique est une brique bonus
        ListBonus[Partie.nbr_bonus].positionBonus.x = ((*i)*16);
        ListBonus[Partie.nbr_bonus].positionBonus.y = ((*j)*16) +16;
        ListBonus[Partie.nbr_bonus].positionSprite.h = 14;
        ListBonus[Partie.nbr_bonus].positionSprite.w = 32;


        ListBonus[Partie.nbr_bonus].bonustype =(rand()%8)+1;
        ListBonus[Partie.nbr_bonus].positionSprite.y = (ListBonus[Partie.nbr_bonus].bonustype);
        ListBonus[Partie.nbr_bonus].positionSprite.y =(ListBonus[Partie.nbr_bonus].bonustype - 1)*14;
        Partie.nbr_bonus += 1;
    }
    if(Partie.bonus == 4){
        testi = *i;
        testj = *j;
        testi -= 2;
        casseflame(&testi,&testj);
        testj +=1;
        testi +=1;
        casseflame(&testi,&testj);
        testi += 1;
        testj -= 1;
        casseflame(&testi,&testj);
        testj -= 1;
        testi -= 3;
        casseflame(&testi,&testj);

    }
}

int TypeCollision(int * i,int * j){
    if (tab_Collision[*i][*j] == 8 || tab_Collision[*i][*j] == 7){
        return 2;
    }else if (tab_Collision[*i][*j] <= 6 && tab_Collision[*i][*j] >= 1){
        return 1;
    }else{
        return 3;
    }

}

void casseflame(int * i,int * j){
    int buffer,
        flammei,
        flammej;

    flammei = *i;
    flammej = *j;

    buffer = TypeCollision(&flammei,&flammej);      //Ici on verifie les types de briques autour de celle cassee lorsque que la balle est explosive
    switch(buffer)
        {
        case 1:
            Partie.brique -= 1;
            tab_Collision [*i][*j]=0;
            *i += 1;
            tab_Collision [*i][*j]=0;
            break;
        case 2:
            tab_Collision [*i][*j]-=1;
            *i += 1;
            tab_Collision [*i][*j]-=1;
            break;
        case 3:
            *i += 1;
            break;
        default:
            *i += 1;
            break;
        }

}



