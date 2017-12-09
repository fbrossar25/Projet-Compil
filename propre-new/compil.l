%option yylineno

%{
	#include <stdlib.h>
  	#include <string.h>
  	#include "y.tab.h"
	
	void dup_alloc_list_add(char* dup);
%}
%%

\n 									{  }
"//".*								{ }
"int"								{ return TYPE; }
"main"								{ return MAIN;}
"return" 							{ return RETURN;}
"if" 								{ return IF; }
"else" 								{ return ELSE; }
"while"	 							{ return WHILE; }
"for"								{ return FOR; }
"do"								{ return DO; }
"++"								{ return INCR; }
"--"								{ return DECR; }
"=="								{ return EGAL; }
"!="								{ return INEGAL; }
">="								{ return SUPEG; }
"<="								{ return INFEG; }
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
										yylval.string = dup; 
										dup_alloc_list_add(dup);
										return ID;
									}
[0-9]+ 								{ yylval.value = atoi(yytext); return ENTIER; }
"/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/" { /*return COMMENTAIRE ;*/ }
[ \t\v\r\f] 						{/* Ignore */}
. 									{ printf("Car. inconnu: %s\n", yytext); }

%%
