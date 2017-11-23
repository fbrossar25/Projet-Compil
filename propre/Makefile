prefixe=compil
files=
target=compil
input=input.txt
link=-ly -ll
arg=-g -std=c99
generated=y.tab.c lex.yy.c y.tab.h

all: mrproper
	yacc -d $(prefixe).y
	lex $(prefixe).l
	gcc $(arg) $(generated) $(files) ast.c ast.h -o $(target) $(link)
	
clean:
	rm -f *.o $(generated)

test: all $(prefixe)
	cat $(input) | ./$(target)

mrproper: clean
	rm -f $(target)

remake: mrproper all
