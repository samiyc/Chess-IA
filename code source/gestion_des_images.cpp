/*!==================================================================
Fichier : gestion_des_images.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void charger_les_images       ();
	void afficher_menu            ();
	void afficher_end             (char txt[]);
	void afficher_le_fond         ();
	void afficher_info            (int type, int x, int y);
	void afficher_piece           (int car_piece, int x, int y);
	void liberation_de_la_memoire ();

===================================================================*/


//LES VARIABLES GLOBALES ============================================
SDL_Surface* img_case_n;
SDL_Surface* img_case_b;

SDL_Surface* img_piece[12];

SDL_Surface* img_info_move;
SDL_Surface* img_info_eat;
SDL_Surface* img_info_def;
SDL_Surface* img_info_dep;

SDL_Surface* img_menu;


//LES FONCTIONS =====================================================


// Cette fonction permet de charger toutes les images des pièce de jeux
void charger_les_images()
{
    // Chargement de imagees
    img_case_n = SDL_LoadBMP("data/image/other/case_noir.bmp");
    img_case_b = SDL_LoadBMP("data/image/other/case_blanche.bmp");

	// les infos
    img_info_move = SDL_LoadBMP("data/image/other/move.bmp");
    img_info_eat  = SDL_LoadBMP("data/image/other/eat.bmp");
    img_info_def  = SDL_LoadBMP("data/image/other/def.bmp");
    img_info_dep  = SDL_LoadBMP("data/image/other/dep.bmp");

    SDL_SetColorKey(img_info_move, SDL_SRCCOLORKEY, SDL_MapRGB(img_info_move->format, 0, 255, 0));
    SDL_SetColorKey(img_info_eat , SDL_SRCCOLORKEY, SDL_MapRGB(img_info_eat->format , 0, 255, 0));
    SDL_SetColorKey(img_info_def , SDL_SRCCOLORKEY, SDL_MapRGB(img_info_def->format , 0, 255, 0));
    SDL_SetColorKey(img_info_dep , SDL_SRCCOLORKEY, SDL_MapRGB(img_info_dep->format , 0, 255, 0));

    //le menu
    img_menu  = SDL_LoadBMP("data/image/other/menu.bmp");
    SDL_SetColorKey(img_menu , SDL_SRCCOLORKEY, SDL_MapRGB(img_menu->format , 0, 255, 0));

	//les pieces
    img_piece[0] = SDL_LoadBMP("data/image/b_pion.bmp");
    img_piece[1] = SDL_LoadBMP("data/image/b_tour.bmp");
    img_piece[2] = SDL_LoadBMP("data/image/b_cavalier.bmp");
    img_piece[3] = SDL_LoadBMP("data/image/b_fou.bmp");
    img_piece[4] = SDL_LoadBMP("data/image/b_dame.bmp");
    img_piece[5] = SDL_LoadBMP("data/image/b_roi.bmp");

    img_piece[6] = SDL_LoadBMP("data/image/n_pion.bmp");
    img_piece[7] = SDL_LoadBMP("data/image/n_tour.bmp");
    img_piece[8] = SDL_LoadBMP("data/image/n_cavalier.bmp");
    img_piece[9] = SDL_LoadBMP("data/image/n_fou.bmp");
    img_piece[10] = SDL_LoadBMP("data/image/n_dame.bmp");
    img_piece[11] = SDL_LoadBMP("data/image/n_roi.bmp");

    //permet d'éfacer le fond vert des images
    for(int i=0; i<12; i++)
    {
        SDL_SetColorKey(img_piece[i], SDL_SRCCOLORKEY, SDL_MapRGB(img_piece[i]->format, 0, 255, 0));
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher le menu
void afficher_menu()
{
	char c_level[10];
	SDL_Color couleur_texte_noir = {0, 0, 0};
	SDL_Color couleur_texte = {level*15+50, (10-level)*20, 0};

    SDL_Rect position_menu;
	position_menu.x = 0;
	position_menu.y = 0;

    SDL_Surface* texte[4];
    texte[0] = TTF_RenderText_Blended(police, "Math Fail v1.2", couleur_texte_noir);
    texte[1] = TTF_RenderText_Blended(police, "Level :", couleur_texte_noir);

    sprintf(c_level, "%d", level);
    texte[2] = TTF_RenderText_Blended(police, c_level, couleur_texte);

    SDL_Rect position[3];
    position[0].x = (8*TAILLE_CASE/2) - (texte[0]->w/2);
    position[0].y = (8*TAILLE_CASE/3) - (texte[0]->h/2);
    position[1].x = (8*TAILLE_CASE/2) - (texte[1]->w) - 10;
    position[1].y = (8*TAILLE_CASE/2) - (texte[1]->h/2);
    position[2].x = (8*TAILLE_CASE/2) + 10;
    position[2].y = (8*TAILLE_CASE/2) - (texte[2]->h/2);

	//affichage
	afficher_le_fond();
	afficher_les_pieces();
	SDL_BlitSurface(img_menu, 0, screen, &position_menu);
	SDL_BlitSurface(texte[0], 0, screen, &position[0]);
	SDL_BlitSurface(texte[1], 0, screen, &position[1]);
	SDL_BlitSurface(texte[2], 0, screen, &position[2]);

	SDL_Flip(screen);

	//enter
	int continu = 1;
	do {

		SDL_Delay(20);
		scan_materiel();
		if (up_key())
		{
			level ++;
			if (level > 10) level = 10;

			sprintf(c_level, "%d", level);
			couleur_texte.r = level*15+50;
			couleur_texte.g = (10-level)*20;
			SDL_FreeSurface(texte[2]);
			texte[2] = TTF_RenderText_Blended(police, c_level, couleur_texte);

			//affichage
			afficher_le_fond();
			afficher_les_pieces();
			SDL_BlitSurface(img_menu, 0, screen, &position_menu);
			SDL_BlitSurface(texte[0], 0, screen, &position[0]);
			SDL_BlitSurface(texte[1], 0, screen, &position[1]);
			SDL_BlitSurface(texte[2], 0, screen, &position[2]);

			SDL_Flip(screen);
		}
		if (down_key())
		{
			level --;
			if (level < 1) level = 1;

			sprintf(c_level, "%d", level);
			couleur_texte.r = level*15+50;
			couleur_texte.g = (10-level)*20;
			SDL_FreeSurface(texte[2]);
			texte[2] = TTF_RenderText_Blended(police, c_level, couleur_texte);

			//affichage
			afficher_le_fond();
			afficher_les_pieces();
			SDL_BlitSurface(img_menu, 0, screen, &position_menu);
			SDL_BlitSurface(texte[0], 0, screen, &position[0]);
			SDL_BlitSurface(texte[1], 0, screen, &position[1]);
			SDL_BlitSurface(texte[2], 0, screen, &position[2]);

			SDL_Flip(screen);
		}
		if (quitter())
		{
			SDL_FreeSurface(texte[0]);
			SDL_FreeSurface(texte[1]);
			SDL_FreeSurface(texte[2]);
			liberation_de_la_memoire();
			printf("\n\nExit success..");
			exit(EXIT_SUCCESS);
		}
		if (enter_key()) continu = 0;

    } while (continu);

	//mise a jour des constantes du jeu
	MAX_RANG         = (level/3+1)*2;
	LARGEUR_ANNALYSE = level*3;
	if (level == 10) MAX_RANG = 10;

	//liberation de la mémoire
	SDL_FreeSurface(texte[0]);
    SDL_FreeSurface(texte[1]);
    SDL_FreeSurface(texte[2]);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher le message de fin de parti
void afficher_end(char txt[])
{
	police = TTF_OpenFont("data/font/heroin07.ttf", 30);

	SDL_Color couleur_texte = {0, (10-(level/2))*20, level*15+50};

    SDL_Rect position_menu;
	position_menu.x = 0;
	position_menu.y = 0;

    SDL_Surface* texte;
    texte = TTF_RenderText_Blended(police, txt, couleur_texte);



    SDL_Rect position;
    position.x = (8*TAILLE_CASE/2) - (texte->w/2);
    position.y = (8*TAILLE_CASE/2) - (texte->h/2);

	//affichage
	afficher_le_fond();
	afficher_les_pieces();
	SDL_BlitSurface(img_menu, 0, screen, &position_menu);
	SDL_BlitSurface(texte, 0, screen, &position);

	SDL_Flip(screen);

	//enter
	int continu = 1;
	do {

		SDL_Delay(20);
		scan_materiel();

		if (quitter())
		{
			SDL_FreeSurface(texte);
			liberation_de_la_memoire();
			printf("\n\nExit success..");
			exit(EXIT_SUCCESS);
		}
		//if (enter_key()) continu = 0;

    } while (continu);

	//liberation de la mémoire
	SDL_FreeSurface(texte);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher le fond de l'écran (le damier)
void afficher_le_fond()
{
    int i, j;
    SDL_Rect position;

    for(i = 0; i<8; i++)
    {
        for(j = 0; j<8; j++)
        {
            position.x = i*TAILLE_CASE;
            position.y = j*TAILLE_CASE;

            if (((i+j)%2) == 0)
            {
                SDL_BlitSurface(img_case_b, 0, screen, &position);
            }
            else
            {
                SDL_BlitSurface(img_case_n, 0, screen, &position);
            }
        }
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de grisser un case, afin d'informer
// le joueur d'un déplacement possible
void afficher_info(int type, int x, int y)
{
    SDL_Rect position;

	position.x = x*TAILLE_CASE;
	position.y = y*TAILLE_CASE;

	if (type == D_MOVE) SDL_BlitSurface(img_info_move, 0, screen, &position);
	if (type == D_EAT)  SDL_BlitSurface(img_info_def,  0, screen, &position);
	if (type == D_DEF)  SDL_BlitSurface(img_info_def,  0, screen, &position);
	if (type == D_NULL) SDL_BlitSurface(img_info_dep,  0, screen, &position);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher une piece a l'emplacement de la sourie
void afficher_piece(int car_piece, int x, int y)
{
    SDL_Rect position;

    for(int k=0; k<12; k++)
	{
		if (car_piece == conf_car_piece[k])
		{
			position.x = x;
			position.y = y;
			SDL_BlitSurface(img_piece[k], 0, screen, &position);
			k=12;
		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de vider la mémoire ram des images chargées
// précédament
void liberation_de_la_memoire()
{
    SDL_FreeSurface(screen);
    SDL_FreeSurface(img_info_move);
    SDL_FreeSurface(img_info_eat);
    SDL_FreeSurface(img_info_def);
    SDL_FreeSurface(img_info_dep);
    SDL_FreeSurface(img_menu);

    SDL_FreeSurface(img_case_n);
    SDL_FreeSurface(img_case_b);

    for(int i = 0; i<12; i++)
    {
        SDL_FreeSurface(img_piece[i]);
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




