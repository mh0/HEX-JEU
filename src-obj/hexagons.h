// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Définit l'hexagone par ses attributs (coordonnées dans 
// plateau et couleur) et fournit les fonctions qui le gèrent.
// Auteur : Mahdi Hentati
// Révision: -


// Couleurs de l'hexagone
#define EMPTY 0
#define BLUE 1
#define RED 2

// Coordonnées du centre du premier hexagon
// (auquelles il faut ajouter BOARD_X,Y ) 
#define _1x1_X 19 
#define _1x1_Y 183

typedef struct t_hexagon* Hexagon ;


/**
 * @brief Crée un hexagone
 * @param x
 * @param y
 * @param Couleur de l'hexagone
 * @return Hexagon
 */
Hexagon HEXAGON_new (int x , int y , int color);

/**
 * @brief Change la couleur de l'hexagone
 * @param h: Hexagone
 * @param new_color
 */
void HEXAGON_setColor(Hexagon h, int new_color);

/**
 * @brief Affiche les données de l'hexagone
 * @param h
 */
void HEXAGON_print(Hexagon h);

/**
 * @brief retourne la coordonnée x de l'hexagone
 * @param h
 * @return 
 */
int HEXAGON_x(Hexagon h);

/**
 * @brief retourne la coordonnée y de l'hexagone
 * @param h
 * @return 
 */
int HEXAGON_y(Hexagon h);

/**
 * @brief retourne la couleur de l'hexagone
 * @param h
 * @return 
 */
int HEXAGON_col(Hexagon h);


/**
 * @brief copie en profondeur d'un hexagone
 * @param h
 * @return 
 */
Hexagon HEXAGON_copy(Hexagon h);