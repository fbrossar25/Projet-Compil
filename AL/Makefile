all: remake

remake: mrproper AL

AL:
	flex AL.lex
	gcc -g -o AL lex.yy.c -lfl -std=c99 -D_POSIX_C_SOURCE

clean:
	rm -f lex.yy.c

mrproper: clean
	rm -f AL

test: AL
	./AL input.txt