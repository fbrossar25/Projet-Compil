%option yylineno

%{
	#include <stdlib.h>
  	#include <string.h>

	#include "alloc.h"
  	#include "y.tab.h"
	
	void dup_alloc_list_add(char* dup);
%}
%%

\n 									{ }
"//".*								{ /* return COMMENTAIRE; */ }
"\"".*"\""							{ 
										char* dup = strdup(yytext);
										yylval.string = dup; 
										add_alloc(dup);
										return STRING;
									}
"int"								{ return TYPE; }
"main"								{ return MAIN; }
"printf"							{ return PRINTF; }
"printi"							{ return PRINTI; }
"return" 							{ return RETURN; }
"if" 								{ return IF; }
"else" 								{ return ELSE; }
"while"	 							{ return WHILE; }
"for"								{ return FOR; }
"do"								{ return DO; }
"++"								{ return INCR; }
"--"								{ return DECR; }
"+="								{ return SHORT_INCR; }
"-="								{ return SHORT_DECR; }
"=="								{ return EGAL; }
"!="								{ return INEGAL; }
">="								{ return SUPEG; }
"<="								{ return INFEG; }
"true"								{ return TRUE; }
"false"								{ return FALSE; }
[()]         						{ return yytext[0]; }
[{}]         						{ return yytext[0]; }
[[]]         						{ return yytext[0]; }
";"									{ return yytext[0]; }
"-"									{ return yytext[0]; }
"+"									{ return yytext[0]; }
"/"									{ return yytext[0]; }
"*"									{ return yytext[0]; }
">"									{ return yytext[0]; }
"<"									{ return yytext[0]; }
"="									{ return yytext[0]; }
[a-zA-Z_][0-9a-zA-Z_]* 			 	{
										char* dup = strdup(yytext);
										strcat(dup, "_var");//pour éviter d'éventuels problèmes avec les mot clé mips ou les temporaires
										add_alloc(dup);
										yylval.string = dup; 
										return ID;
									}
[0-9]+ 								{ yylval.value = atoi(yytext); return ENTIER; }
"/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/" { /*return COMMENTAIRE ;*/ }
[ \t\v\r\f] 						{/* Ignore */}
. 									{ printf("Car. inconnu: %s\n", yytext); }

%%
