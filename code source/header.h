/*!==================================================================
Fichier : header.h
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
===================================================================*/
#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

//PROTOTYPE DES FONCTIONS ============================================


	/* config */
	void lire_fichier_de_config   ();


	/* gestion SDL */
	void initialisation_fenetre   ();


	/* gestion des images */
	void charger_les_images       ();
	void afficher_menu            ();
	void afficher_end             (char txt[]);
	void afficher_le_fond         ();
	void afficher_info            (int type, int x, int y);
	void afficher_piece           (int car_piece, int x, int y);
	void liberation_de_la_memoire ();


	/* gestion de la carte */
	void charger_la_map           ();
	void afficher_les_pieces      ();
	//******
	int  get_piece                (int x, int y);
	void put_piece                (int car_piece, int x, int y);
	//******
	int  get_color_of_unit        (int car_piece);
	int  get_type_of_unit         (int car_piece);


	/* gestion des regles */
	int  deplacement_possible     (char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_possibilite       (char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_deplacement       (char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_diagonale         (char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  en_echec                 (char p_map[8][8], int id_piece, int pos_x, int pos_y);
	//******
	void save_move_dispo          (int id_unit, int x, int y);
	void afficher_case_dispo      ();
	//******
	void gestion_du_deplacement   (int &etat_souri, int &id_piece_saisi, int &tp_x_dif, int &tp_y_dif);
	void end_game                 ();


	/* gestion des periphériques */
	void scan_materiel            ();
	bool left_click               ();
	bool right_click              ();
	bool up_key                   ();
	bool down_key                 ();
	bool clear_key                ();
	bool enter_key                ();
	bool quitter                  ();


	/* gestion du BOT */
	void play                     ();
	void bot_make_a_choice        (int rang);
	int  next_round               (int id);
	int  add_element              (int id, int id_coup_ref, int n_rang, int couleur, int n_couleur, int x, int y, int x2, int y2, int var);
	void ecraimage                (int id_pere, int rang, int couleur);
	void sup_element              (int id);
	//******
	int  annalyser_situation      (char map[8][8], int choix_couleur);
	int  valeur_piece             (int id_unit, char map[8][8], int pos_x, int pos_y);
	int  valeur_move              (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);
	int  valeur_def               (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);
	int  valeur_eat               (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);
	//******
	void run_annalyse             (char map[8][8]);
	void save_move                (int id);
	void afficher_move            ();


//*******************************************************************
#endif // HEADER_H_INCLUDED
