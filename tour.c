#include "pse.h"




void tour (void){

	int nb_tour=5;
	int n=1;
	pthread_mutex_t mex1;
	pthread_mutex_t mex2;
	pthread_mutex_t mex3;
	
	pthread_mutex_init (&mex1, NULL);
	pthread_mutex_init (&mex2, NULL);
	pthread_mutex_init (&mex3, NULL);
	
	pthread_mutex_lock(&mex1);
	pthread_mutex_lock(&mex2);
	pthread_mutex_lock(&mex3);
	
	# joueur 1 joue
	
	pthread_mutex_unlock(&mex1);
	
	# envoyer ses cartes
	# demander la carte qu'il donne
	# faire attendre 3 sec
	
	# si il tape
	
		# si cartes identiques
		
			# envoyer aux autres joueurs tape !
		
		# sinon le faire piocher
		
	pthread_mutex_lock(&mex1);
	
	
	# joueur 2 joue
	
	pthread_mutex_unlock(&mex2);
	
	# envoyer ses cartes
	# demander la carte qu'il donne
	# faire attendre 3 sec
	
	# si il tape
	
		# si cartes identiques
		
			# envoyer aux autres joueurs tape !
		
		# sinon le faire piocher
		
	pthread_mutex_lock(&mex2);
	
	
	
	# joueur 3 joue
	
	pthread_mutex_unlock(&mex3);
	
	# envoyer ses cartes
	# demander la carte qu'il donne
	# faire attendre 3 sec
	
	# si il tape
	
		# si cartes identiques
		
			# envoyer aux autres joueurs tape !
		
		# sinon le faire piocher
		
	pthread_mutex_lock(&mex3);
	
	
	
	
	
	
	
	
