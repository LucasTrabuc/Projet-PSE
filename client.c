#include "pse.h"

#define CMD   "client"

int main(int argc, char *argv[]) {
  int sock, ret;
  struct sockaddr_in *adrServ;
  int fin = FAUX;
  char ligne[LIGNE_MAX];
  int lgEcr;

  if (argc != 3)
    erreur("usage: %s machine port\n", argv[0]);
	
	pid_t pid_fils;
  pid_fils = fork ();
	
  printf("%s: creating a socket\n", CMD);
  sock = socket (AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    erreur_IO("socket");

  printf("%s: DNS resolving for %s, port %s\n", CMD, argv[1], argv[2]);
  adrServ = resolv(argv[1], argv[2]);
  if (adrServ == NULL)
    erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);

  printf("%s: adr %s, port %hu\n", CMD,
	        stringIP(ntohl(adrServ->sin_addr.s_addr)),
	        ntohs(adrServ->sin_port));

  printf("%s: connecting the socket\n", CMD);

  
  // connection
  ret = connect(sock, (struct sockaddr *)adrServ, sizeof(struct sockaddr_in));
  if (ret < 0)
    erreur_IO("connect");
	else
		printf("réussi\n");
    
// fin param

// si processus père i.e.  écriture
  
  if (pid_fils!=0){
	  while (!fin) {
			if (fgets(ligne, LIGNE_MAX, stdin) == NULL)
				erreur("saisie fin de fichier\n");

			lgEcr = ecrireLigne(sock, ligne);
			if (lgEcr == -1)
				erreur_IO("ecrire ligne");
			
			printf("%s: demande envoyée\n", CMD);

			if (strcmp(ligne, "fin\n") == 0)
				fin = VRAI;
	  }

	  if (close(sock) == -1)
		erreur_IO("close socket");

	  exit(EXIT_SUCCESS);
  }
  
// si processus fils  i.e. lecture
	
	else {
		while (!fin) {
			if (lireLigne(sock, ligne) > 0)
				printf("%s \n", ligne);
		  }

		  if (close(sock) == -1)
			erreur_IO("close socket");

		  exit(EXIT_SUCCESS);
	}
}
	
	
	
	
	
	
	
  
