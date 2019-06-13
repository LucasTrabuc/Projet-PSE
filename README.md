Projet-PSE
Ce projet a pour but de recréer le jeu du "kilo de merde" pour plusieurs joueurs pouvant se connecter à distance.
Pour ne pas multiplier le nombre de configurations de jeu possibles, nous avons choisis de nous limiter aux règles pour trois joueurs.

Pour lancer le jeu, il faut d'abord décompresser le .tar, ouvrir un terminal dans le dossier Projet-PSE et écrire "make" pour le compiler. Ensuite, écrire "./serveur n°port" avec n°port choisi permet de lancer le serveur qui contrôle la partie. Les joueurs doivent eux aussi se connecter avec un terminal ouvert dans Projet-PSE en écrivant "./client adresseIP n°port", avec adresseIP étant l'adresse IP de la machine sur laquelle se trouve le serveur (localhost si sur la même machine) et n°port étant le même que pour le serveur. La partie ne commencera que lorsque les trois joueurs seront connectés.

Le jeu à trois se joue avec les 12 têtes d'un jeu de carte et le but d'une manche est d'obtenir une main composée de 4 cartes de même valeur. Au cours du jeu, lorsque c'est votre tour de jouer, on vous propose de donner une de vos cartes au joueur suivant et, à la fin du tour d'échange, il faut envoyer la lettre "a" si vous possédez une main gagnante. Si un joueur envoie la bonne lettre, les autres doivent alors envoyer la lettre "a" le plus vite possible et le dernier joueur à l'envoyer perd la manche. Lorsqu'un joueur perd une manche, il pioche une carte du paquet des 40 cartes non utilisées et reçoit un nombre différent de "kilos de merde" selon la valeur de la carte:
2-6 et 8-10 -> la valeur de la carte
valet -> 11
dame -> 12
roi -> 13
as -> 50
Le 7 a un effet spécial, c'est la "chasse d'eau" qui débarrasse le joueur de tous les "kilos de merde" accumulés jusque-là.
La partie s'arrête lorsqu'un joueur atteint 100 "kilos de merde".

Le fichier client.c contient la fonction permettant au joueur de communiquer avec le serveur.
Le fichier serveur.c contient les fonctions utilisées pour communiquer avec les clients et gérer les threads qui s'occupent de ces communications. Le thread principal s'occupe de la gestion du jeu.
Le fichier fonctions.c contient les fonctions de création et de modification du jeu utilisées par le serveur.
Le fichier fonctions.h contient notamment les structures utilisées pour représenter le jeu.

Ce projet n'est pas fini et rencontre des problèmes à l'exécution. Le système d'envoi de la lettre "a" n'est notamment pas implémenté, il est cependant possible de voir une partie commantée en bas de serveur.c montrant la structure de ce qui était prévu.
