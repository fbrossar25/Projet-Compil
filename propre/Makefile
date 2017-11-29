#nom de l'éxécutable cible
EXEC = compil

#compilateur, analyseurs
CC = gcc
YACC = yacc -d -Wall -v
FLEX = flex -d

#flages de compilation
LDFLAGS = -ly -ll -lfl
CFLAGS = -Wall -std=gnu99 -g

#fichier générés par flex et yacc (sauf y.output)
GENERATED = lex.yy.c y.tab.c

#fichier d'entrée pour make test
INPUT = input.txt

#fichiers sources
SRC = ast lib
OBJ := $(addsuffix .o, $(SRC))
SRC := $(addsuffix .c, $(SRC))

#compilation du projet
all: $(EXEC)

$(EXEC): $(OBJ) $(GENERATED)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f *.o

lex.yy.c: $(EXEC).l
	$(FLEX) $<

y.tab.c: $(EXEC).y
	$(YACC) $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(GENERATED)

test: all $(EXEC)
	cat $(INPUT) | ./$(EXEC)

valgrind: all $(EXEC)
	valgrind ./$(EXEC)

valgrind-test: all $(EXEC)
	cat $(INPUT) | valgrind ./$(EXEC)

vt : valgrind-test

mrproper: clean
	rm -f $(EXEC) y.output

remake: mrproper all
