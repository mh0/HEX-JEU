// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Contient les sous-programmes permettant de gérer un point/pixel (création, modification...)
// Auteur : Mahdi Hentati
// Révision: -


typedef struct t_Point * Point;

/**
 * @brief crée un point avec les coordonnées passés en paramètre.
 * @param x
 * @param y
 * @return 
 */
Point POINT_new(int x, int y);

/**
 * @brief crée un point qui a comme coordonnées le premier paramètre p translaté par le vecteur vect.
 * @param p
 * @param vect
 * @return 
 */
Point POINT_translate(Point p, Point vect);

/**
 * @brief renvoie la coordonnée x du point passé en paramètre
 * @param p
 * @return 
 */
int POINT_x(Point p);

/**
 * @brief renvoie la coordonnée y du point passé en paramètre
 * @param p
 * @return 
 */
int POINT_y(Point p);

/**
 * @brief affiche les coordonnée x et y du point passé en paramètre 
 * @param p
 */
void POINT_print(Point p);

/**
 * @brief libère la case mémoire occupée par le point passé en paramètre
 * @param p
 */
void POINT_destroy (Point p);