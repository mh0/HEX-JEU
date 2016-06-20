// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Enregistre / charge l’état du plateau 
// Auteur : Moussa Brahim
// Révision: -

#include "PlayDeque.h"
#include "hexagons.h"
#include <stdbool.h>


/**
 * @brief enregistre l’état du plateau en cours dans le fichier d’enregistrement.
 * @param historique
 * @return false si l’écriture échoue.
 */
bool SAVEGAME_save (PlayDeque historique);


/**
 * @brief charge le fichier d’enregistrement 
 * @return l’historique ou NULL si échec/incohérence. 
 */
PlayDeque SAVEGAME_load ();