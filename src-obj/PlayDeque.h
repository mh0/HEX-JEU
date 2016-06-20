// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Gère la liste (Double-ended queue) des coups. Un coup est un pointeur
// vers l’hexagone joué (qui se trouve éventuellement dans _board[11][11]) 
// Auteur : Moussa Brahim
// Révision: -

#include <stdbool.h>
#include "hexagons.h"
typedef struct t_deque * PlayDeque ;

/**
 * @brief initialise la Deque
 * @return 
 */
PlayDeque PLAYDEQUE_init ();


/**
 * @brief empile l’hexagone h à la tête
 * @param d
 * @param h
 */
void PLAYDEQUE_push_front (PlayDeque d, Hexagon h);


/**
 * @brief empile l’hexagone h à la queue
 * @param d
 * @param h
 */
void PLAYDEQUE_push_back (PlayDeque d, Hexagon h) ;


/**
 * @brief retourne la tête.
 * @param d
 * @return 
 */
Hexagon PLAYDEQUE_get_front (PlayDeque d) ; 


/**
 * @brief retourne la queue.
 * @param d
 * @return 
 */
Hexagon PLAYDEQUE_get_back (PlayDeque d) ;


/**
 * @brief dépile la tête et retourne l’hexagone courant.
 * @param d
 * @return 
 */
Hexagon PLAYDEQUE_pop_front (PlayDeque d) ; 


/**
 * @brief dépile la queue et retourne l’hexagone courant.
 * @param d
 * @return 
 */
Hexagon PLAYDEQUE_pop_back (PlayDeque d) ;


/**
 * @brief vérifie si la deque d est vide
 * @param d
 * @return 
 */
bool PLAYDEQUE_empty (PlayDeque d);


/**
 * @brief affiche l’état de la deque sur stdout.
 * @param d
 */
void PLAYDEQUE_print (PlayDeque d);


/**
 * @brief Détruit la PlayDeque et ses noeuds.
 * @param d
 */
void PLAYDEQUE_destroy (PlayDeque d);


/**
 * @brief renvoie le nombre d'hexagones
 * @param d
 * @return 
 */
unsigned int PLAYDEQUE_size (PlayDeque d);


/**
 * @brief Initialise l'itérateur sur la queue de la liste
 * @param d
 */
void PLAYDEQUE_iter_init(PlayDeque d);


/**
 * @brief renvoie l'hexagone courant et passe au suivant
 */
Hexagon PLAYDEQUE_iterate();