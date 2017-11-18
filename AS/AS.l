%{
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"
%}

ID 		[[:alpha:]_][[:alnum:]_]*("[".*"]")*
INT 	[0-9]+
COMMENT "/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/"
OPERATOR "-"|"+"|"*"|"/"|"++"|"--"|"<"|">"|"<="|">="|"=="|"->"|"&"{1,2}|"||"|"$"
TYPE "int"|"void"|"stencil"

%%

{OPERATOR}   { return yytext[0]; }
{ID} 		     { yylval.string = strdup(yytext); return ID; }
{INT}     	 { yylval.value = atoi(yytext); return NUMBER; }
{TYPE}       { yylval.string = strdup(yytext); return TYPE; }
{COMMENT}	   { }
[()]      	 { return yytext[0]; }          
[\n]         { return yytext[0]; }
.            { printf("Caractere inconnu: %s", yytext); }

%%