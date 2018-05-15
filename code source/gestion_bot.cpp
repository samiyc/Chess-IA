/*!==================================================================
Fichier : gestion_bot.h
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void play                     ();
	void bot_make_a_choice        (int rang);
	int  next_round               (int id);
	int  add_element              (int id, int id_coup_ref, int n_rang, int couleur, int n_couleur, int x, int y, int x2, int y2, int var);
	void ecraimage                (int id_pere, int rang, int couleur);
	void sup_element              (int id);

	int  annalyser_situation      (char map[8][8], int choix_couleur);
	int  valeur_piece             (int id_unit, char map[8][8], int pos_x, int pos_y);
	int  valeur_move              (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);
	int  valeur_def               (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);
	int  valeur_eat               (int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y);

	void run_annalyse             (char map[8][8]);
	void save_move                (int id);
	void afficher_move            ();

===================================================================*/
//ecriture du module à partie du 24/07/2010


//MACRO-FONCTIONS DE TEST ===========================================
#define COLOR(c) ((c == C_NOIR) ? "noir" : "blanc")
#define PAIRE(c) (((c%2) == 0) ? 1 : 0 )


//LES VARIABLES GLOBALES ============================================
int tab_coup_ref[200];//LARGEUR_ANNALYSE
int nb_coup_ref;

struct_stat tab_stat[200];//LARGEUR_ANNALYSE

struct_point origin;
struct_point dest;
int save_dep = 0;


//LES FONCTIONS =====================================================


// Premier fonction appelé, lorsque le bot doit jouer
// Le bot fait une copy de la "carte" d'origine dans plusieurs "map"
// Une fois l'analyse réaliser il écrase l'ancienne "carte"
void play()
{
	int rang;
	int continuer;
	int retour;

	printf("\n\n#### PLAY ####\n");

	//initialisation
	memcpy(brain[0].map, carte, 8*8);
	brain[0].id_pere = 0;
	brain[0].id_coup_ref = 0;
	brain[0].rang        = 0;
	brain[0].couleur     = C_NOIR;

	nb_coup = 1;

	nb_coup_ref = 0;
	for(int c=0; c<LARGEUR_ANNALYSE; c++)
	{
		tab_coup_ref[nb_coup_ref] = (c + 1);
		nb_coup_ref++;
	}

	//Création du round 0
	next_round(0);

	//Création des Round 1 est supérieurs
	rang      = 1;
	continuer = 1;
	printf("rang 1 - nb coup %d/%d\n", nb_coup, NB_MAX_COUP);

	while(continuer)
	{
		for(int i=0; ((i<nb_coup) && continuer); i++)
		{
			if (brain[i].rang == rang)
			{
				SDL_Delay(20);
				retour = next_round(i);
				if (retour == BOT_ERREUR) continuer = 0;
			}
		}
		if (continuer)
		{
			if (rang >= (MAX_RANG-1)) continuer = 0;

			printf("rang %d - nb coup %d/%d\n", rang+1, nb_coup, NB_MAX_COUP);
			if (rang > 2)
			{
				for(int i=0; (i<nb_coup); i++)
				{
					if (brain[i].rang == (rang - 1))
					{
						sup_element(i);
						i--;
					}
				}
			}
			rang++;
		}

	}//Fin de la simulation


	//Suppressions des coup ne pouvant être analyser
	for(int i=0; (i<nb_coup); i++)
	{
		if (brain[i].rang == (rang+1))
		{
			sup_element(i);
			i--;
		}
	}


	//Le bot analyse les résultat de la simulation et jou
	printf("bot make a choice\n");
	bot_make_a_choice(rang);

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// le bot fait un choix et jou
// le paramètre rang permet de savoir ou s'est
// arreter la simulation
void bot_make_a_choice(int rang)
{
	//init
	rang = ((rang%2) == 1) ? (rang-1) : (rang);

	//reset
	for(int c=0; c<LARGEUR_ANNALYSE; c++)
	{
		tab_stat[c].nombre = 0;
		tab_stat[c].score  = 0;
		tab_stat[c].min    = 99999;
		tab_stat[c].total  = 0;
	}

//Calcule de la qualité des coup de référence en fonction des résultat de la simulation.
//Les coups de référence sont les coup que le Bot peut jouer dans la situation actuel.

	for(int c=0; c<LARGEUR_ANNALYSE; c++)
	{
		//on parcour tous les element in brain
		for(int i=1; i<nb_coup; i++)
		{
			//on ne prend que les dernier résultats
			if ((brain[i].rang == rang) && (brain[i].id_coup_ref == tab_coup_ref[c]))
			{
				//On prend en compte la différence de score entre les deux joueurs
				int var = ( (brain[i].score_noir) - (brain[i].score_blanc) );

				tab_stat[c].nombre += 1;
				tab_stat[c].score  += var;

				if (var < tab_stat[c].min)
				{
					tab_stat[c].min = var;
				}
			}
		}
	}

	//Annalyse des moyennes et élection du meillieur coup.
	int max_la_menace = -99999;
	int id_de_la_menace = -1;

	printf("\n");
	for(int c=0; c<LARGEUR_ANNALYSE; c++)
	{
		if (tab_stat[c].nombre > 0)
		{
			tab_stat[c].total = tab_stat[c].score / tab_stat[c].nombre;

			int coup_ref = tab_coup_ref[c];
			int nb       = tab_stat[c].nombre;
			int min      = tab_stat[c].min;
			int real     = (brain[c+1].score_noir - brain[c+1].score_blanc);
			int fictif   = tab_stat[c].total;
			int result   = fictif;

			printf("coup_ref:%d  -  real:%6d  fictif:%6d\n", coup_ref, real, fictif);

			if((result > max_la_menace))
			{
				max_la_menace = result;
				id_de_la_menace = coup_ref;
			}
		}
	}

	//Si (le bot peut jouer)
	// La map du coup-referent et copier dans la "carte" générale
	//Sinon
	// cela signifi qu'il a perdu.
	// il se retirera alors de la partie.
	if (id_de_la_menace != -1)
	{
		//le bot jou
		printf("bot a sélectionné le coup : %d\n\n\n", id_de_la_menace);
		save_move(id_de_la_menace);
		memcpy(carte, brain[id_de_la_menace].map, 8*8);
	}
	else
	{
		printf("bot lose the game...\n\n\n");
		activation_bot = false;
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// permet d'ajouter au tableau brain le meilleurs coups potentiel
// a partir d'une situation donnée par l'id.
// retourn deux etat : BOT_OK ou BOT_ERREUR (si il n'y a plus de place in brain)
int next_round(int id)
{
	int id_piece;
	int couleur_piece;
	int id_cible;
	int couleur_cible;
	int rang        = brain[id].rang;
	int n_rang      = rang+1;

	int id_coup_ref = brain[id].id_coup_ref;

	int couleur = brain[id].couleur;
	int n_couleur = 0;

	if (couleur == C_NOIR ) n_couleur = C_BLANC;
	if (couleur == C_BLANC) n_couleur = C_NOIR;


	//Le bot test toutes les possibilitées de déplacement de ces pièces
	for(int x=0; x<8; x++)
	for(int y=0; y<8; y++)
	{
		id_piece = brain[id].map[y][x];
		couleur_piece = get_color_of_unit(id_piece);

		if (couleur_piece == couleur)
		{
///******************************************************************
			for(int x2=0; x2<8; x2++)
			for(int y2=0; y2<8; y2++)
			{
				id_cible = brain[id].map[y2][x2];
				couleur_cible = get_color_of_unit(id_cible);

				if (couleur_piece != couleur_cible)
				{
					int var = deplacement_possible(brain[id].map, id_piece, x, y, x2, y2);

					if (var != D_NULL)
					{
						///ADD A NEW ELEMENT IN BRAIN
						//La fonction add() permet d'enregistrer un coup suplémentaire dans le tableau "brain"
						//Deplus cette fonction permet de calculer la valeur des score obtenu.
						var = add_element(id, id_coup_ref, n_rang, couleur, n_couleur, x, y, x2, y2, var);
						if(var == BOT_ERREUR) return(BOT_ERREUR);
					}
				}
			}
///******************************************************************
		}
	}

	//Puis en fin il compare les différents score obtenu,
	//Lors des annalyses.
	ecraimage(id, n_rang, couleur);

	return (BOT_OK);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//La fonction add() permet d'enregistrer un coup suplémentaire dans le tableau "brain"
//Deplus cette fonction permet de calculer la valeur des score obtenu.
int add_element(int id, int id_coup_ref, int n_rang, int couleur, int n_couleur, int x, int y, int x2, int y2, int var)
{
	//on vérifi qu'il y est encore de la place
	if (nb_coup < NB_MAX_COUP)
	{
		// le seul Coup in brain qui possède une valeur d' "id_coup_ref" égale à zéro et le coup d'origine.
		if   (id_coup_ref == 0)
		{
			//ce coup est alors un coup de référence
			//il est un fils du coup d'origine
			brain[nb_coup].id_coup_ref = nb_coup;
		}
		else
		{
			//Ce coup n'est pas un coup-ref, il intervient dans les round suivant
			brain[nb_coup].id_coup_ref = id_coup_ref;
		}

		brain[nb_coup].id_pere     = id;
		brain[nb_coup].rang        = n_rang;
		brain[nb_coup].couleur     = n_couleur;

		brain[nb_coup].ox    = x;
		brain[nb_coup].oy    = y;
		brain[nb_coup].dx    = x2;
		brain[nb_coup].dy    = y2;

		//modification de la map et calcule du nouveau score
		memcpy(brain[nb_coup].map, brain[id].map, 8*8);

		//transformation des pions en Dame
		if (get_type_of_unit(brain[nb_coup].map[y][x]) == P_PION)
		{
			if ((y2 == 0) && (get_color_of_unit(brain[nb_coup].map[y][x]) == C_BLANC))
			{
				brain[nb_coup].map[y2][x2] = conf_car_piece[4];
			}
			else if ((y2 == 7) && (get_color_of_unit(brain[nb_coup].map[y][x]) == C_NOIR))
			{
				brain[nb_coup].map[y2][x2] = conf_car_piece[10];
			}
			else
			{
				brain[nb_coup].map[y2][x2] = brain[nb_coup].map[y][x];
			}
		}
		else
		{
			brain[nb_coup].map[y2][x2] = brain[nb_coup].map[y][x];
		}

		//on efface l'encienne position
		brain[nb_coup].map[y][x] = conf_car_fond;

		//deplacement de la tour pour le ROQ
		if (var == D_ROQ)
		{
			int col_end = 3;
			if (x2 == 6) col_end = 5;

			int col_start = 0;
			if (x2 == 6) col_start = 7;

			int ligne;
			if  (couleur == C_NOIR) ligne = 0;
			else ligne = 7;

			brain[nb_coup].map[ligne][col_end]   = brain[nb_coup].map[ligne][col_start];
			brain[nb_coup].map[ligne][col_start] = conf_car_fond;
		}

		brain[nb_coup].score_blanc = annalyser_situation(brain[nb_coup].map, C_BLANC);
		brain[nb_coup].score_noir  = annalyser_situation(brain[nb_coup].map, C_NOIR );


		//printf("  +  id:%2d rang:%d coup_ref:%d - c'est au %s de jouer | blanc:%d  noir:%d\n", nb_coup, brain[nb_coup].rang, brain[nb_coup].id_coup_ref, COLOR(brain[nb_coup].couleur), brain[nb_coup].score_blanc, brain[nb_coup].score_noir);

		//incrémentation du conteur
		nb_coup++;
	}
	else
	{
		printf("\nbrain full..\n");
		return (BOT_ERREUR);
	}
	return(BOT_OK);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// permet de garder le coup
// le plus interressant après avoir tester tous les déplacement des pieces.
void ecraimage(int id_pere, int rang, int couleur)
{
	int id_coup_ref;
	int min;
	int id_sup = nb_coup-1;
	int nb;

	int n_couleur = 0;
	if (couleur == C_NOIR ) n_couleur = C_BLANC;
	if (couleur == C_BLANC) n_couleur = C_NOIR;

	if (id_pere == 0)
	{
		//on compte le nombre de fils
		nb = 0;
		for(int i=1; i<nb_coup; i++)
		{
			if(brain[i].rang == 1)
			{
				nb++;
			}
		}
		if (nb==0)
		{
			add_element(id_pere, brain[id_pere].id_coup_ref, rang, couleur, n_couleur, 0, 0, 0, 0, D_MOVE);
		}

		if ((nb-LARGEUR_ANNALYSE) > 0)
		{
			//On ne garde que les X meilleurs coups
			for(int c=0; c<(nb-LARGEUR_ANNALYSE); c++)
			{
				min = 999999;

				for(int i=0; i<nb_coup; i++)
				{
					if(brain[i].rang == 1)
					{
						int var;

						//if (couleur == C_NOIR) var = brain[i].score_noir  - brain[i].score_blanc;
						//else                   var = brain[i].score_blanc - brain[i].score_noir;
						if (couleur == C_NOIR) var = brain[i].score_noir;
						else                   var = brain[i].score_blanc;

						if(var < min)
						{
							min = var;
							id_sup = i;
						}
					}
				}
				//printf("  -  id:%d\n", id_sup);
				sup_element(id_sup);
			}
		}
	}
	else
	{
		id_coup_ref = brain[id_pere].id_coup_ref;

		//on compte le nombre de fils créé par le père (tous les déplacements possibles)
		nb = 0;
		for(int i=1; i<nb_coup; i++)
		{
			if((brain[i].rang == rang) && (brain[i].id_coup_ref == id_coup_ref))
			{
				nb++;
			}
		}
		if (nb==0)
		{
			add_element(id_pere, brain[id_pere].id_coup_ref, rang, couleur,  n_couleur, 0, 0, 0, 0, D_MOVE);
		}

		//Quantité de coup que l'on veut garder
		//1 ? --> afin de ne garder que le meilleur coup
		int quant = 1;

		int var = (nb-quant);
		if (var > 0)
		{

			//On ne garde que les X meilleurs coups
			for(int c=0; c<var; c++)
			{
				min = 999999;

				for(int i=0; i<nb_coup; i++)
				{
					if((brain[i].rang == rang) && (brain[i].id_coup_ref == id_coup_ref))
					{
						int var;

						//if (couleur == C_NOIR) var = brain[i].score_noir  - brain[i].score_blanc;
						//else                   var = brain[i].score_blanc - brain[i].score_noir;
						if (couleur == C_NOIR) var = brain[i].score_noir;
						else                   var = brain[i].score_blanc;

						if(var < min)
						{
							min = var;
							id_sup = i;
						}
					}
				}
				///SUPPRESSION D'UN COUP IN BRAIN
				//printf("  -  id:%d\n", id_sup);
				sup_element(id_sup);
			}

		}
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// permet de supprimer un élement in brain
void sup_element(int id)
{
	for(int i=id; i<(nb_coup-1); i++)
	{
		memcpy(brain[i].map, brain[i+1].map, 8*8);

		brain[i].score_blanc = brain[i+1].score_blanc;
		brain[i].score_noir  = brain[i+1].score_noir;
		brain[i].id_pere     = brain[i+1].id_pere;
		brain[i].id_coup_ref = brain[i+1].id_coup_ref;
		brain[i].rang        = brain[i+1].rang;
		brain[i].couleur     = brain[i+1].couleur;

		brain[i].ox = brain[i+1].ox;
		brain[i].oy = brain[i+1].oy;
		brain[i].dx = brain[i+1].dx;
		brain[i].dy = brain[i+1].dy;

		if (brain[i].id_pere == 0) brain[i].id_coup_ref = i;
	}
	nb_coup--;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************


// Cette fonction permet de calculer la puissance d'attaque des
// joueurs dans une situation données, afin de les départager
int annalyser_situation(char map[8][8], int choix_couleur)
{
	int id_piece;
	int couleur;
	int type;
	int retour = 0;

	//traitement
	for(int x=0; x<8; x++)
	for(int y=0; y<8; y++)
	{
		id_piece = map[y][x];
		couleur  = get_color_of_unit(id_piece);
		type     = get_type_of_unit (id_piece);

		if ((couleur == choix_couleur) && (id_piece != conf_car_fond))
		{
		//*/
			if      (type == P_PION)     retour += conf_val_piece[0];
			else if (type == P_TOUR)     retour += conf_val_piece[1];
			else if (type == P_CAVALIER) retour += conf_val_piece[2];
			else if (type == P_FOU)      retour += conf_val_piece[3];
			else if (type == P_DAME)     retour += conf_val_piece[4];
			else if (type == P_ROI)      retour += conf_val_piece[5];
		//*/
			retour += valeur_piece(id_piece, map, x, y);
		//*/
		}
	}

//*/
	//calcul le stress enduré par l'enemi ^^
	for(int x=0; x<8; x++)
	for(int y=0; y<8; y++)//ORIGIN
	{
		id_piece = map[y][x];
		couleur  = get_color_of_unit(id_piece);
		type     = get_type_of_unit (id_piece);

		if ((couleur != choix_couleur) && (id_piece != conf_car_fond))
		{
			if      (type == P_PION)     retour -= conf_val_piece[0];
			else if (type == P_TOUR)     retour -= conf_val_piece[1];
			else if (type == P_CAVALIER) retour -= conf_val_piece[2];
			else if (type == P_FOU)      retour -= conf_val_piece[3];
			else if (type == P_DAME)     retour -= conf_val_piece[4];
			else if (type == P_ROI)      retour -= conf_val_piece[5];
		}
	}
//*/

	return (retour);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de calculer la puissance d'attaque
// d'un piece dans une situation données
int valeur_piece(int id_unit, char map[8][8], int pos_x, int pos_y)
{
	int var, retour=0;

	for (int y=0; y<8; y++)//DEST
	for (int x=0; x<8; x++)
	{
		var = deplacement_possible(map, id_unit, pos_x, pos_y, x, y);

		if (var == D_MOVE)
		{
			retour += valeur_move(id_unit, map, pos_x, pos_y, x, y);
		}
		else if (var == D_EAT)
		{
			retour += valeur_eat(id_unit, map, pos_x, pos_y, x, y);
		}
	}

	//detect si id_unit est ciblé par l'énemi et non défendu
	for (int y=0; y<8; y++)//ORIGIN
	for (int x=0; x<8; x++)
	{
		int id = map[y][x];
		var = deplacement_possible(map, id, x, y, pos_x, pos_y);

		if (var == D_EAT)
		{
			retour += valeur_def(id_unit, map, pos_x, pos_y, x, y);
		}
	}

	return (retour);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si une case
// de deplacement est attaquable par l'enemi
int valeur_move(int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y)
{
	int id;
	int var;
	int couleur;
	int couleur_piece = get_color_of_unit(id_unit);

	for (int y=0; y<8; y++)//ORIGIN
	for (int x=0; x<8; x++)
	{
		id = map[y][x];
		couleur = get_color_of_unit(id);

		if ((id != conf_car_fond) && (couleur != couleur_piece))
		{
			var = deplacement_possible(map, id, x, y, cible_x, cible_y);

			if (var == D_MOVE)
			{
				return (0);
			}
		}
	}

	return (conf_val_gain_move);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si une piece
// est bien defendu
int valeur_def(int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y)
{
	int id;
	int var;
	int couleur;
	int couleur_piece = get_color_of_unit(id_unit);

	int val_piece;
	int val_cible;

	val_piece = conf_val_piece[get_type_of_unit(id_unit)];
	val_cible = conf_val_piece[get_type_of_unit(map[cible_y][cible_x])];

	// la cible peut me manger
	// si ma piece vo moins chère que celle de mon adverssaire
	// qu'elle soit def ou non, elle est quasiment perdu..

	for (int y=0; y<8; y++)//ORIGIN
	for (int x=0; x<8; x++)
	{
		id = map[y][x];
		couleur = get_color_of_unit(id);

		if ((id != conf_car_fond) && (couleur == couleur_piece))
		{
			var = deplacement_possible(map, id, x, y, pos_x, pos_y);

			if (var == D_DEF)
			{
				if(val_cible < val_piece)
				{
					return (-val_piece);
				}
				return (0);
			}
		}
	}

	return (-val_piece);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir si un
// echange de piece est profitable
int valeur_eat(int id_unit, char map[8][8], int pos_x, int pos_y, int cible_x, int cible_y)
{
	int id;
	int var;
	int couleur;
	int couleur_piece = get_color_of_unit(id_unit);
	int val_piece;
	int val_cible;

	val_piece = conf_val_piece[get_type_of_unit(id_unit)];
	val_cible = conf_val_piece[get_type_of_unit(map[cible_y][cible_x])];

	for (int y=0; y<8; y++)//ORIGIN
	for (int x=0; x<8; x++)
	{
		id = map[y][x];
		couleur = get_color_of_unit(id);

		if ((id != conf_car_fond) && (couleur != couleur_piece))
		{
			var = deplacement_possible(map, id, x, y, cible_x, cible_y);

			if (var == D_DEF)
			{
				return (0);
			}
		}
	}

	//PETITE COQUILLE CETTE VALEUR DEVRAIS ETRE MODIFIABLE DANS LE FICHIER DE CONFIG
	return (10);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************


// Cette fonction permet de tester les annalyses
void run_annalyse(char map[8][8])
{
	int a, b, c;

	a = annalyser_situation(map, C_BLANC);
	b = annalyser_situation(map, C_NOIR );
	c = a - b;

	printf("  Blanc:%4d  Noir:%4d  B-N:%4d \n", a, b, c);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'enregistrer
// le déplacement d'un piece
void save_move(int id)
{
	origin.x = brain[id].ox;
	origin.y = brain[id].oy;
	dest.x = brain[id].dx;
	dest.y = brain[id].dy;
	save_dep = 1;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher
// la position de départ et de fin d'une piece en bleu
void afficher_move()
{
	if (save_dep == 1)
	{
		afficher_info(D_NULL, origin.x, origin.y);
		afficher_info(D_NULL,   dest.x,   dest.y);
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%





