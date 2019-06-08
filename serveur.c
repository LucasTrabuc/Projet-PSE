#include "pse.h"

#define    CMD      "serveur"
#define NB_WORKERS  50

void creerCohorteWorkers(void);
int chercherWorkerLibre(void);
void *threadSessionClient(void *arg);
void sessionClient(int canal);
int remiseAZeroJournal(int fdJournal);

DataThread dataThread[NB_WORKERS];
int fdJournal;
sem_t sem_mutex; //permet de gérer les workers libres



int main(int argc, char *argv[]) {
  short port;
  int ecoute, canal, ret;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  int numWorker;
  
  sem_init(&sem_mutex, 0, NB_WORKERS); //ouvre une file de 50 workers
  
  
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

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
  
  while (VRAI) {
    printf("%s: accepting a connection\n", CMD);
    canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    if (canal < 0)
      erreur_IO("accept");

    printf("%s: adr %s, port %hu\n", CMD,
	        stringIP(ntohl(adrClient.sin_addr.s_addr)),
	        ntohs(adrClient.sin_port));
	        
	        
	 
	
    /*while ( (numWorker = chercherWorkerLibre()) < 0)
      usleep(1);*/
    sem_wait(&sem_mutex); //attribution du jeton
    numWorker = chercherWorkerLibre(); //gestion d'erreur si pas de worker libre
    printf("worker %d libre\n", numWorker);
    dataThread[numWorker].spec.canal = canal;
    sem_post(&dataThread[numWorker].spec.sem); //réveille le worker
  }

  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  if (close(fdJournal) == -1)
    erreur_IO("fermeture journal");

  exit(EXIT_SUCCESS);
}

void creerCohorteWorkers(void) {
  int i, ret;
  sem_t sem_event;

  for (i = 0; i < NB_WORKERS; i++) {
    ret = pthread_create(&dataThread[i].spec.id, NULL, threadSessionClient,
                          &dataThread[i]);
    if (ret != 0)
      erreur_IO("pthread_create");
    dataThread[i].spec.libre = VRAI;
    dataThread[i].spec.canal = -1;
    sem_init(&sem_event, 0, 0); //on crée le sem de réveil pour chaque worker
    dataThread[i].spec.sem = sem_event;
  }
}

// retourne le numero du worker libre ou -1 si pas de worker libre
int chercherWorkerLibre(void) {
  int i;
  
  i = 0;
  while (dataThread[i].spec.libre == FAUX && i < NB_WORKERS)
    i++;
  if (i < NB_WORKERS)
  {
    return i;
  }
  else
    return -1;
}

void *threadSessionClient(void *arg) {
  DataThread *dataThread;
  int canal;

  dataThread = (DataThread *)arg;
  

  while (VRAI) {
    /*while (dataThread->spec.canal < 0)
      usleep(1);*/
    sem_wait(&dataThread->spec.sem); //bloquant
    
    
    	
    	dataThread->spec.libre = FAUX;
    	canal = dataThread->spec.canal;

    	sessionClient(canal);
    	dataThread->spec.canal = -1;
    	dataThread->spec.libre = VRAI;
    	sem_post(&sem_mutex); //retour du jeton
    	
    	
  }


  pthread_exit(NULL);
}

void sessionClient(int canal) {
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgLue;

  while (!fin) {
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lire ligne");
    else if (lgLue == 0) {
      printf("arret client\n");
      fin = VRAI;
    }

    printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);

    if (strcmp(ligne, "fin") == 0) {
      printf("serveur: fin client\n");
      fin = VRAI;
    }
    else if (strcmp(ligne, "init") == 0) {
      printf("serveur: remise a zero journal\n");
      fdJournal = remiseAZeroJournal(fdJournal);
    }
    else if (ecrireLigne(fdJournal, ligne) != -1) {
      printf("serveur: ligne de %d octets ecrite dans journal\n", lgLue);
    }
    else
      erreur_IO("ecriture journal");
  } // fin while

  if (close(canal) == -1)
    erreur_IO("fermeture canal");
}

// le fichier est ferme et rouvert vide, le nouveau descripteur est retourne
int remiseAZeroJournal(int fdJournal) {
  int fdNouv;

  if (close(fdJournal) == -1)
    erreur_IO("raz journal - fermeture");

  fdNouv = open("journal.log", O_WRONLY|O_TRUNC|O_APPEND, 0644);
  if (fdNouv == -1)
    erreur_IO("raz journal - ouverture");

  return fdNouv;
}
