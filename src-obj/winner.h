// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Détection de la fin de la partie (voir winner.c pour une
// description complète.
// Auteur : Mahdi Hentati
// Révision: -


/**
 * @brief initialisation des structures union-find
 */
void WINNER_init();


/**
 * @brief vérifie si played_hex est l'hexagone gagnant
 * @param played_hex
 * @return true si le dernier joueur gagne
 */
bool WINNER_detect(Hexagon played_hex);


/**
 * @brief Libération de la mémoire
 */
void WINNER_destroy();