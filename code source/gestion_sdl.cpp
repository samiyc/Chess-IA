/*!==================================================================
Fichier : gestion_sdl.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void initialisation_fenetre   ();

===================================================================*/


//LES VARIABLES GLOBALES ============================================
SDL_Surface *screen;
TTF_Font *police;


//LES FONCTIONS =====================================================

// Permet d'initialisation de la fenetre avec la librairie SDL
void initialisation_fenetre()
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError() );
        system("pause");
        exit(-1);
    }

    // initialisation police de caractère
    TTF_Init();
    police = TTF_OpenFont("data/font/heroin07.ttf", 50);

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

	//set the title
	SDL_WM_SetCaption("MathFail v1.2  (11/09/2010) | By samuel yanez-carbonell", NULL);

    // create a new window
    screen = SDL_SetVideoMode(8*TAILLE_CASE, 8*TAILLE_CASE, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if ( !screen )
    {
        fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
        system("pause");
        exit(-1);
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




