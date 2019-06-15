#include "stdio.h"
#include "stdlib.h"


typedef struct _Carte{
	int figure; // 0 à 12
	int enseigne; // 0 à 3
	int main; //numéro du joueur qui possède cette carte ou 0 si elle est dans la pile
	int id; // 0 à 51
	struct _Carte* suivant;
} Carte;


typedef struct{
	Carte* tete;
} Pile;

typedef struct{
	Pile* main;//pile qui varie de 3 à 5 cartes
	int rang_joueur; //1er 2eme ou 3eme
	int points;
	
} Joueur;






void Affichage_carte(int n_joueur, Joueur** joueurs);
Pile* Initialiser(); // création d'une pile, initialise la tete à NULL et renvoie un pointeur
int est_vide(Pile* P);
void empiler(Pile *P, Carte* carte);
Carte* depiler(Pile *P);
void detruire (Pile *P);
void pioche_n_push(Pile *P, int debut, int fin); //pioche une carte au hasard dans la pile et la replace au hasard

void initialisation(Pile *pile_jouee, Pile *pile_non_jouee); //cree la pile
void melange(Pile* pile, int nb_melange, int debut, int fin); //melange la pile, nb_melange représente le nombre de fois où une carte est prelevée au hasard et repositionnée au hasard
void distribution(Pile* pile, Joueur** joueurs); //distribue les cartes aux joueurs à partir du deck melangé

void Transfert_carte(Joueur** joueurs, int donneur, int receveur, int choix_carte); //transfère la carte choisie par le joueur donneur au joueur receveur


