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
		printf("%s de %s id = %d\n", Figure[parcours->figure], Enseigne[parcours->enseigne], parcours->id);
		parcours = parcours -> suivant;
	}
	
	//CREATION JOUEURS
	
	Joueur* joueurs[3];	
	
	
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

void Affichage(
	
	ligne = 	
	ecrireLigne(dataThreadEcr[i].spec.canal,ligne);
	
void Transfert_carte(Joueur** joueurs, int donneur, int receveur, int choix_carte)
{
	//On récupère la carte du donneur
	
	Carte* temp = malloc(sizeof(Carte));
	if(choix_carte == 1)
		temp = depiler(joueurs[donneur]);
	else
	{
		Carte* courant;// = malloc(sizeof(Carte));
		courant = joueur[donneur]->tete;
		for(int i=1; i<choix_carte; i++)
			courant = courant->suivant;
		temp = courant->suivant;
		courant->suivant = temp->suivant;
		temp->suivant = NULL;	
	}
	
	//on donne la carte au receveur
	
	empiler(joueurs[receveur], temp);
}


	
		
		
	
	
	
	
	
	
	
	
	
	
	
	

