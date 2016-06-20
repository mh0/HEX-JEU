// Mahdi Hentati - Moussa Brahim
// Groupe 3
// L2 info - 2015/2016
// Jeu de Hex
// Description: Gère l’affichage des messages de débogage.
// Auteur : Mahdi Hentati
// Révision: -

//#define VERBOSE <-- Active le mode Verbose partout
#undef VERBOSE // désactivation des messages de débogage

#ifdef VERBOSE
#define DEBUG printf("\nDEBUGGER: "); printf
#else
#define DEBUG(format, args...) ((void)0)
#endif
