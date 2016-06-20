// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: 
// Contient les sous-programmes qui permettent de localiser les hexagones sur la table du jeu. 
// Un polygone est défini par un ensemble de points (ordonnés).
// Auteur : Mahdi Hentati
// Révision: -

// La fonction POLYGON_contains est inspirée de la classe Polygon de OpenJDK
// http://hg.openjdk.java.net/jdk7/jdk7/jdk/file/357ecafd727b/src/share/classes/java/awt/Polygon.java
// Licence: http://www.gnu.org/licenses/old-licenses/gpl-2.0.fr.html
 
#include "point.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct t_polygon * Polygon ;

/**
 * @brief crée un nouveau polygone.
 * @return 
 */
Polygon POLYGON_new ();


/**
 * @brief ajoute un point au polygone.
 * @param P
 * @param p
 * @return 
 */
void POLYGON_addPoint (Polygon P, Point p); 


/**
 * @brief affiche les points du polygone.
 * @param P
 */
void POLYGON_print (Polygon P);


/**
 * @brief renvoie vrai si le point p est contenu dans le polygone P (contour du polygone exclu).
 * @param P
 * @param p
 * @return 
 */
bool POLYGON_contains (Polygon P, Point p) ;


/**
 * @brief translation d’un polygone par un vecteur de coordonnées (x,y)
 * @param P
 * @param x
 * @param y
 * @return 
 */
Polygon POLYGON_translate (Polygon P, int x, int y);


/**
 * @brief libère la case mémoire occupée par le polygone passé en paramètre.
 * @param P
 */
void POLYGON_destroy (Polygon P);


/**
 * @brief renvoie le point d’indice index dans le polygone
 * @param p
 * @param index
 * @return 
 */
Point POLYGON_getPoint(Polygon p, int index);


/**
 * @brief renvoie le nombre de points dans le polygone.
 * @param p
 * @return 
 */
int POLYGON_nbPoints(Polygon p);

/*
 * Unitest: 
	Polygon first_hex = POLYGON_new ();
	POLYGON_addPoint (first_hex,POINT_new(19,26));
	POLYGON_addPoint (first_hex,POINT_new(32,19));
	POLYGON_addPoint (first_hex,POINT_new(35,19));
	POLYGON_addPoint (first_hex,POINT_new(48,29));
	POLYGON_addPoint (first_hex,POINT_new(48,43));
	POLYGON_addPoint (first_hex,POINT_new(35,50));
	POLYGON_addPoint (first_hex,POINT_new(32,50));
	POLYGON_addPoint (first_hex,POINT_new(19,43));
	
	POLYGON_print(first_hex);
	
	Polygon tr = POLYGON_translate (first_hex, BOARD_X,BOARD_Y);
	POLYGON_print(tr);
	
	printf("IN ? %d \n", POLYGON_contains(tr, POINT_new(x,y))  );
	 * 
	 * */


