%option yylineno

%{
	#include <stdlib.h>
  	#include <string.h>
  	#include "y.tab.h"
	
	void dup_alloc_list_add(char* dup);

	char* extract_string_content(const char* string);
%}
%%

\n 									{ }
"//".*								{ /* return COMMENTAIRE; */ }
"\"".*"\""							{ 
										//char* dup = extract_string_content(yytext);
										char* dup = strdup(yytext);
										yylval.string = dup; 
										dup_alloc_list_add(dup);
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
										yylval.string = dup; 
										dup_alloc_list_add(dup);
										return ID;
									}
[0-9]+ 								{ yylval.value = atoi(yytext); return ENTIER; }
"/*"([^*]*[*]+[*]*[^*/])*[^*]*[*]+"/" { /*return COMMENTAIRE ;*/ }
[ \t\v\r\f] 						{/* Ignore */}
. 									{ printf("Car. inconnu: %s\n", yytext); }

%%

char* extract_string_content(const char* string)
{
	size_t size = strlen(string);
	// size - 2 + 1
	// - 2 car on enlève les ", +1 pour avoir \0 à la fin
	size_t content_size = size - 2;
	char* content = calloc(content_size + 1, sizeof(char));
	for(size_t i=0; i<content_size; i++)
	{
		content[i] = string[i + 1];
	}
	content[content_size] = '\0';
	return content;
}