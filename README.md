# Projet compilateur C vers MIPS

Réalisé par BROSSARD Florian et DJEBRI Maroua

Ce projet à été créée dans le cadre de la première année de
master 1 ILC à Strasbourg.

Le but est de créer un compilateur prenant en language d'entrée
un sous-ensemble de C et donnant en language de sortie un fichier en assembleur MIPS.

# Compiler le projet

Un makefile est inclus dans le projet. Voici quelques commandes: 

* make : compile le projet
* make test : compile puis lance le projet avec input.c en fichier d'entée et MIPS.s en sortie
* make vt : même chose que make test mais lance le tout avec valgrind pour vérifier la présence de fuites mémoire
* make tar : archive le dossier du projet dans son dossier parent
* make mars : lance le logiciel Mars si Mars4_5.jar est présent
* make mars-test : éxécute make test puis lance le logiciel Mars avec en paramètre le fichier de sortie MIPS.s, ce qui imprime dans le terminal le résultat.
* clean : supprime tout les fichier générés sauf y.output, MIPS.s ainsi que l'éxécutable
* mrproper : même chose que clean mais supprime aussi y.output, MIPS.s et l'éxécutable

# Pré-requis

JAVA pour utiliser Mars, gcc, bison, flex pour compiler le projet, valgrind pour les tests de fuites mémoires, tar pour l'archivage.

Sous linux :

* sudo apt-get update; sudo apt-get install build-essential openjdk-8-jdk flex bison valgrind

Le logiciel Mars :
* http://courses.missouristate.edu/kenvollmar/mars/