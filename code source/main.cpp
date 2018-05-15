/*!==================================================================
Fichier : main.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
===================================================================*/
#include "constantes.h"
#include "header.h"


//LES FONCTION ANNEXES ==============================================
#include "config.cpp"
#include "gestion_sdl.cpp"
#include "gestion_des_images.cpp"
#include "gestion_carte.cpp"
#include "gestion_clavier.cpp"
#include "gestion_regles.cpp"
#include "gestion_bot.cpp"


///FONCTION PRINCIPALE ==============================================
int main ( int argc, char** argv )
{
    //les variables
    int etat_souri = 0;
    int id_piece_saisi = 0;
	int tp_x_dif;
	int tp_y_dif;


    //LES INITIALISATIONS ===========================================
    lire_fichier_de_config();
    initialisation_fenetre();
    charger_les_images();
    charger_la_map();

    afficher_menu();

	printf("\n\n########  FICHIER DE LOG - MATHFAIL v1.2 - LEVEL:%d  ########\n\n\n", level);
    run_annalyse(carte);


    ///DEBUT DE LA BOUCLE PRINCIPALE ================================
    bool done = false;
    while (!done)
    {
        /// GESTION CLAVIER
        scan_materiel();

        if (quitter()) done = true;

        if (left_click())
        {
			gestion_du_deplacement(etat_souri, id_piece_saisi, tp_x_dif, tp_y_dif);
        }

        if (right_click() && (etat_souri == 1))
        {
        	etat_souri = 0;
        	put_piece(id_piece_saisi, old_pos_click_x, old_pos_click_y);
        }

        if (clear_key())
        {
        	memcpy(carte, tmp_map, 8*8);
        }


        /// DRAWING STARTS HERE

			// clear screen
			SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

			// le fond
			afficher_le_fond();

			// dernier déplacement de l'ordinateur
			if (activation_bot) afficher_move();

			//afficher les cases dispo pour le déplacement
			if (etat_souri == 1) afficher_case_dispo();

			// les pièces
			afficher_les_pieces();//if (clear_key())

			// piece selectionnée
			if (etat_souri == 1) afficher_piece(id_piece_saisi, event.button.x-tp_x_dif, event.button.y-tp_y_dif);

        /// DRAWING ENDS HERE

        SDL_Flip(screen);
        SDL_Delay(20);

    }///FIN DE LA BOUCLE PRINCIPALE =================================


    liberation_de_la_memoire();
    printf("\n\nExit success..");
    return (EXIT_SUCCESS);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




