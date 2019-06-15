# PROJET : Fichier Makefile
#

include ../Makefile.inc

EXE = serveur client

all: ${EXE}

serveur: serveur.o fonctions.o
	gcc -o $@ $^ $(LDLIBS)
	
fonctions.o: fonctions.c
	gcc -o $@ -c $< $(CFLAGS)
	
serveur.o: serveur.c fonctions.h
	gcc -o $@ -c $< $(CFLAGS)


	


	


