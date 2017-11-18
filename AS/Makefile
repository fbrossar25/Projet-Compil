prefixe=AS
target=AS
input=input.txt

all: $(prefixe).y $(prefixe).l
	yacc -d $(prefixe).y
	lex $(prefixe).l
	gcc y.tab.c lex.yy.c ast.c ast.h -o $(target) -ly -ll

clean:
	rm -f *.o y.tab.c y.tab.h lex.yy.c

test: $(prefixe)
	cat $(input) | ./$(target)

mrproper: clean
	rm -f $(target)