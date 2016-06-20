// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Gère le plateau et l’historique et contrôle l’affichage (‘noyau’ du programme). 
// Auteur : Mahdi Hentati
// Révision: -

#include "point.h"
#include <stdbool.h>
#include "hexagons.h"
#include "PlayDeque.h"
#define BOARD_SIZE 11

// Board
#define BOARD_X 150
#define BOARD_Y 80

// Coordonnées du centre du premier hexagone

#define HEX_X 33
#define HEX_Y 34


/**
 * @brief Dessine la grille du plateau, stocke les polygones dessinés 
 * dans gui_board[.][.], et initialise _board[.][.] avec des hexagones
 * de couleur EMPTY
 */
void BOARD_draw();


/**
 * @brief Modifie la couleur de l’hexagone h et l’affiche sur la grille.
 * @param h
 * @param col
 */
void BOARD_setHex(Hexagon h, int col);


/**
 * @brief invoque la fonction POLYGON_contains pour trouver l’hexagone 
 * de _board[.][.]  correspondant au clic passé en paramètre (Point p).
 * @param p
 * @return NULL si le point ne correspond à aucun hexagone affiché.
 */
Hexagon BOARD_pixelToHex (Point p) ;


/**
 * @brief vérifie si le point cliqué est dans le plateau et s’il est 
 * légal de jouer à l’emplacement correspondant. Ensuite, invoque 
 * BOARD_isWinner pour vérifier si l’hexagone joué termine le jeu, 
 * sinon, le joueur courant est modifié.
 * @param p
 */
void BOARD_playAt(Point p);


/**
 * @brief Retourne true si l’hexagone désigné est libre
 * @param hex
 * @return 
 */
bool BOARD_isLegalPlay (Hexagon hex) ;


/**
 * @brief Modifie la couleur du joueur courant. 
 * @param color
 */
void BOARD_setCurrPlayer (int color);


/**
 * @brief  Réinitialise le plateau et commence un nouveau jeu.
 */
void BOARD_reset();


/**
 * @brief Annule le dernier coup joué.
 */
void BOARD_undo();


/**
 * @brief Destruction du plateau pour libérer la mémoire
 */
void BOARD_destroy();


/**
 * @brief Charge une partie à partir de l’historique passé en paramètre.
 * @param d
 */
void BOARD_load(PlayDeque d);


/**
 * @brief Enregistre l’historique du jeu en cours.
 */
void BOARD_save();