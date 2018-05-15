/*!==================================================================
Fichier : gestion_regles.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	int  deplacement_possible     (char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_possibilite       (char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_deplacement       (char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  tester_diagonale         (char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y);
	int  en_echec                 (char p_map[8][8], int id_piece, int pos_x, int pos_y);

	void save_move_dispo          (int id_unit, int x, int y);
	void afficher_case_dispo      ();

	void gestion_du_deplacement   (int &etat_souri, int &id_piece_saisi, int &tp_x_dif, int &tp_y_dif);
	void end_game                 ();

===================================================================*/

// Cette fonction retourne la chaine qui correspond au type de déplacement
// Ce qui permet par exemple :
// printf("type de deplacement : ", TYPE_DEPLACEMENT(variable));
// !note: la sorti standard du programme se trouve dans le fichier stdou.txt
#define TYPE_DEPLACEMENT(c) ((c == D_NULL) ? "D_NULL" : ((c == D_MOVE) ? "D_MOVE" : ((c == D_EAT) ? "D_EAT" : ((c == D_ROQ) ? "D_ROQ" : "D_DEF"))))


//LES VARIABLES GLOBALES ============================================
struct_point temp_move[100];
int          nb_move;
struct_point temp_eat [100];
int          nb_eat;
struct_point temp_def [100];
int          nb_def;



//LES FONCTIONS =====================================================


// Cette fonction permet de tester si le déplacement
// d'une pièce est possible sur une case cible
// En prenant en compte la possibilité que le roi soit en echec
// retour possible : D_NULL, D_MOVE, D_EAT, D_DEF, D_ROQ
int deplacement_possible(char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y)
{
	int retour;
	int var;

	retour = tester_possibilite(map, id_piece, pos_x, pos_y, cible_x, cible_y);
	if  (retour == D_NULL) return (D_NULL);
	if ((retour == D_MOVE) || (retour == D_EAT)) goto etape2;

///ETAPE 1 ------------------------------------------------------

	//NOTE : retour = D_DEF ou bien D_ROQ
	//       pas de déplacement

	var = en_echec(map, id_piece, pos_x, pos_y);

	if (var != KING_OK) return(D_NULL);
	return(retour);

///ETAPE 2 ------------------------------------------------------

	//NOTE : retour = D_MOVE ou bien D_EAT
	//       il y a un déplacement
	etape2:

	//nouvelle carte pour les tests
	char temp_map[8][8];
	memcpy(temp_map, map, 8*8);

	//deplacement de la piece
	temp_map[cible_y][cible_x] = id_piece;
	temp_map[pos_y  ][pos_x  ] = conf_car_fond;

///ETAPE 3 ------------------------------------------------------

	var = en_echec(temp_map, id_piece, cible_x, cible_y);

	if (var != KING_OK) return(D_NULL);
	return(retour);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Cette fonction permet de tester si le déplacement
// d'une pièce est possible sur une case cible
// !note: ne prend pas en compte la possibilité que le roi soit en echec
// retour possible : D_NULL, D_MOVE, D_EAT, D_DEF, D_ROQ
int tester_possibilite(char map[8][8], int id_piece, int pos_x, int pos_y, int cible_x, int cible_y)
{
	int type_piece    = get_type_of_unit(id_piece);
	int couleur_piece = get_color_of_unit(id_piece);
	int id_cible      = map[cible_y][cible_x];
	int couleur_cible = get_color_of_unit(id_cible);


	int x_dif, y_dif;
	x_dif = cible_x - pos_x;
	y_dif = cible_y - pos_y;


	if (couleur_piece == C_BLANC) y_dif = -y_dif;
	if ((pos_x == cible_x) && (pos_y == cible_y)) return (D_NULL);


///******************************************************************

	//SWITCH MAISON (pas bien !!)
	if (type_piece == P_PION)/// PION -------------------------------
	{
		int retour;
		if (couleur_piece == couleur_cible)
		{
			retour = D_DEF;
		}
		else
		{
			retour = D_EAT;
		}

		if (couleur_piece == C_BLANC)
		{
			if ((x_dif == 0) && (y_dif == 2) && (pos_y == 6) && (id_cible == conf_car_fond) && (map[5][pos_x] == conf_car_fond))
			{
				return (D_MOVE);
			}
			if ((x_dif == -1) && (y_dif == 1) && (id_cible != conf_car_fond))
			{
				return (retour);
			}
			if ((x_dif == 1) && (y_dif == 1) && (id_cible != conf_car_fond))
			{
				return (retour);
			}
		}
		else
		{
			if ((x_dif == 0) && (y_dif == 2) && (pos_y == 1) && (id_cible == conf_car_fond) && (map[2][pos_x] == conf_car_fond))
			{
				return (D_MOVE);
			}
			if ((x_dif == -1) && (y_dif == 1) && (id_cible != conf_car_fond))
			{
				return (retour);
			}
			if ((x_dif == 1) && (y_dif == 1) && (id_cible != conf_car_fond))
			{
				return (retour);
			}
		}
		if ((x_dif == 0) && (y_dif == 1) && (id_cible == conf_car_fond))
		{
			return (D_MOVE);
		}
	}
	else if (type_piece == P_TOUR)/// TOUR --------------------------
	{
		if(pos_x == cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_deplacement(map, O_DOWN, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_deplacement(map, O_UP, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}
		if(pos_y == cible_y)
		{
			if (pos_x < cible_x)
			{
				return (tester_deplacement(map, O_RIGHT, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_deplacement(map, O_LEFT, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}

		return (D_NULL);
	}
	else if (type_piece == P_CAVALIER)/// CAVALIER ------------------
	{
		int retour;
		if (id_cible == conf_car_fond)
		{
			retour = D_MOVE;
		}
		else
		{
			if (couleur_piece == couleur_cible)
			{
				retour = D_DEF;
			}
			else
			{
				retour = D_EAT;
			}
		}

		if (((pos_x-2) == cible_x) && ((pos_y-1) == cible_y)) return (retour);
		if (((pos_x-2) == cible_x) && ((pos_y+1) == cible_y)) return (retour);

		if (((pos_x+2) == cible_x) && ((pos_y-1) == cible_y)) return (retour);
		if (((pos_x+2) == cible_x) && ((pos_y+1) == cible_y)) return (retour);

		if (((pos_x-1) == cible_x) && ((pos_y-2) == cible_y)) return (retour);
		if (((pos_x-1) == cible_x) && ((pos_y+2) == cible_y)) return (retour);

		if (((pos_x+1) == cible_x) && ((pos_y-2) == cible_y)) return (retour);
		if (((pos_x+1) == cible_x) && ((pos_y+2) == cible_y)) return (retour);

		return (D_NULL);
	}
	else if (type_piece == P_FOU)/// FOU ----------------------------
	{
		if(pos_x < cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_diagonale(map, O_SE, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_diagonale(map, O_NE, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}
		if(pos_x > cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_diagonale(map, O_SW, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_diagonale(map, O_NW, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}

		return (D_NULL);
	}
	else if (type_piece == P_DAME)/// DAME --------------------------
	{
		if(pos_x == cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_deplacement(map, O_DOWN, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_deplacement(map, O_UP, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}
		if(pos_y == cible_y)
		{
			if (pos_x < cible_x)
			{
				return (tester_deplacement(map, O_RIGHT, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_deplacement(map, O_LEFT, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}

		if(pos_x < cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_diagonale(map, O_SE, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_diagonale(map, O_NE, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}
		if(pos_x > cible_x)
		{
			if (pos_y < cible_y)
			{
				return (tester_diagonale(map, O_SW, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
			else
			{
				return (tester_diagonale(map, O_NW, id_piece, pos_x, pos_y, cible_x, cible_y));
			}
		}

		return (D_NULL);
	}
	else if (type_piece == P_ROI)/// ROI ----------------------------
	{
		int retour;
		if (id_cible == conf_car_fond)
		{
			retour = D_MOVE;
		}
		else
		{
			if (couleur_piece == couleur_cible)
			{
				retour = D_DEF;
			}
			else
			{
				retour = D_EAT;
			}
		}

		if (((pos_x-1) == cible_x) && (pos_y == cible_y)) return (retour);
		if (((pos_x+1) == cible_x) && (pos_y == cible_y)) return (retour);

		if ((pos_x == cible_x) && ((pos_y-1) == cible_y)) return (retour);
		if ((pos_x == cible_x) && ((pos_y+1) == cible_y)) return (retour);

		if (((pos_x+1) == cible_x) && ((pos_y-1) == cible_y)) return (retour);
		if (((pos_x+1) == cible_x) && ((pos_y+1) == cible_y)) return (retour);

		if (((pos_x-1) == cible_x) && ((pos_y-1) == cible_y)) return (retour);
		if (((pos_x-1) == cible_x) && ((pos_y+1) == cible_y)) return (retour);

		//ROQ..
		if (couleur_piece == C_BLANC)
		{
			if ((pos_x == 4) && (pos_y == 7) && (cible_x == 6) && (cible_y == 7) && (map[pos_y][pos_x+1] == conf_car_fond) && (map[pos_y][pos_x+2] == conf_car_fond) && (get_type_of_unit(map[pos_y][pos_x+3]) == P_TOUR))
			{
				return (D_ROQ);
			}
			if ((pos_x == 4) && (pos_y == 7) && (cible_x == 2) && (cible_y == 7) && (map[pos_y][pos_x-1] == conf_car_fond) && (map[pos_y][pos_x-2] == conf_car_fond) && (map[pos_y][pos_x-3] == conf_car_fond) && (get_type_of_unit(map[pos_y][pos_x-4]) == P_TOUR))
			{
				return (D_ROQ);
			}
		}
		else
		{
			if ((pos_x == 4) && (pos_y == 0) && (cible_x == 6) && (cible_y == 0) && (map[pos_y][pos_x+1] == conf_car_fond) && (map[pos_y][pos_x+2] == conf_car_fond) && (get_type_of_unit(map[pos_y][pos_x+3]) == P_TOUR))
			{
				return (D_ROQ);
			}
			if ((pos_x == 4) && (pos_y == 0) && (cible_x == 2) && (cible_y == 0) && (map[pos_y][pos_x-1] == conf_car_fond) && (map[pos_y][pos_x-2] == conf_car_fond) && (map[pos_y][pos_x-3] == conf_car_fond) && (get_type_of_unit(map[pos_y][pos_x-4]) == P_TOUR))
			{
				return (D_ROQ);
			}
		}

		return (D_NULL);
	}
	return (D_NULL);

///******************************************************************


}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si l'on peut déplacer une Tour/Dame
// dans la direction voulu
// les directions attenduent sont : O_UP, O_DOWN, O_LEFT, O_RIGHT
// retour possible : D_NULL, D_MOVE, D_EAT, D_DEF
int tester_deplacement(char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y)
{
	int id_cible = map[cible_y][cible_x];

	int couleur_piece = get_color_of_unit(id_piece);
	int couleur_cible = get_color_of_unit(id_cible);

	int retour;
	if (id_cible == conf_car_fond)
	{
		retour = D_MOVE;
	}
	else
	{
		if (couleur_piece == couleur_cible)
		{
			retour = D_DEF;
		}
		else
		{
			retour = D_EAT;
		}
	}

	for (int i=1; i<8; i++)
	{

		if (orientation == O_DOWN)
		{
			if ((pos_y+i) > 7)                        return (D_NULL);
			if ((pos_y+i) == cible_y)                 return (retour);
			if (map[pos_y+i][pos_x] != conf_car_fond) return (D_NULL);
		}
		else if (orientation == O_UP)
		{
			if ((pos_y-i) < 0)                        return (D_NULL);
			if ((pos_y-i) == cible_y)                 return (retour);
			if (map[pos_y-i][pos_x] != conf_car_fond) return (D_NULL);
		}
		else if (orientation == O_RIGHT)
		{
			if ((pos_x+i) > 7)                        return (D_NULL);
			if ((pos_x+i) == cible_x)                 return (retour);
			if (map[pos_y][pos_x+i] != conf_car_fond) return (D_NULL);
		}
		else if (orientation == O_LEFT)
		{
			if ((pos_x-i) < 0)                        return (C_NULL);
			if ((pos_x-i) == cible_x)                 return (retour);
			if (map[pos_y][pos_x-i] != conf_car_fond) return (D_NULL);
		}

	}//Fin boucle for

	return (D_NULL);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si l'on peut déplacer un Fou/une Dame
// dans la direction voulu
// les directions attenduent sont : O_NE, O_SE, O_SW, O_NW
// retour possible : D_NULL, D_MOVE, D_EAT, D_DEF
int tester_diagonale(char map[8][8], int orientation, int id_piece, int pos_x, int pos_y, int cible_x, int cible_y)
{
	int id_cible = map[cible_y][cible_x];

	int couleur_piece = get_color_of_unit(id_piece);
	int couleur_cible = get_color_of_unit(id_cible);

	int retour;
	if (id_cible == conf_car_fond)
	{
		retour = D_MOVE;
	}
	else
	{
		if (couleur_piece == couleur_cible)
		{
			retour = D_DEF;
		}
		else
		{
			retour = D_EAT;
		}
	}

	for (int i=1; i<8; i++)
	{

		if (orientation == O_NE)
		{
			if ( ((pos_x+i) > 7) && ((pos_y-i) < 0) )               return (C_NULL);
			if ( ((pos_x+i) == cible_x) && ((pos_y-i) == cible_y) ) return (retour);
			if (map[pos_y-i][pos_x+i] != conf_car_fond)             return (D_NULL);
		}
		else if (orientation == O_SE)
		{
			if ( ((pos_x+i) > 7) && ((pos_y+i) > 7) )               return (C_NULL);
			if ( ((pos_x+i) == cible_x) && ((pos_y+i) == cible_y) ) return (retour);
			if (map[pos_y+i][pos_x+i] != conf_car_fond)             return (D_NULL);
		}
		else if (orientation == O_SW)
		{
			if ( ((pos_x-i) < 0) && ((pos_y+i) > 7) )               return (C_NULL);
			if ( ((pos_x-i) == cible_x) && ((pos_y+i) == cible_y) ) return (retour);
			if (map[pos_y+i][pos_x-i] != conf_car_fond)             return (D_NULL);
		}
		else if (orientation == O_NW)
		{
			if ( ((pos_x-i) < 0) && ((pos_y-i) < 0) )               return (C_NULL);
			if ( ((pos_x-i) == cible_x) && ((pos_y-i) == cible_y) ) return (retour);
			if (map[pos_y-i][pos_x-i] != conf_car_fond)             return (D_NULL);
		}

	}//Fin boucle for

	return (D_NULL);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// cette fonction permet de savoir si le roi est en echec
int en_echec(char map[8][8], int id_piece, int pos_x, int pos_y)
{
	int id;
	int couleur;
	int type_piece    = get_type_of_unit(id_piece);
	int couleur_piece = get_color_of_unit(id_piece);

	int id_king;
	int pos_king_x = -1;
	int pos_king_y = -1;

	//find king
	if (type_piece != P_ROI)
	{
		bool continu = true;
		for (int y=0; (y<8) && continu; y++)
		for (int x=0; (x<8) && continu; x++)
		{
			id_king = map[y][x];
			couleur = get_color_of_unit(id_king);
			if ((get_type_of_unit(id_king) == P_ROI) && (couleur == couleur_piece))
			{
				pos_king_x = x;
				pos_king_y = y;
				continu = false;
			}
		}
		if (continu == true) return (KING_IS_DIED);//NO KING FOUND
	}
	else
	{
		id_king = id_piece;
		pos_king_x = pos_x;
		pos_king_y = pos_y;
	}

	//king mangeable ?
	for (int y=0; y<8; y++)//ORIGIN
	for (int x=0; x<8; x++)
	{
		id = map[y][x];
		couleur = get_color_of_unit(id);

		if (id != conf_car_fond)
		if (couleur != couleur_piece)
		if (tester_possibilite(map, id, x, y, pos_king_x, pos_king_y) == D_EAT)
		{
			return(KING_FAIL);
		}
	}

	return(KING_OK);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************
// 29/06/2010


void save_move_dispo(int id_unit, int pos_x, int pos_y)
{
	nb_eat = 0;
	nb_move = 0;
	nb_def = 0;

	for (int y=0; y<8; y++)
	for (int x=0; x<8; x++)
	{
		int retour = deplacement_possible(carte, id_unit, pos_x, pos_y, x, y);
		if (retour == D_EAT)
		{
			temp_eat[nb_eat].x = x;
			temp_eat[nb_eat].y = y;
			nb_eat++;
		}
		else if ((retour == D_MOVE) || (retour == D_ROQ))
		{
			temp_move[nb_move].x = x;
			temp_move[nb_move].y = y;
			nb_move++;
		}
		else if (retour == D_DEF)
		{
			temp_def[nb_def].x = x;
			temp_def[nb_def].y = y;
			nb_def++;
		}
		else
		{
			//mouvement impossible
		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void afficher_case_dispo()
{
	for (int i=0; i<nb_move; i++)
	{
		afficher_info(D_MOVE, temp_move[i].x, temp_move[i].y);
	}
	for (int i=0; i<nb_eat; i++)
	{
		afficher_info(D_EAT, temp_eat[i].x, temp_eat[i].y);
	}
	for (int i=0; i<nb_def; i++)
	{
		afficher_info(D_DEF, temp_def[i].x, temp_def[i].y);
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************
// 24/07/2010


// C'est ici qu'est réaliser le traitement qui permet a l'utilisateur
// de déplacer les pieces de l'échequier
void gestion_du_deplacement(int &etat_souri, int &id_piece_saisi, int &tp_x_dif, int &tp_y_dif)
{
	int retour;

	if (etat_souri == 0)
	{
		//save
		//gestion_carte_temp(S_SAVE);
		memcpy(tmp_map, carte, 8*8);

		//flag deplacement en cours
		etat_souri = 1;

		//on prend la piece
		id_piece_saisi = get_piece(pos_click_x, pos_click_y);

		//on verifi que l'emplacement n'est pas vide
		if (id_piece_saisi != conf_car_fond)
		{
			//on vérifi que l'utilisateur déplace bien une pièce blanche
			if ((get_color_of_unit(id_piece_saisi) == C_BLANC) || (!activation_bot))
			{
				//enregistrements des cases dispo pour le déplacement
				save_move_dispo(id_piece_saisi, pos_click_x, pos_click_y);

				//enregistrement de décalage par rapport a l'angle sup-gauche de la case cliqué
				tp_x_dif = event.button.x - pos_click_x * TAILLE_CASE;
				tp_y_dif = event.button.y - pos_click_y * TAILLE_CASE;
			}
			else
			{
				//on repose la piece
				etat_souri = 0;
				put_piece(id_piece_saisi, pos_click_x, pos_click_y);
			}
		}
		else
		{
			//on stop le move
			etat_souri = 0;
		}
	}
	else//ON POSE LA PIECE
	{
		etat_souri = 0;
		retour = deplacement_possible(carte, id_piece_saisi, old_pos_click_x, old_pos_click_y, pos_click_x, pos_click_y);

		if ((retour == D_MOVE) || (retour == D_EAT))
		{
			//on pose la pièce
			put_piece(id_piece_saisi, pos_click_x, pos_click_y);

			//le bot jou
			run_annalyse(carte);
			end_game();
			if (activation_bot)
			{
				afficher_le_fond();afficher_les_pieces();SDL_Flip(screen);
				play();
				run_annalyse(carte);
				end_game();
			}
		}
		else if (retour == D_ROQ)
		{
			//on pose la pièce
			put_piece(id_piece_saisi, pos_click_x, pos_click_y);

			//on déplace la tour
			if(get_color_of_unit(id_piece_saisi) == C_BLANC)
			{
				if (pos_click_x == 6)
				{
					int var = get_piece(7, 7);
					put_piece(var, 5, 7);
				}
				else
				{
					int var = get_piece(0, 7);
					put_piece(var, 3, 7);
				}
			}
			else
			{
				if (pos_click_x == 6)
				{
					int var = get_piece(7, 0);
					put_piece(var, 5, 0);
				}
				else
				{
					int var = get_piece(0, 0);
					put_piece(var, 3, 0);
				}
			}

			//le bot jou
			run_annalyse(carte);
			end_game();
			if (activation_bot)
			{
				afficher_le_fond();afficher_les_pieces();SDL_Flip(screen);
				play();
				run_annalyse(carte);
				end_game();
			}
		}
		else
		{
			put_piece(id_piece_saisi, old_pos_click_x, old_pos_click_y);
		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Cette fonction teste si un des deux rois est en echec-et-mat
// permet d'afficher le message de fin du jeu WIN/LOOSE
void end_game()
{
	int id_piece;
	int couleur_piece;
	int id_cible;
	int couleur_cible;

	bool b_flag;
	bool n_flag;

	//traitement
	b_flag = false;
	n_flag = false;
	for(int x=0; x<8; x++)//ORIGIN
	for(int y=0; y<8; y++)
	{
		id_piece = carte[y][x];
		couleur_piece = get_color_of_unit(id_piece);

		for(int x2=0; x2<8; x2++)//DESTINATION
		for(int y2=0; y2<8; y2++)
		{
			id_cible = carte[y2][x2];
			couleur_cible = get_color_of_unit(id_cible);

			if (couleur_piece != couleur_cible)
			{
				int var = deplacement_possible(carte, id_piece, x, y, x2, y2);

				if (var != D_NULL)
				{
					if (couleur_piece == C_BLANC) b_flag = true;
					if (couleur_piece == C_NOIR ) n_flag = true;
				}
			}
		}
	}

	//end
	char chaine[100];
	sprintf(chaine, "Level:%d, Vous avez perdu la partie...", level);
	if (!b_flag) afficher_end(chaine);
	sprintf(chaine, "Level %d, Vous avez gagner, Bravo !!", level);
	if (!n_flag) afficher_end(chaine);

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%





