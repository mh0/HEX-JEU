// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Programme principal : appel de la GUI, initialisation du jeu, etc.
// Auteur : Mahdi Hentati

#include "gui.h"
#include "board.h"


#include "hexagons.h"
#include "winner.h"

int main(int argc, char *argv[])
{


	
	GUI_start(); // démarrage de la GUI
	GUI_title(); 
	GUI_menu_item("Nouveau Jeu"); 
	GUI_menu_item("Sauvegarder"); 
	GUI_menu_item("Charger"); 
	GUI_menu_item("Quitter"); 
	GUI_menu_item(" "); 
	GUI_menu_item("Annuler coup"); 
	BOARD_setCurrPlayer(RED); // Initialisation du joueur courant à rouge 
	BOARD_draw(); // Initialisation de la table du jeu
	GUI_wait(); // attente des évènements
	return 0;
}
