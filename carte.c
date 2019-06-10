#include "header.h"


void initialisation(Pile *pile_jouee, Pile *pile_non_jouee){ //création des deux piles de cartes
	for(int figure=0; figure<=9; figure++){
		for(int enseigne = 0; enseigne <=3; enseigne++){
			empiler(pile_non_jouee, figure, enseigne);
		}
	}
	for(int figure=10; figure<=12; figure++){
		for(int enseigne = 0; enseigne <=3; enseigne++){
			empiler(pile_jouee, figure, enseigne);			
		}
	}
	//verification
	/*Carte* courant = (Carte*) malloc(sizeof(Carte)); 
	courant = pile_non_jouee->tete;
	while(courant!=NULL){
		printf("verif id = %d\n", courant->id);
		courant = courant->suivant;
	}*/
}

void melange(Pile* pile, int nb_melange, int debut, int fin){
	for(int i=0; i<nb_melange; i++){ //pour le jeu_non_joue
		pioche_n_push(pile, debut, fin);
		
	}
	Carte* parcours = (Carte*) malloc(sizeof(Carte));
	parcours = pile->tete;
	//verification
	/*while(parcours!=NULL){
		printf("Melange %d\n", parcours->id);
		parcours = parcours -> suivant;
	}*/
}


	
		
