//On suppose que l'on joue avec les règles pour 3 personnes
//pour compiler make main 
//pour executer ./main

#include "header.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"


const char* Figure[] = {"As", "Deux", "Trois", "Quatre", "Cinq", "Six", "Sept", "Huit", "Neuf", "Dix", "Valet", "Dame", "Roi"};
const char* Enseigne[] = {"Carreaux", "Coeur", "Pique", "Trefle"};



int main(void){
	srand(time(NULL));
	
	//CREATION DES PILES
	
	Pile* pile_jouee = Initialiser(); //12 cartes ensuite repartie entre les joueurs
	Pile* pile_non_jouee = Initialiser(); //40 cartes qui peuvent être piochée (kilos de merde)
	initialisation(pile_jouee, pile_non_jouee);
	printf("Initialisation terminee\n");
	
	//MELANGE DES PILES
	
	
	melange(pile_non_jouee, 100, 0, 40);
	melange(pile_jouee, 100, 40, 52);
	
	Carte* parcours = (Carte*) malloc(sizeof(Carte));
	parcours = pile_jouee->tete;
	printf("Pile jouee melangee\n");
	while(parcours!=NULL){
		printf("%s de %s id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}
	
	//CREATION JOUEURS
	
	Joueur* joueurs[3];	
	
	
	//DISTRIBUTION
	
	distribution(pile_jouee,joueurs);
		
	//Carte* parcours1 = (Carte*) malloc(sizeof(Carte));
	/*for(int i=0; i<=2; i++){
		parcours = joueurs[i]->main->tete;
		while(parcours!=NULL){
			printf("joueur %d : %s de %s id = %d\n", i, Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
			parcours = parcours -> suivant;
		}
	}
	
	parcours = pile_jouee->tete;
	printf("Pile jouee melangee\n");
	while(parcours!=NULL){
		printf("%s de %s id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}*/
	
	return 0;
}
