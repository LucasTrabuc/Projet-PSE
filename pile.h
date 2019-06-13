#include "stdio.h"
#include "stdlib.h"
//#include "header.h"



Pile Initialiser();
int est_vide(Pile* P);
void empiler(Pile *P, int figure, int enseigne);
int depiler(Pile *P);
void detruire (Pile *P);
Carte* pioche(Pile *P, int debut, int fin);
