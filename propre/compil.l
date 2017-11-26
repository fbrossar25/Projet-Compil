%option yylineno
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "y.tab.h"

  char *strdup(const char *s);
  int fileno(FILE *stream);

%}

ID 		[a-zA-Z_][a-zA-Z0-9_]*
ENTIER 	[0-9]+
COMMENTAIRE "/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/"
TYPE 	"int"



%%

<<EOF>>         { return END; }
{TYPE}          { return TYPE; }
"="             { return yytext[0]; }
"=="            { return OP_EGAL; }
"!="            { return OP_DIFF; }
">="            { return OP_SUPEG; }
"<="            { return OP_INFEG; }
"&&" 			      { return AND; }
"||" 			      { return OR; }
"!"				      { return NOT; }
"true" 		      { return TRUE; }
"false"         { return FALSE; }
"for"		        { return FOR ;}
"if"			      { return IF;}
"else" 			    {return ELSE;}
"do" 			      { return DO;}
"while" 		    { return WHILE;}
{ID}        	  { yylval.string = strdup(yytext);   return ID; }
{ENTIER}     	  { yylval.entier = atoi(yytext);     return ENTIER; }
{COMMENTAIRE}   { }
[\n]            { /*return yytext[0];*/ }
[\t ]+          { /*return BLANCS;*/ }
[(){};]      	  { return yytext[0]; } 
.               { printf("token non reconnu: %s\n", yytext); }





%%
