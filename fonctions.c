//On suppose que l'on joue avec les règles pour 3 personnes


#include "fonctions.h"
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "pse.h"

#define NB_WORKERS  3

Joueur* joueurs[NB_WORKERS];


const char Figure[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K'};
const char Enseigne[] = {'K', 'C', 'P', 'T'};
extern DataThread* dataThreadEcr;
	
void Init_jeu(void){	
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
		printf("%c de %c id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}
	
	//CREATION JOUEURS
	for(int i=0; i<=2; i++){
		Joueur *point_joueur = malloc(sizeof(Joueur));
		joueurs[i] = point_joueur;
		joueurs[i]->points = 0;
	}	
	
	
	//DISTRIBUTION
	
	distribution(pile_jouee,joueurs);
}

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
	for(int i=0; i<nb_melange; i++){ 
		pioche_n_push(pile, debut, fin);		
	}
	Carte* parcours = (Carte*) malloc(sizeof(Carte));
	parcours = pile->tete;
}



void distribution(Pile* pile, Joueur** joueurs){
	
	Carte* carte_donnee = malloc(sizeof(Carte));			
	for(int n_joueur = 0; n_joueur <= 2; n_joueur++){
		joueurs[n_joueur]->main = Initialiser();
		for(int i=0; i<=3; i++){
			carte_donnee = depiler(pile);
			empiler(joueurs[n_joueur]->main, carte_donnee);
		}		
	}
	
} 

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
	carte->suivant = P->tete; 
	P->tete = carte;
	
}

Carte* depiler(Pile *P)
{	
	
	Carte* temp;
	temp = P->tete;
	P->tete = P->tete->suivant;
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
	
	//parcours de la pile
	Carte* courant = (Carte*) malloc(sizeof(Carte)); 
	courant = P->tete;
	Carte* temp;
	
	if((pioche == 0)||( pioche == 40)){
		temp = P->tete;
		P->tete = P->tete->suivant;
		temp->suivant = NULL;
	}
	else{ 
		for(int i = 1; i<pioche; i++){
			courant = courant->suivant;
		}
		temp = courant->suivant;

		//On extrait
		courant->suivant = temp->suivant;
		temp->suivant = NULL;
	}
	
	//PUSH (replace la carte)
	courant = P->tete;
	int push = rand()%(fin-debut);

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
	
	
}

void Affichage_carte(int n_joueur, Joueur** joueurs){
	Carte* parcours;
	parcours = joueurs[n_joueur]->main->tete;
	int lgEcr;
	while(parcours!=NULL){
		char ligne[LIGNE_MAX];
		ligne[0] = Figure[parcours->figure];
		ligne[1] = Enseigne[parcours->enseigne];  
		ligne[2] = 0;
		lgEcr = ecrireLigne(dataThreadEcr[n_joueur].spec.canal,ligne);
		if (lgEcr == -1)
		 	erreur_IO("ecrire ligne");
		parcours = parcours -> suivant;
	}
	
}
	
void Transfert_carte(Joueur** joueurs, int donneur, int receveur, int choix_carte)
{
	//On récupère la carte du donneur
	
	Carte* temp = malloc(sizeof(Carte));
	if(choix_carte == 1)
		temp = depiler(joueurs[donneur]->main);
	else
	{
		Carte* courant;
		courant = joueurs[donneur]->main->tete;
		for(int i=1; i<choix_carte; i++)
			courant = courant->suivant;
		temp = courant->suivant;
		courant->suivant = temp->suivant;
		temp->suivant = NULL;	
	}
	
	//on donne la carte au receveur
	
	empiler(joueurs[receveur]->main, temp);
}


	
		
		
	
	
	
	
	
	
	
	
	
	
	
	

