carte.o: carte.c header.h
	gcc -c carte.c
	
pile.o: pile.c header.h
	gcc -c pile.c
	
main.o: main.c header.h
	gcc -c main.c
	
main: main.o pile.o carte.o
	gcc main.o pile.o carte.o -o main
