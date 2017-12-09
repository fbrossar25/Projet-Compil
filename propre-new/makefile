#nom de l'éxécutable cible
EXEC = compil

#compilateur, analyseurs
CC = gcc
YACC = yacc -d -Wall -v
FLEX = flex #-d

#flages de compilation
LDFLAGS = -ly -ll -lfl
CFLAGS = -Wall -std=gnu99 -g -Wno-unused-function

#fichier générés par flex et yacc (sauf y.output)
GENERATED = lex.yy.c y.tab.c y.tab.h

#fichier d'entrée pour make test
INPUT = input.txt

MIPS = mips.s

VALGRIND = --track-origins=yes --leak-check=full --show-leak-kinds=all

#fichiers sources
SRC = ast lib symbol error mips
OBJ := $(addsuffix .o, $(SRC))
SRC := $(addsuffix .c, $(SRC))

#compilation du projet
all: mrproper $(EXEC)

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

test: all
	./$(EXEC) $(INPUT) $(MIPS)

valgrind: all
	valgrind $(VALGRIND) ./$(EXEC)

valgrind-test: all
	valgrind $(VALGRIND) ./$(EXEC) $(INPUT) $(MIPS)

vt : valgrind-test

mrproper: clean
	rm -f $(EXEC) y.output

remake: mrproper all

gdb: all
	gdb $(EXEC)