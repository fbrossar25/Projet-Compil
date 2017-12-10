#nom de l'éxécutable cible
EXEC = compil

#compilateur, analyseurs
CC = gcc
YACC = yacc -d -Wall -v
FLEX = flex -d

#flages de compilation
LDFLAGS = -ly -ll -lfl
CFLAGS = -Wall -std=gnu99 -g -Wno-unused-function

#fichier générés par flex et yacc (sauf y.output)
GENERATED = lex.yy.c y.tab.c y.tab.h

#fichier d'entrée pour make test
INPUT = input.c

MIPS = mips.s

VALGRIND = --track-origins=yes --leak-check=full --show-leak-kinds=all

MARS = java -jar Mars4_5.jar

#fichiers sources
SRC = ast quads symbol error mips alloc
OBJ := $(addsuffix .o, $(SRC))
SRC := $(addsuffix .c, $(SRC))

#compilation du projet
all: mrproper $(EXEC)

$(EXEC): $(GENERATED) $(OBJ)
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

test: all
	./$(EXEC) $(INPUT) $(MIPS)

valgrind: all
	valgrind $(VALGRIND) ./$(EXEC)

valgrind-test: all
	valgrind $(VALGRIND) ./$(EXEC) $(INPUT) $(MIPS)

vt: valgrind-test

mars:
	$(MARS) mips.s

mars-test: all mars

mrproper: clean
	rm -f $(EXEC) y.output

remake: mrproper all

gdb: all
	gdb $(EXEC)