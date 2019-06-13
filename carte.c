#include "header.h"


void initialisation(Pile *pile_jouee, Pile *pile_non_jouee){ //création des deux piles de cartes
	for(int n = 0; n <= 39; n++){
		Carte* carte = (Carte*) malloc(sizeof(Carte)); 
		carte->id = n;
		empiler(pile_non_jouee, carte);
	}
	for(int n = 40; n <= 51; n++){
		Carte* carte = (Carte*) malloc(sizeof(Carte));
		carte->id = n; 
		empiler(pile_jouee, carte);
	} 
	Carte* courant = pile_non_jouee->tete;
	for(int figure=9; figure>=0; figure--){
		for(int enseigne = 3; enseigne >=0; enseigne--){
			courant->figure = figure;
			courant->enseigne = enseigne;
			courant = courant->suivant;
		}
	}
	courant = pile_jouee->tete;
	for(int figure=12; figure>=10; figure--){
		for(int enseigne = 3; enseigne >=0; enseigne--){
			courant->figure = figure;
			courant->enseigne = enseigne;
			courant = courant->suivant;	
		}
	}
}

void melange(Pile* pile, int nb_melange, int debut, int fin){
	for(int i=0; i<nb_melange; i++){ //pour le jeu_non_joue
		pioche_n_push(pile, debut, fin);
		
	}
	Carte* parcours = (Carte*) malloc(sizeof(Carte));
	parcours = pile->tete;
}

void distribution(Pile* pile, Joueur** joueurs){
	
	Carte* carte_donnee = malloc(sizeof(Carte));			
	for(int n_joueur = 0; n_joueur <= 2; n_joueur++){
		Joueur* joueur = malloc(sizeof(Joueur));
		Pile* Main = malloc(sizeof(Pile));
		Main = Initialiser();
		joueur->main = Main;
		for(int i=0; i<=3; i++){
			carte_donnee = depiler(pile);
			empiler(joueur->main, carte_donnee);
		}
		joueurs[n_joueur] = joueur;
		
	}
	
} 
		
			
			



	
		
