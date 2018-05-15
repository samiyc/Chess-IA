/*!==================================================================
Fichier : gestion_clavier.h
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void scan_materiel            ();
	bool left_click               ();
	bool right_click              ();
	bool clear_key                ();
	bool quitter                  ();

===================================================================*/


//LES VARIABLES GLOBALES ============================================
SDL_Event event;

int e_left_click = 0;
int e_right_click = 0;

int pos_click_x;
int pos_click_y;

int old_pos_click_x;
int old_pos_click_y;


//LES FONCTIONS =====================================================


// Cette fonction permet de scanner les périphériques afin de savoir
// si l'utilisateur à appuyer sur un bouton
void scan_materiel()
{
	SDL_PollEvent(&event); //SDL_PollEvent  SDL_WaitEvent

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			e_left_click = 1;
		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			e_right_click = 1;
		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//-------------------------------------------------------------------


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur le bouton gauche de la souri
bool left_click()
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			if (e_left_click == 1)
			{
				e_left_click = 0;

				old_pos_click_x = pos_click_x;
				old_pos_click_y = pos_click_y;
				pos_click_x = (event.button.x / TAILLE_CASE);
				pos_click_y = (event.button.y / TAILLE_CASE);

				return (true);
			}
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur le bouton droite de la souri
bool right_click()
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			if (e_right_click == 1)
			{
				e_right_click = 0;

				old_pos_click_x = pos_click_x;
				old_pos_click_y = pos_click_y;
				pos_click_x = (event.button.x / TAILLE_CASE);
				pos_click_y = (event.button.y / TAILLE_CASE);

				return (true);
			}
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//-------------------------------------------------------------------


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur fleche du haut
bool up_key()
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_UP)
		{
			return (true);
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur fleche du bas
bool down_key()
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_DOWN)
		{
			return (true);
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//-------------------------------------------------------------------


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur retour
bool clear_key()
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_BACKSPACE)
		{
			return (true);
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir
// si l'utilisateur à appuyer sur entrer
bool enter_key()
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			return (true);
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si l'utilisateur à appuyer
// sur la croix ou bien sur la touche echap
bool quitter()
{
	if (event.type == SDL_QUIT)
	{
		return (true);
	}
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			return (true);
		}
	}
	return (false);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






