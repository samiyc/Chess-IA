/*!==================================================================
Nom     : config.cpp
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 02/10/2010
Version : 1.2 (11/09/2010)
---------------------------------------------------------------------
Fonctions du module :

	void lire_fichier_de_config   ();

===================================================================*/


//LES VARIABLES GLOBALES ============================================
bool activation_bot;

int level;
int MAX_RANG = 20;
int LARGEUR_ANNALYSE = 11;

//les caractères
char conf_car_piece [12];
char conf_car_fond;

//bot config
int conf_val_piece [6];
int conf_val_gain_move;


//LES FONCTIONS =====================================================

// Cette fonction permet de charger toutes les données de configuration
// depuis le fichier "data/config.txt"
// Toutes les variables de configuration commence par "conf_"
void lire_fichier_de_config()
{
	//Variables temporaires
    FILE *f_fichier;
    char s_tampon[MAX_CHAINE];
    char s_fonction[MAX_CHAINE];
    char s_valeur  [MAX_CHAINE];

	//Ouverture du fichier
    if ((f_fichier = fopen("./data/config.txt", "r")) == NULL)
    {
        fprintf(stderr, "\n# ERREUR : impossible d'acceder au fichier config.txt\n");
        system("pause");
        exit(-1);
    }

    //Lecture du fichier
    while(fgets(s_tampon, MAX_CHAINE-1, f_fichier))
    {

        if ((s_tampon[0] != '#') && (s_tampon[0] != '\n'))
        {

			sscanf(s_tampon, "%s : %s;", s_fonction, s_valeur);

			//Récupération des valeurs
			if (strcmp(s_fonction, "level") == 0)
			{
				int var  = atoi(s_valeur);
				level            = (var < 1) ? 1 : ((var > 10) ? 10 : var);

				MAX_RANG         = level*2;
				LARGEUR_ANNALYSE = level*3;
			}
			else if (strcmp(s_fonction, "activation-bot") == 0)
			{
				activation_bot = (strcmp("oui", s_valeur) == 0) ? true : false;
			}
			else if (strcmp(s_fonction, "blanc-pion") == 0)
			{
				conf_car_piece[0] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "blanc-tour") == 0)
			{
				conf_car_piece[1] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "blanc-cavalier") == 0)
			{
				conf_car_piece[2] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "blanc-fou") == 0)
			{
				conf_car_piece[3] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "blanc-dame") == 0)
			{
				conf_car_piece[4] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "blanc-roi") == 0)
			{
				conf_car_piece[5] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-pion") == 0)
			{
				conf_car_piece[6] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-tour") == 0)
			{
				conf_car_piece[7] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-cavalier") == 0)
			{
				conf_car_piece[8] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-fou") == 0)//Mario
			{
				conf_car_piece[9] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-dame") == 0)
			{
				conf_car_piece[10] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "noir-roi") == 0)
			{
				conf_car_piece[11] = s_valeur[0];
			}
			else if (strcmp(s_fonction, "case-vide") == 0)
			{
				conf_car_fond = s_valeur[0];
			}
			else if (strcmp(s_fonction, "valeur-pion") == 0)
			{
				conf_val_piece[0] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "valeur-tour") == 0)
			{
				conf_val_piece[1] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "valeur-cavalier") == 0)
			{
				conf_val_piece[2] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "valeur-fou") == 0)
			{
				conf_val_piece[3] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "valeur-dame") == 0)
			{
				conf_val_piece[4] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "valeur-roi") == 0)
			{
				conf_val_piece[5] = atoi(s_valeur);
			}
			else if (strcmp(s_fonction, "gain-move") == 0)
			{
				conf_val_gain_move = atoi(s_valeur);
			}
        }

    }//fin while
    fclose(f_fichier);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



