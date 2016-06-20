#define VERBOSE 1
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "debugging.h"
#include "gui.h"
#include "hexagons.h"
#include "polygon.h"
#include "board.h"
#include "point.h"
#include "PlayDeque.h"
#include "winner.h"
#include "savegame.h"
static int win=0;

static int BOARD_isWinner(Hexagon played_hex) ;

static Polygon gui_board[11][11];
static Hexagon _board[11][11];
static Polygon board_limits;
static PlayDeque history;
static int current_player=EMPTY ;


static void win_notify()
{
	if (current_player==RED)
		GUI_message("Rouge gagne!");
	else
		GUI_message("Bleu gagne!");
}
static void turn_notify()
{
	Hexagon last_played=PLAYDEQUE_get_front(history);
	if (last_played==NULL)
	{
		GUI_message("Nouveau jeu. Rouge commence");
		return;
	}
	char  msg[55];
	int x= HEXAGON_x(last_played);
	int y= HEXAGON_y(last_played);
	if (HEXAGON_col(last_played)==RED)
		sprintf(msg,"Dernier coup (N %d): Rouge(%d,%d). A Bleu de jouer",PLAYDEQUE_size(history),x,y);
	else if (HEXAGON_col(last_played)==BLUE)
		sprintf(msg,"Dernier coup (N %d): Bleu(%d,%d). A Rouge de jouer",PLAYDEQUE_size(history),x,y);

	GUI_message(msg);

}

static void BOARD_switchPlayer()
{
	if (current_player==BLUE)
		current_player=RED;
	else
		current_player=BLUE;

}

void BOARD_draw()
{
	board_limits = POLYGON_new();
	history=PLAYDEQUE_init ();
	POLYGON_addPoint(board_limits, POINT_new(BOARD_X+2,BOARD_Y+16));
	POLYGON_addPoint(board_limits, POINT_new(BOARD_X+163,BOARD_Y+303));
	POLYGON_addPoint(board_limits, POINT_new(BOARD_X+516,BOARD_Y+307));
	POLYGON_addPoint(board_limits, POINT_new(BOARD_X+352,BOARD_Y+15));
	// leaks de mémoire mineurs ici
	int i,j;
	for (i=0; i<11; i++)
		for (j=0; j<11; j++)
		{
			gui_board[i][j] = GUI_drawHexagon(17.0,(BOARD_X+HEX_X)+i*30 +15*j ,(BOARD_Y+HEX_Y)+j*25,900000);
			DEBUG("BOARD_draw: drawing hex of center (%d,%d)\n",(BOARD_X+HEX_X)+i*30 +15*j,(BOARD_Y+HEX_Y)+j*25 );
			_board[i][j] = HEXAGON_new(i,j,EMPTY);
		}
	WINNER_init();
	GUI_message("Nouveau jeu. Rouge commence");
}

void BOARD_reset()
{
	int i,j;
	for (i=0; i<11; i++)
		for (j=0; j<11; j++)
		{
			BOARD_setHex(_board[i][j], EMPTY);
		}
	PLAYDEQUE_destroy(history);
	history=PLAYDEQUE_init();
	GUI_message("Nouveau jeu. Rouge commence");
	current_player=RED;
	WINNER_destroy();
	win=0;
	WINNER_init();
}


void BOARD_destroy()
{
	POLYGON_destroy(board_limits);
	int i,j;
	for (i=0; i<11; i++)
		for (j=0; j<11; j++)
			free(_board[i][j]);
	PLAYDEQUE_destroy(history);
	WINNER_destroy();
}

Hexagon BOARD_pixelToHex (Point p)
{
	if (!POLYGON_contains(board_limits,p))
		return NULL;
	else
	{
		int i,j;
		for (i=0; i<11; i++)
			for (j=0; j<11; j++)
				if (POLYGON_contains( gui_board[i][j],p))
					return (_board[i][j]);
	}
	return NULL;
}

void BOARD_setHex(Hexagon h, int col)
{
	assert(h!=NULL);
	int i = HEXAGON_x(h);
	int j = HEXAGON_y(h);
	assert(i<11 && j <11 && i>-1 && j >-1);
	HEXAGON_setColor(_board[i][j], col);
	GUI_setHex((BOARD_X+HEX_X)+i*30 +15*j -10 ,(BOARD_Y+HEX_Y)+j*25 -10 ,col);
}

void BOARD_playAt(Point p)
{
	Hexagon played_hex = BOARD_pixelToHex(p);
	if (played_hex==NULL)
	{
		//printf("\nBOARD_playAt:Out of range: ");
		POINT_print(p);
		return ;
	}
	if (BOARD_isLegalPlay(played_hex))
	{
		BOARD_setHex(played_hex,current_player);
		PLAYDEQUE_push_front(history,played_hex);
		win=BOARD_isWinner(played_hex);
		turn_notify();
		if (win)
		{
			win=1;
			win_notify();
			/*int i,j;
			for (i=0; i<11; i++)
				for (j=0; j<11; j++)
					HEXAGON_setColor(_board[i][j], current_player);*/
		}
		else  // changement du tour
		{
			BOARD_switchPlayer();
			turn_notify();
		}
	}
	else if(win)
		GUI_message("Partie déjà terminée!");
}

bool BOARD_isLegalPlay (Hexagon hex)
{
	assert( hex != NULL);
	return (HEXAGON_col(hex)==EMPTY && !win) ;
}

void BOARD_setCurrPlayer (int color)
{
	assert(color==RED || color==BLUE);
	current_player=color;
}

// 0 : Aucun , RED/BLUE : couleur du gagnant
static int BOARD_isWinner(Hexagon played_hex)
{
	//return 0;
	return  WINNER_detect(played_hex);
}

void BOARD_undo()
{
	/*if (win)
	{
		GUI_message("Partie terminée! Ne peut pas annuler");
		return ;
	}*/
	if (!PLAYDEQUE_empty (history))
	{
		Hexagon  undo_hex = PLAYDEQUE_pop_front(history);
		PlayDeque d=PLAYDEQUE_init ();
		BOARD_switchPlayer();
		PLAYDEQUE_iter_init(history);
		Hexagon  foo=NULL;
		while((foo=PLAYDEQUE_iterate())!=NULL)
		{
			PLAYDEQUE_push_front(d,HEXAGON_copy(foo));
			// copie en profondeur
		}
		BOARD_load(d);
		BOARD_setHex(undo_hex,EMPTY); // affichage du pion noir
		GUI_message("Dernier coup annulé.");
	}
	else
		GUI_message("Aucun pion joué! Ne peut pas annuler");
}




void BOARD_save()
{
	if (PLAYDEQUE_empty(history)){
		GUI_message("Table vide! Ne peut pas enregister");
		return;
	}
	bool saved=SAVEGAME_save(history);
	GUI_message("Enregitrement..");
	if (!saved)
		GUI_message("L'enregistrement a échoué.");
	else
		GUI_message("Enregistré.");
}

void BOARD_load(PlayDeque h)
{
	PlayDeque d=h;
	if (d==NULL)
	{
		d=SAVEGAME_load ();
		if (d==NULL)
		{
			GUI_message("Le chargement a échoué.");
			return;
		}
	}
	assert(d!=NULL);
	if (PLAYDEQUE_empty(d))
	{
		printf("Fichier chargé vide");
		GUI_message("Fichier chargé vide");
		return;
	}

	BOARD_reset();
	Hexagon hex;
	current_player=HEXAGON_col(PLAYDEQUE_get_back(d));
	// pre-cond: col=RED/BLUE
	int prev_player=-1;
	if (current_player==BLUE)
		prev_player=RED;
	else if (current_player==RED)
		prev_player=BLUE;
	assert(prev_player!=-1);
	int x,y;
	win=0;

	while(!PLAYDEQUE_empty(d))
	{

		hex=PLAYDEQUE_pop_back(d);
		current_player=HEXAGON_col(hex);
		x=HEXAGON_x(hex);
		y=HEXAGON_y(hex);
		if (current_player==prev_player)
		{
			printf("BOARD_load: prev_player = %d",prev_player);
			printf("BOARD_load: Last Hex: %d,%d (%d)\n",x,y,current_player);
			BOARD_reset();
			GUI_message("Fichier chargé incohérent. (Couleur du joueur)");
			PLAYDEQUE_destroy(d);
			return;
		}
		if (BOARD_isLegalPlay(_board[x][y]))
		{
			BOARD_setHex(hex,current_player);
			PLAYDEQUE_push_front(history,_board[x][y]);
			free(hex);
			hex=_board[x][y];
			win=BOARD_isWinner(hex);
			if (win)
			{
				GUI_message("Fichier chargé. (Il existe un gagnant)");
				if(!PLAYDEQUE_empty(d))
				{
					BOARD_reset();
					GUI_message("Echec: il existe un gagnant mais encore des coups!");
					PLAYDEQUE_destroy(d);
					return;
				}
				return;
			}
		}
		else
		{
			BOARD_reset();
			GUI_message("Fichier chargé incohérent. (coup illégal)");
			PLAYDEQUE_destroy(d);
			return;
		}
		prev_player=current_player;
	}
	BOARD_switchPlayer();
	turn_notify();
	//BOARD_print();
	PLAYDEQUE_destroy(d);
}
