#include "stdio.h"
#include "stdlib.h"


typedef struct _Carte{
	int figure; // 0 à 12
	int enseigne; // 0 à 3
	int main; //numéro du joueur qui possède cette carte ou 0 si elle est dans la pile
	int id; // 0 à 51
	struct _Carte* suivant;
} Carte;


typedef struct {
	Carte* tete;
} Pile;




Pile Initialiser();
int est_vide(Pile* P);
void empiler(Pile *P, int figure, int enseigne);
int depiler(Pile *P);
void detruire (Pile *P);
void pioche_n_push(Pile *P, int debut, int fin); //pioche une carte au hasard dans la pile et la replace au hasard

void initialisation(Pile *pile_jouee, Pile *pile_non_jouee); //cree la pile
void melange(Pile* pile, int nb_melange, int debut, int fin); //melange la pile





