#include "pse.h"
#include "fonctions.h"

#define    CMD      "serveur"
#define NB_WORKERS  3	//choix du nombre de joueurs

void Init_jeu(void); //initialise le jeu
void creerCohorteWorkers(void);
int chercherWorkerLibre(void);
void *threadSessionClient(void *arg);
void sessionClient(int canalLec, int canalEcr, int numJoueur);
void Appui(void *arg);
void chrono(void *arg);
void GestionAppui (int canal, int fin_appui);

DataThread dataThreadLec[NB_WORKERS];
DataThread dataThreadEcr[NB_WORKERS];
int joueur_actuel;
/*semaphore de gestion des clients*/ 
sem_t sem_mutex, sem_appui;
pthread_mutex_t verrouStat= PTHREAD_MUTEX_INITIALIZER;
int tab_jeu[2] = {0,-1};
int tab_appui[2] = {0,-1};
extern Joueur * joueurs[NB_WORKERS];
pthread_barrier_t barriere_tour, barriere_appui;



int main(int argc, char *argv[]) {
  short port;
  int ecoute, canal_lec, canal_ecr, ret, lgEcr, fin_appui;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  int numWorker;
  char ligne[LIGNE_MAX];
  pthread_t thread_chrono;
  
  sem_init(&sem_mutex, 0, NB_WORKERS);//initialisation de sémaphore
  //sem_init(&sem_tour, 0, 0);//sémaphores qui servent à débloquer tous les threads en même temps
  sem_init(&sem_appui, 0,0);
  pthread_barrier_init(&barriere_tour, NULL, NB_WORKERS+1);
  pthread_barrier_init(&barriere_appui, NULL, NB_WORKERS+1);
  
  if (argc != 2)
    erreur("usage: %s port\n", argv[0]);

  port = (short)atoi(argv[1]);
  creerCohorteWorkers();

  printf("%s: creating a socket\n", CMD);
  ecoute = socket (AF_INET, SOCK_STREAM, 0);
  if (ecoute < 0)
    erreur_IO("socket");
  
  adrEcoute.sin_family = AF_INET;
  adrEcoute.sin_addr.s_addr = INADDR_ANY;
  adrEcoute.sin_port = htons(port);
  printf("%s: binding to INADDR_ANY address on port %d\n", CMD, port);
  ret = bind (ecoute,  (struct sockaddr *)&adrEcoute, sizeof(adrEcoute));
  if (ret < 0)
    erreur_IO("bind");
  
  printf("%s: listening to socket\n", CMD);
  ret = listen (ecoute, 5);
  if (ret < 0)
    erreur_IO("listen");
  
  for(int i = 0; i<=2; i++) {
    printf("%s: accepting a connection\n", CMD);
    canal_lec = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    if (canal_lec < 0)
      erreur_IO("accept");
    printf("%s: adr %s, port %hu\n", CMD, stringIP(ntohl(adrClient.sin_addr.s_addr)), ntohs(adrClient.sin_port));
    
    canal_ecr = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    if (canal_ecr < 0)
      erreur_IO("accept");
	  printf("%s: adr %s, port %hu\n", CMD, stringIP(ntohl(adrClient.sin_addr.s_addr)), ntohs(adrClient.sin_port));

    sem_wait(&sem_mutex);//attend si aucun worker libre
    numWorker = chercherWorkerLibre();
    printf("worker %d libre\n", numWorker);
    dataThreadLec[numWorker].spec.canal = canal_lec;
    dataThreadEcr[numWorker].spec.canal = canal_ecr;
    dataThreadLec[numWorker].spec.tid = i;
    dataThreadEcr[numWorker].spec.tid = i;
    sem_post(&dataThreadLec[numWorker].spec.sem);//réveille le worker
  }
  
  Init_jeu(); // INITIALISE LE JEU
  
  Carte* parcours = malloc(sizeof(Carte));
  for(int i=0;i<=2;i++){
  	parcours = joueurs[i]->main->tete;
		while(parcours!=NULL){
			printf("joueur %d : %d de %d id = %d\n", i, parcours->figure, parcours->enseigne, parcours->id);
			parcours = parcours -> suivant;
		}
	}
  
  char phrase[24] = {'Q','u','e','l','l','e',' ','c','a','r','t','e',' ','e','c','h','a','n','g','e','r',' ','?',0};
  for(int tour = 1; tour<=5 ;tour++){
  	for(int individu = 0;individu<=2;individu++){
  		int canalEcr = dataThreadEcr[individu].spec.canal;
  		joueur_actuel = individu;
  		
  		pthread_barrier_wait(&barriere_tour);
  		Affichage_carte(individu, joueurs);
  		
  		for(int i =0; i<=23; i++)
  			ligne[i] = phrase[i];
			printf("demande %d\n",individu);
  		lgEcr = ecrireLigne(canalEcr, ligne);
			if (lgEcr == -1)
		 		erreur_IO("ecrire ligne");
		 		
  		tab_jeu[0] = 0;
  		tab_jeu[1] = -1;
  		while(!tab_jeu[0]) {};
  		//printf("%d:sorti!",tab_jeu[1]);
    	Transfert_carte(joueurs, individu, (individu+1)%NB_WORKERS, tab_jeu[1]);
  	}
  	for(int individu=0;individu<NB_WORKERS;individu++)
  		Affichage_carte(individu, joueurs);
  	printf("test1");
  	tab_appui[0] = 0;
  	tab_appui[1] = -1; 
  	pthread_barrier_wait(&barriere_appui);//lance l'appui
  	
  	int time = 5000000;
  	ret = pthread_create(&thread_chrono, NULL, chrono, &time);
    if (ret != 0)
      erreur_IO("pthread_create");
      
  	while(!tab_appui[0]) {};
  	if(tab_appui[1] == -1){
  		fin_appui = 1;
  		for(int individu=0;individu<NB_WORKERS;individu++)
  			GestionAppui (dataThreadEcr[individu].spec.canal, fin_appui);
  	}
  	else {
  		fin_appui = 0;
  		for(int individu=0;individu<NB_WORKERS;individu++) {
  			if(individu != tab_appui[1])
  				GestionAppui (dataThreadEcr[individu].spec.canal, fin_appui);
  			printf("gestion perdant à ajouter\n");
  		}
  	}
  }	
  	
  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  exit(EXIT_SUCCESS);
}



void creerCohorteWorkers(void) {
  int i, ret;
  int numThread;
	sem_t sem_event;

	if ( pthread_mutex_lock( &verrouStat) != 0) {
  		perror("mutex_lock");
  		exit (EXIT_FAILURE);
  	}	
  for (i = 0; i < NB_WORKERS; i++) {
  	
  	numThread = i;
		sem_init(&sem_event,0,0);//crée 1 sémaphore par worker de lecture
    ret = pthread_create(&dataThreadLec[i].spec.id, NULL, threadSessionClient, &numThread);
    if (ret != 0)
      erreur_IO("pthread_create");
    dataThreadLec[i].spec.libre = VRAI;
    dataThreadLec[i].spec.canal = -1;
		dataThreadLec[i].spec.sem = sem_event;//sémaphore de réveil
		
		if ( pthread_mutex_lock( &verrouStat) != 0) {
  		perror("mutex_lock");
  		exit (EXIT_FAILURE);
  	}

		sem_init(&sem_event,0,0);//crée 1 sémaphore par worker d'écriture
    ret = pthread_create(&dataThreadEcr[i].spec.id, NULL, Appui, &numThread);
    if (ret != 0)
      erreur_IO("pthread_create");
    dataThreadEcr[i].spec.libre = VRAI;
    dataThreadEcr[i].spec.canal = -1;
		dataThreadEcr[i].spec.sem = sem_event;//sémaphore de réveil
	if ( pthread_mutex_lock( &verrouStat) != 0) {
  		perror("mutex_lock");
  		exit (EXIT_FAILURE);
  	}
  
  }
}

// retourne le numero du worker libre ou -1 si pas de worker libre
int chercherWorkerLibre(void) {
  int i;
  
  i = 0;
  while (dataThreadLec[i].spec.libre == FAUX && i < NB_WORKERS)
    i++;
  if (i < NB_WORKERS)
    return i;
  else
    return -1;
}

void *threadSessionClient(void *arg) {
  DataThread *dataThreadL;
  DataThread *dataThreadE;
  int canalLec, canalEcr, numJoueur;
  int *j;
	
  j = (int *)arg;
  numJoueur = *j;
  if ( pthread_mutex_unlock( &verrouStat) != 0) {
  	perror("mutex_unlock");
  	exit (EXIT_FAILURE);
  }
  dataThreadL = &dataThreadLec[numJoueur];
  dataThreadE = &dataThreadEcr[numJoueur];

  while (VRAI) {
 
    sem_wait(&dataThreadL->spec.sem);//sommeil
    dataThreadL->spec.libre = FAUX;
    canalLec = dataThreadL->spec.canal;
    canalEcr = dataThreadE->spec.canal;
    //printf("%d,%d,%d ",numJoueur,canalEcr,&(dataThreadE->spec));
    sessionClient(canalLec, canalEcr, numJoueur);
    dataThreadL->spec.canal = -1;
    dataThreadE->spec.canal = -1;
    dataThreadL->spec.libre = VRAI;
    dataThreadE->spec.libre = VRAI;
    sem_post(&sem_mutex);//dit un worker libre de plus
  }

  pthread_exit(NULL);
}


void sessionClient(int canalLec, int canalEcr, int numJoueur) {
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgLue, lgEcr;
  int carte_jouee, j;
  
	ligne[0] = 'J';
	ligne[1] = 'o';
	ligne[2] = 'u';
	ligne[3] = 'e';
	ligne[4] = 'u';
	ligne[5] = 'r';
	ligne[6] = ' ';
	ligne[7] = numJoueur+49;
	ligne[8] = 0;
  printf("écriture sur %d, canal %d\n",numJoueur, canalEcr);
  lgEcr = ecrireLigne(canalEcr, ligne);
	if (lgEcr == -1)
		 erreur_IO("ecrire ligne");

  while (!fin) {
		do {
				pthread_barrier_wait(&barriere_tour);
				j = joueur_actuel;
				ligne[0] = 'T';
				ligne[1] = 'o';
				ligne[2] = 'u';
				ligne[3] = 'r';
				ligne[4] = ' ';
				ligne[5] = 'd';
				ligne[6] = 'e';
				ligne[7] = ' ';
				ligne[8] = j+49;
				ligne[9] = 0;
				printf("affiche tour %d\n",numJoueur);
				lgEcr = ecrireLigne(canalEcr, ligne);
				if (lgEcr == -1)
		 			erreur_IO("ecrire ligne");
			} while (j != numJoueur);
			
		//while(!tab_jeu[0]) {
		  lgLue = lireLigne(canalLec, ligne);
		  if (lgLue < 0)
		    erreur_IO("lire ligne");
		  else if (lgLue == 0) {
		    printf("arret client\n");
		    fin = VRAI;
		  }

		  if (strcmp(ligne, "fin") == 0) {
		    printf("serveur: fin client\n");
		    fin = VRAI;
		  }

		  else {
		  	carte_jouee = atoi(ligne)-1;
		  	printf("carte %d\n",carte_jouee);
		  	tab_jeu[1] = carte_jouee;
		  	tab_jeu[0] = 1; 
		  }
		//}
  } // fin while

  if (close(canalLec) == -1)
    erreur_IO("fermeture canal");
  if (close(canalEcr) == -1)
    erreur_IO("fermeture canal");
}

void Appui(void *arg) {
  char ligne[LIGNE_MAX];
  int lgLue, numJoueur, canalEcr, canalLec;
  int *i;
  char *phrase = {'S','i',' ','v','o','u','s',' ','a','v','e','z','4',' ','c','a','r','t','e',' ','d','e',' ','m','ê','m','e',' ','v','a','l','e','u','r',' ','e','n','v','o','y','e','z',' ',':',' ','a',0};
  
  i = (int *)arg;
  numJoueur = *i;
  canalEcr = dataThreadEcr[numJoueur].spec.canal;
  canalLec = dataThreadLec[numJoueur].spec.canal;
  if ( pthread_mutex_unlock( &verrouStat) != 0) {
  	perror("mutex_unlock");
  	exit (EXIT_FAILURE);
  }
  while(VRAI) {
		pthread_barrier_wait(&barriere_appui);//débloque tous les threads en même temps

		for(int k=0;k<47;k++)
			ligne[k] = phrase[k];
		ecrireLigne(canalEcr, ligne);
		
		lgLue = lireLigne(canalLec, ligne);
		if (lgLue < 0)
		  erreur_IO("lire ligne");
		else {
			printf("%d a appuyé\n",numJoueur);
			tab_appui[1] = numJoueur;
			tab_appui[0] = 1;
		}
	}
}

void chrono(void *arg) {
	int *i = (int *)arg;
	int time = *i;
	
	usleep(time);
	tab_appui[1] = -1;
	tab_appui[0] = 1;
}

void GestionAppui(int canalEcr, int fin_appui) {
	char ligne[LIGNE_MAX];
	int lgEcr;
	
	if (!fin_appui) {
		char *phrase = {'Q','u','e','l','q','u',"'",'u','n',' ','a',' ','a','p','p','u','y','é',',',' ','e','n','v','o','y','e','z',' ',':',' ','a',0};
		for(int k=0;k<32;k++)
			ligne[k] = phrase[k];
		lgEcr = ecrireLigne(canalEcr, ligne);
		if (lgEcr == -1)
		 	erreur_IO("ecrire ligne");
	}
	else {
		char *phrase = {'L','e',' ','t','e','m','p','s',' ','p','o','u','r',' ','a','p','p','u','y','e','r',' ','e','s','t',' ','é','c','o','u','l','é','.',' ','P','o','u','r',' ','p','a','s','s','e','r',' ','a','u',' ','n','o','u','v','e','a','u',' ','t','o','u','r',',',' ','e','n','v','o','y','e','z',' ',':',' ','b',0};
		for(int k=0;k<75;k++)
			ligne[k] = phrase[k];
		lgEcr = ecrireLigne(canalEcr, ligne);
		if (lgEcr == -1)
		 	erreur_IO("ecrire ligne");
	}
}

