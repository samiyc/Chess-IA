/*!==================================================================
Fichier : gestion_carte.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void charger_la_map           ();
	void afficher_les_pieces      ();

	int  get_piece                (int x, int y);
	void put_piece                (int car_piece, int x, int y);

	int  get_color_of_unit        (int car_piece);
	int  get_type_of_unit         (int car_piece);

===================================================================*/


//LES VARIABLES GLOBALES ============================================

char tmp_map[8][8];


//LES FONCTIONS =====================================================

// Cette fonction permet de charger la carte
// depuis le fichier /data/map.txt
void charger_la_map()
{
    int i, j;
    FILE* fichier;
    char temp[9*9];

    if ((fichier = fopen("data/map.txt", "r")) == NULL)
    {
    	fprintf(stderr, "data/map.txt est introuvable");
        system("pause");
        exit(-1);
    }
    else
    {
        fread(temp, 1, 9*9, fichier);

        for(i=0; i<9*9; i++)
        {
            if (temp[i] == 10)
            {
                for(j=i; j<9*9; j++)
                {
                    temp[j] = temp[j+1];
                }
            }
        }

        temp[8*8] = '\0';

		for(i=0; i<8; i++)
        {
            for(j=0; j<8; j++)
			{
				carte[i][j] = temp[i*8+j];
			}
        }

		fclose(fichier);

		memcpy(tmp_map, carte, 8*8);
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet d'afficher les pièces
// attention il faut que les images des pièces
// soit chargées en mémoire
void afficher_les_pieces()
{
    SDL_Rect position;

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            for(int k=0; k<12; k++)
            {
                if (carte[i][j] == conf_car_piece[k])
                {
                    position.x = j*TAILLE_CASE;
                    position.y = i*TAILLE_CASE;
                    SDL_BlitSurface(img_piece[k], 0, screen, &position);
                    k=12;
                }
            }
        }
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************


// Permet de saisir un piece sur la carte (utilisateur)
int get_piece(int x, int y)
{
	int id = carte[y][x];
	carte[y][x] = conf_car_fond;
	return (id);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Permet de poser une piece sur la carte (utilisateur)
void put_piece(int car_piece, int x, int y)
{
	//transformation des pions en Dame
	if (get_type_of_unit(car_piece) == P_PION)
	{
		if ((y == 0) && (get_color_of_unit(car_piece) == C_BLANC))
		{
			carte[y][x] = conf_car_piece[4];
		}
		else if ((y == 7) && (get_color_of_unit(car_piece) == C_NOIR))
		{
			carte[y][x] = conf_car_piece[10];
		}
		else
		{
			carte[y][x] = car_piece;
		}
	}
	else
	{
		carte[y][x] = car_piece;
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//************************ HEADER REPERE ****************************


// Cette fonction permet de savoir de quel couleur est la pièces
int get_color_of_unit(int car_piece)
{
	if (car_piece == conf_car_fond) return (C_NULL);

	for (int i=0; i<12; i++)
	{
		if (car_piece == conf_car_piece[i])
		{
			if(i < 6)
			{
				return (C_BLANC);
			}
			else
			{
				return (C_NOIR);
			}
		}
	}
	return (C_NULL);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// Cette fonction permet de savoir de quel type est la pièces
int get_type_of_unit(int car_piece)
{
	if (car_piece == conf_car_fond) return (P_NULL);

	int id = -1;

	for (int i=0; i<12; i++)
	{
		if (car_piece == conf_car_piece[i])
		{
			if(i < 6)
			{
				id = i;
			}
			else
			{
				id = i - 6;
			}
			i = 12;
		}
	}

	return(id);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%






