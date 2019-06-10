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
	Pile pile_jouee = Initialiser(); //12 cartes ensuite repartie entre les joueurs
	Pile pile_non_jouee = Initialiser(); //40 cartes qui peuvent être piochée (kilos de merde)
	
	
	initialisation(&pile_jouee, &pile_non_jouee);
	printf("Initialisation terminee\n");
	melange(&pile_non_jouee, 100, 0, 39);
	melange(&pile_jouee, 100, 40, 51);
	//test
	Carte* parcours = (Carte*) malloc(sizeof(Carte));
	parcours = pile_jouee.tete;
	printf("Pile jouee melangee\n");
	while(parcours!=NULL){
		printf("%s de %s id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}
	parcours = pile_non_jouee.tete;
	printf("Pile non jouee melangee\n");
	while(parcours!=NULL){
		printf("%s de %s id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}
	printf("termine\n");
	return 0;
}
