%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "y.tab.h"

  char *strdup(const char *s);
  int fileno(FILE *stream);

%}

ID 		[a-zA-Z_][a-zA-Z0-9_]
ENTIER 	[0-9]+
COMMENTAIRE "/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/"
OPERATEUR "-"|"+"|"*"|"/"|"<"|">"|"<="|">="|"=="|"&&"|"||"
TYPE 	"int"

%%

{TYPE}          { yylval.string = strdup(yytext);   return TYPE; }
{OPERATEUR}     { yylval.string = strdup(yytext);   return OPERATEUR; }
{ID}		    { yylval.string = strdup(yytext);   return ID; }
{ENTIER}     	{ yylval.entier = atoi(yytext);     return ENTIER; }
{COMMENTAIRE}   { }
[()]      	    { return yytext[0]; }          
[\n]            { return yytext[0]; }
.               { printf("Caractere inconnu: %s\n", yytext); }

%%