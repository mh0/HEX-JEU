// Basé sur l'exemple posté sur moodle par V. Dugat.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <assert.h>
#include "hexagons.h"
#include "gui.h"
#include "board.h"
#include "point.h"
#include "polygon.h"

#define PI 3.14159265358979323846

#define NO_DRAW 1 // Ne pas redessiner la grille (trop moche)

#define MENU_ITEM_W 20 // des constantes pour détecter
#define MENU_X 18 // les clics du menu

/* SDl */
static Uint32 initflags = SDL_INIT_VIDEO;
static Uint32 videoflags = SDL_HWSURFACE; // utiliser la mémoire vidéo
static Uint8  video_bpp = 32; // 32 bits de couleur
static SDL_Surface *ecran;
static SDL_Surface *board;
static SDL_Surface *texte1;
static TTF_Font *fontMenu;

static int menu_count =1; // compteur

static void GUI_clean();
static void exit_game();


// variables et fonctions relatives aux actions du menu
static Point current_click;
static int inside_rect(int xB, int yB)
{

	int x=POINT_x(current_click);
	int y=POINT_y(current_click);
	return (x>MENU_X && x<xB && y<yB && y>(yB-MENU_ITEM_W));
}


// détection des clics sur le menu
static void GUI_menu_click (Point P)
{
	current_click=P;
	if (inside_rect(144,145))
		BOARD_reset();
	else if (inside_rect(149,175))
		BOARD_save();
	else if (inside_rect(103,205))
		BOARD_load(NULL);
	else if (inside_rect(97,235))
		exit_game();
	else if (inside_rect(149,294))
		BOARD_undo();
}

static void exit_game()
{
	printf("Jeu terminé!\n");
	BOARD_destroy();
	GUI_clean();
	exit(0);
}


void GUI_start ()
{
	// basé sur l'exemple donné sur moodle (Vincent Dugat)

	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 )
	{
		fprintf(stderr, "N'arrive pas à initialiser la SDL : %s\n", SDL_GetError());
		exit(1);
	}

	/* Set video mode */
	ecran = SDL_SetVideoMode(WINWI, WINHI, video_bpp, videoflags); // surface principale
	if (ecran == NULL)
	{
		fprintf(stderr, "N'arrive pas a` etablir mode video%dx%dx%d : %s\n", WINWI, WINHI, video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	SDL_WM_SetCaption("Jeu de Hex", NULL); // legende de la fenêtre
}

// Titre et nom d'auteur
void GUI_title()
{
	TTF_Font *fontMenu2;
	SDL_Surface *texte2;

	TTF_Init();

	fontMenu = TTF_OpenFont("Fonts/AltamonteNF.ttf",45);
	fontMenu2 = TTF_OpenFont("Fonts/AltamonteNF.ttf",20);
	SDL_Color fontCol = {42,42,42};

	texte1 = TTF_RenderText_Blended(fontMenu,"Jeu de Hex",fontCol);
	texte2 = TTF_RenderText_Blended(fontMenu2,"Mahdi Hentati - Brahim Moussa (2016)",fontCol);

	SDL_Rect  posTexte1, posTexte2 ;
	posTexte2.x = 150;
	posTexte2.y = BOARD_Y-40 ;

	posTexte1.x = 200;
	posTexte1.y = posTexte2.y - 50;

	SDL_BlitSurface(texte1,NULL,ecran,&posTexte1);
	SDL_BlitSurface(texte2,NULL,ecran,&posTexte2);
	TTF_CloseFont(fontMenu);
	TTF_CloseFont(fontMenu2);
	SDL_FreeSurface(texte1);
	SDL_FreeSurface(texte2);
}


void GUI_menu_item(char *txt)
{
	TTF_Init();
	fontMenu = TTF_OpenFont("Fonts/AltamonteNF.ttf",20);
	SDL_Color fontCol = {0,46,184};
	texte1 = TTF_RenderText_Blended(fontMenu,txt,fontCol);

	SDL_Rect  posTexte1;
	posTexte1.x = 20;
	posTexte1.y = menu_count*30+10+BOARD_Y;

	SDL_BlitSurface(texte1,NULL,ecran,&posTexte1);
	TTF_CloseFont(fontMenu);
	SDL_FreeSurface(texte1);
	menu_count++;
}

void GUI_message (char *txt)
{
	printf("GUI_message:%s \n",txt);
	SDL_Surface *infobar;
	infobar = IMG_Load("Images/infobar.png");

	int Y=410;
	SDL_Rect pos;
	pos.x = 0;
	pos.y = Y;
	SDL_BlitSurface(infobar,NULL,ecran,&pos);
	SDL_FreeSurface(infobar);

	TTF_Init();
	// charger une fonte
	fontMenu = TTF_OpenFont("Fonts/Calibri.ttf",20);
	SDL_Color fontCol = {0,46,184};
	texte1 = TTF_RenderText_Blended(fontMenu,txt,fontCol);

	SDL_Rect  posTexte1;
	posTexte1.x = 120;
	posTexte1.y = Y+25;

	TTF_CloseFont(fontMenu);
	SDL_BlitSurface(texte1,NULL,ecran,&posTexte1);
	SDL_FreeSurface(texte1);
}


// affiche le pion
void GUI_setHex(int x, int y, int color)
{
	assert(color>=0 && color <3);
	SDL_Surface *pion;
	if (color==BLUE)
		pion = IMG_Load("Images/button-blue22.png");
	else if (color==RED)
		pion = IMG_Load("Images/button-red22.png");
	else
		pion = IMG_Load("Images/button-empty22.png");

	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_BlitSurface(pion,NULL,ecran,&pos);
	SDL_FreeSurface(pion);
}

int GUI_wait()
{
	bool continuer; // pour la boucle principale des évènements
	SDL_Event event; // aussi

	// Affichage du plateau de jeu
	board = IMG_Load("Images/hex.png");    // plateau de jeu la taille en pixels est celle de l'image
	SDL_Rect posBoard;                     // sa position
	posBoard.x = BOARD_X;
	posBoard.y = BOARD_Y;
	SDL_BlitSurface(board,NULL,ecran,&posBoard); // on l'associe avec la surface principale

	SDL_Flip(ecran); //maj des surfaces

	// boucle des évènements
	continuer = true; // un furieux du c ferait plutôt une boucle infinie. Je préfère rester correct.
	SDLKey key_pressed ;
	while (continuer)
	{
		while ( SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int clicX = event.motion.x;
					int clicY = event.motion.y;
					Point clicked = POINT_new(clicX,clicY);
					GUI_menu_click (clicked);
					BOARD_playAt(clicked);
					POINT_destroy(clicked);
				}
				break;
			case SDL_KEYDOWN:
				key_pressed = event.key.keysym.sym; // on récupère la touche
				switch (key_pressed)
				{
				case SDLK_ESCAPE: /* Esc keypress quits the app... */
					continuer = false;
					exit_game();
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				continuer = false;
				exit_game();
				break;
			default:
				break;
			}
		}
		// refresh screen
		SDL_Flip(ecran); //maj des surfaces pour affichage
	}
	return(0);
}

static void GUI_clean()
{
	/* Clean up the SDL library */
	SDL_FreeSurface(ecran);
	SDL_FreeSurface(board);
	TTF_Quit();
	SDL_Quit(); // cause une fuite de mémoire bizzare !!
}

//   fonction de test
void GUI_drawPointxy (int x, int  y, Uint32 col)
{
	if (x >= 0 && x < ecran->w &&
	        y >= 0 && y < ecran->h)
		*((Uint32*)(ecran->pixels) + x + y * ecran->w) = col;
}

//   fonction de tests
void GUI_drawPoint (Point p, Uint32 col)
{
	int x=POINT_x(p);
	int y=POINT_y(p);
	GUI_drawPointxy(x,y,col);
}

//   fonction de tests
void swap(int* x, int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

// fonction de tests
void GUI_drawLine (Point p1 , Point p2, Uint32 col)
{
	if (NO_DRAW)
		return;
	int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
	int x1=POINT_x(p1);
	int y1=POINT_y(p1);
	int x2=POINT_x(p2);
	int y2=POINT_y(p2);


	if (abs(x2 - x1) < abs(y2 - y1))
	{


		if (y1 > y2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}

		xincr = x2 > x1 ? 1 : -1;
		dy = y2 - y1;
		dx = abs(x2 - x1);
		d = 2 * dx - dy;
		aincr = 2 * (dx - dy);
		bincr = 2 * dx;
		x = x1;
		y = y1;

		GUI_drawPointxy( x, y, col);

		for (y = y1+1; y <= y2; ++y)
		{
			if (d >= 0)
			{
				x += xincr;
				d += aincr;
			}
			else
				d += bincr;

			GUI_drawPointxy(x, y, col);
		}

	}
	else
	{


		if (x1 > x2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}

		yincr = y2 > y1 ? 1 : -1;
		dx = x2 - x1;
		dy = abs(y2 - y1);
		d = 2 * dy - dx;
		aincr = 2 * (dy - dx);
		bincr = 2 * dy;
		x = x1;
		y = y1;

		GUI_drawPointxy(x, y, col);

		for (x = x1+1; x <= x2; ++x)
		{
			if (d >= 0)
			{
				y += yincr;
				d += aincr;
			}
			else
				d += bincr;

			GUI_drawPointxy(x, y, col);
		}
	}
}


// "dessine" un polygone
void GUI_drawPolygon(Polygon p, Uint32 col)
{
	int i ;
	Point first = POLYGON_getPoint(p,0);
	Point curr ,prev;
	prev=POLYGON_getPoint(p,0);
	int nb= POLYGON_nbPoints(p);
	for ( i=1; i< nb ; i++)
	{
		curr= POLYGON_getPoint(p,i);
		GUI_drawLine(prev,curr,col);
		prev=curr;
	}
	GUI_drawLine(first,curr,col);
	POINT_destroy(first);
}


// taille, Centre(X,Y);
Polygon GUI_drawHexagon (double size, int X, int Y, Uint32 col )
{
	Polygon hex = POLYGON_new();
	int i;
	int p_x,p_y;
	double angle;
	for (i=0; i<7; i++)
	{
		angle = PI/6*(2*i+1);
		p_x= (int)round((double)X+size*cos(angle));
		p_y= (int)round((double)Y+size*sin(angle));
		POLYGON_addPoint(hex,POINT_new (p_x,p_y));
	}
	GUI_drawPolygon(hex,col);
	POLYGON_print(hex);
	return hex;
}
