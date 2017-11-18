%{
	#include <stdio.h>
	void trim_fprintf(FILE *output, char *s);
%}
OPERATOR "-"|"+"|"*"|"/"|"++"|"--"|"<"|">"|"<="|">="|"=="|"->"|"&"{1,2}|"||"|"$"
BLANKS [[:blank:]]+
TYPE "int"|"void"|"stencil"
ARRAY "{".*"}"
STENCIL {ID}{ARRAY}
INT [0-9]+
CONDITION "if"|"else"
LOOP "while"|"for"|"do"
ID [[:alpha:]_][[:alnum:]_]*("[".*"]")*
COMMENT "/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/"
INLINE_COMMENT "//".*
IGNORE [\v\r\f]
INDENT [\n\t ]
%%
{INDENT} { fprintf(yyout, "%s", yytext); }
";" { fprintf(yyout, "SEMICOLON "); }
"=" { fprintf(yyout, "ASSIGN "); }
"{" { fprintf(yyout, "O_BRACKET "); }
"}" { fprintf(yyout, "C_BRACKET "); }
"(" { fprintf(yyout, "O_PAR "); }
")" { fprintf(yyout, "C_PAR "); }
{INLINE_COMMENT}$ { fprintf(yyout, "INLINE_COMMENT(%s)", yytext); }
{OPERATOR} { fprintf(yyout, "OPERATOR(%s) ", yytext); }
{INT} { fprintf(yyout, "INT(%s) ", yytext); }
{CONDITION} { fprintf(yyout, "CONDITION(%s) ", yytext); }
{LOOP} { fprintf(yyout, "LOOP(%s) ", yytext); }
{ARRAY} { fprintf(yyout, "ARRAY(%s) ", yytext); }
{TYPE}{BLANKS} { /* avoid trailing space in output */fprintf(yyout,"TYPE("); trim_fprintf(yyout, yytext); fprintf(yyout,") "); }
"main"{BLANKS}?"("{BLANKS}?")"{BLANKS}? { fprintf(yyout, "MAIN() "); }
{ID} { fprintf(yyout, "ID(%s) ", yytext); }
{COMMENT} { fprintf(yyout, "COMMENT(%s) ", yytext); }
{IGNORE} {/* Ignore if void */}
. { fprintf(yyout, "UNKNOWN(%s) ", yytext); }
%%
int main(int argc, char** argv)
{
	FILE *in, *out;
	// if file given in first arg exists -> read it
	if(argc > 1 && (in = fopen(argv[1], "r")))
	{
		yyin = in;
	}
	// if file given in second arg exists or can be created -> write output to it
	if(argc > 2 && (out = fopen(argv[2], "w+")))
	{
		yyout = out;
	}
	else
	{
		// write output in stdout by default
		yyout = stdout;
	}

	yylex();

	// end with \n for convenience
	fprintf(yyout, "\n");

	// close opened files
	if(yyin != stdin)
	{
		fclose(yyin);
	}
	if(yyout != stdout)
	{
		fclose(yyout);
	}

	return 0;
}

void trim_fprintf(FILE *output, char *s)
{
	int i=0;
	char c;
	while((c = s[i]) != '\0')
	{
		if(c != ' ')
			fprintf(output, "%c", c);
		i++;
	}
}