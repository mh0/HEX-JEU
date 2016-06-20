// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Gère l’interface graphique et les entrées/sorties avec les autres modules.
// Auteur : Mahdi Hentati - Moussa Brahim
// Révision: -


#define WINHI 500 // window height
#define WINWI 700 // window width

#include <SDL/SDL.h>
#include "polygon.h"


/**
 * @brief initialise l’interface graphique et crée la fenêtre.
 */
void GUI_start();


/**
 * @brief Dessine les contours d’un hexagone défini par sa taille et son centre de coordonnées (X,Y) 
 * @param size
 * @param X
 * @param Y
 * @param col
 * @return Polygone représentant l'hexagone dessiné
 */
Polygon GUI_drawHexagon (double size, int X, int Y, Uint32 col ) ;


/**
 * @brief Attend et gère les évènements et les clics de l’utilisateur.
 * @return 
 */
int GUI_wait();


/**
 * @brief affiche un point de couleur RED/BLUE/EMPTY au point de cordonnées (x,y).
 * @param x
 * @param y
 * @param color
 */
void GUI_setHex(int x, int y, int color);


/**
 * @brief Modifie le texte affiché dans la barre d’informations (dernier coup, tour, etc.)
 * @param msg
 */
void GUI_message(char* msg);


/**
 * @brief Affiche le titre et les auteurs du jeu.
 */
void GUI_title(); 


/**
 * @brief Ajoute un item au menu
 * @param txt
 */
void GUI_menu_item(char *txt);
