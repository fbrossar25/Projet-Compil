EXEC = compil
FILES = ast symbol
SRC = $(addsuffix .c, $(addprefix src/, $(FILES)))
OBJ = $(addsuffix .o, $(addprefix obj/, $(FILES)))
INPUT = input.txt
LDFLAGS = -ly -ll -lfl
CFLAGS = -g -std=gnu99 -Wall
GENERATED = lex.yy.c y.tab.c
CC = gcc
YACC = yacc -d -Wall -v
FLEX = flex -d

all: $(EXEC)

$(EXEC): $(OBJ) $(GENERATED)
	$(CC) $(SRC) -o $@ $(OBJ) $(GENERATED) $(LDFLAGS)

lex.yy.c: $(EXEC).l
	$(FLEX) $(EXEC).l

y.tab.c: $(EXEC).y
	$(YACC) $(EXEC).y

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf obj/ $(GENERATED)

test: all $(EXEC)
	cat resources/$(INPUT) | ./$(EXEC)

valgrind: all $(EXEC)
	valgrind ./$(EXEC)

valgrind-test: all $(EXEC)
	cat $(INPUT) | valgrind ./$(EXEC)

vt : valgrind-test

mrproper: clean
	rm -f $(EXEC) y.output

remake: mrproper all

dir :
	@if [ ! -d obj/ ]; then mkdir obj/; fi