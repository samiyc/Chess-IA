/*!==================================================================
Fichier : constantes.h
Projet  : MathFail
Auteur  : Samuel Yanez-carbonell
Date    : 10/06/2010
Version : 1.2 (11/09/2010)
===================================================================*/
#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//LES INCLUDES ======================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//LES DEFINES =======================================================
#define NB_MAX_COUP 1000
#define TAILLE_CASE 80
#define MAX_CHAINE 100


//LES STRUCT ========================================================
typedef struct {
	int x, y;

} struct_point;

typedef struct {
	char map[8][8];
	int score_blanc;
	int score_noir;
	int id_pere;
	int id_coup_ref;
	int rang;
	int couleur;
	int ox, oy;
	int dx, dy;

} struct_coup;

typedef struct {
	int nombre;
	int score;
	int min;
	int total;

} struct_stat;


//LES VARIABLES GLOBALES ============================================
char carte[8][8];              //gestion carte
struct_coup brain[NB_MAX_COUP];//gestion bot
int nb_coup;                   //gestion bot


//ENUMERATION =======================================================
enum enum_orientation {O_NULL, O_UP, O_DOWN, O_LEFT, O_RIGHT, O_NW, O_SW, O_SE, O_NE};
enum enum_case        {C_NULL, C_NOIR, C_BLANC     };
enum enum_deplacement {D_NULL, D_MOVE, D_EAT, D_DEF, D_ROQ};
enum enum_pieces
{
	P_NULL     = -1,
	P_PION     = 0,
	P_TOUR     = 1,
	P_CAVALIER = 2,
	P_FOU      = 3,
	P_DAME     = 4,
	P_ROI      = 5
};
enum enum_bot {BOT_OK, BOT_ERREUR};
enum enum_king {KING_OK, KING_FAIL, KING_IS_DIED};



//*******************************************************************
#endif // CONSTANTES_H_INCLUDED
