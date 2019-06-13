
#include "header.h"





Pile* Initialiser() 
{
	Pile* pilevide= malloc(sizeof(Pile));
	pilevide->tete = NULL; 
	return pilevide;
}

int est_vide(Pile* P) 
{
	return (P->tete == NULL);
}

void empiler(Pile *P, Carte* carte) 
{
	//printf("Figure = %s enseigne = %s id = %d\n", Figure[carte->figure], Enseigne[carte->enseigne], carte->id);

	carte->suivant = P->tete; 
	P->tete = carte;
	
}

Carte* depiler(Pile *P)
{	
	
	Carte* temp;// = malloc(sizeof(Carte));
	
	temp = P->tete;
	
	//printf("%d \n",temp->id);
	
	P->tete = P->tete->suivant;
	printf("salut\n");
	return temp;
}

void detruire (Pile *P)
{
	Carte* carte = P->tete;

	while (P->tete != NULL) 
	{
		carte = P->tete;
		P->tete = carte->suivant;
		free(carte);
	}
}

void pioche_n_push(Pile *P, int debut, int fin){ //pioche une carte au hasard dans la pile et replace cette carte dans la pile à un emplacement aléatoire

	
	//PIOCHE 
	int pioche = rand()%(fin-debut);
	//printf("\nPioche %deme carte\n\n", pioche);	
	
	//parcours de la pile
	Carte* courant = (Carte*) malloc(sizeof(Carte)); 
	courant = P->tete;
	Carte* temp;
	
	if((pioche == 0)||( pioche == 40)){
		temp = P->tete;
		P->tete = P->tete->suivant;
		temp->suivant = NULL;
		//printf("id = %d, taille pile = %d\n", courant->id, fin-debut);
	}
	else{ 
		for(int i = 1; i<pioche; i++){
			//printf("id = %d, i=%d taille pile = %d\n", courant->id, i, fin-debut);
			courant = courant->suivant;
		}
		temp = courant->suivant;
		//printf("Carte choisiee %d\n", temp->id);
		//On extrait
		courant->suivant = temp->suivant;
		temp->suivant = NULL;
	}
	
	//PUSH (replace la carte)
	courant = P->tete;
	int push = rand()%(fin-debut);
	//printf("Push %deme position\n\n",push);
	if(push==0 || pioche == 40){
		temp->suivant = P->tete;
		P->tete = temp;
	}
	else{
		for(int i = 1; i<push; i++){
			courant = courant->suivant;
		}
		temp->suivant = courant->suivant;
		courant->suivant = temp;
	}		
	
	//free
}
	
	
	
