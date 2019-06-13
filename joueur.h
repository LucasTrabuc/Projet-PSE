typedef struct
{
	Pile* main;//pile qui varie de 3 à 5 cartes
	int rang_joueur; //1er 2eme ou 3eme
}Joueur;

pthread_mutex_t tour = PTHREAD_MUTEX_INITIALIZER;

//pthread_mutex_lock(&tour);
//pthread_mutex_unlock(&tour);

sem_t sem_appui;
sem_init(&sem_appui, 0, 2);

//à chaque appui sem_wait(&sem_appui)
//à la fin du tour deux fois sem_post(&sem_appui);

/*pthread_cond_signal(&cond) avec cond qui est le passage de carte
pthread_cond_wait(&cond, &tour) à mettre au début des workers*/
	
	
