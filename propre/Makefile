prefixe=compil
files=
target=compil
input=input.txt
link=-ly -ll -lfl
arg=-g -std=gnu99
generated=y.tab.c lex.yy.c y.tab.h
YACC=yacc -d -Wall -v
FLEX= flex -d

all: mrproper
	$(FLEX) $(prefixe).l
	$(YACC) $(prefixe).y
	gcc $(arg) $(generated) $(files) ast.c ast.h -o $(target) $(link)
	
clean:
	rm -f *.o $(generated)

test: all $(prefixe)
	cat $(input) | ./$(target)

valgrind: all $(prefixe)
	valgrind ./$(target)

valgrind-test: all $(prefixe)
	cat $(input) | valgrind ./$(target)

mrproper: clean
	rm -f $(target)

remake: mrproper all
