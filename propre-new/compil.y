%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "ast.h"

	//pour éviter un warning avec yylex
	#if YYBISON
		union YYSTYPE;
		int yylex(void);
	#endif

	void yyerror(char *s);
	void parsing_ok();
	void yylex_destroy(void);

	void parsing_ok(ast* ast);

	extern char* yytext;
	extern int yylineno;

	extern FILE *yyin, *yyout;
%}

%union {
	int value;
	char* string;
	struct ast* ast; 
}

%type <ast> fonction
%type <ast> action
%type <ast> instruction
%type <ast> expression
%type <ast> condition
%type <ast> comparaison
%type <ast> retour
%type <ast> declaration
%type <ast> boucle
%type <ast> affectation
%type <ast> incrementation
%type <ast> valeur
%type <ast> operation

%token <string> ID
%token TYPE
%token MAIN
%token RETURN
%token <value> ENTIER
%token IF
%token ELSE
%token WHILE
%token DO
%token FOR
%token COMMENTAIRE
%token INCR
%token DECR
%token EGAL
%token INEGAL
%token SUPEG
%token INFEG

%right '='
%left '+' '-'
%left '*' '/'

%%

axiom:
	 	fonction 
		{
			parsing_ok($1);
			return 0;
		}
	|	condition
		{
			parsing_ok($1);
			return 0;
		}
	|	%empty
		{
			printf("Match !\n");
			return 0;
		}
	;

fonction:
	 	TYPE MAIN '(' ')' '{'action retour'}'
		{
			$$ = ast_new_fonction("main", $6, $7);
		}
	|	TYPE MAIN '(' ')' '{' retour '}'
		{
			$$ = ast_new_fonction("main", NULL, $6);
		}
	;


retour:
	 	RETURN ENTIER';'
		{
			$$ = ast_new_nombre($2);
		}
	;

action:
	 	instruction
		{
			$$ = $1;
		}
	|	instruction action
		{
			$$ = ast_new_action($1,$2);
		}
	;

instruction:
		declaration	';'			{$$ = $1;}
	| 	operation	';'			{$$ = $1;}
	|	condition				{$$ = $1;}
	|	boucle					{$$ = $1;}
	|	affectation	';'			{$$ = $1;}
	|	incrementation	';'		{$$ = $1;}
	;

declaration:
		TYPE ID
		{
			$$ = ast_new_id($2);
		}
	|	TYPE affectation
		{
			$$ = $2;
		}
	;

affectation:
		ID '=' valeur
		{
			$$ = ast_new_affectation($1,$3);
		}
	|	ID '=' operation
		{
			$$ = ast_new_affectation($1,$3);
		}
	;

valeur:
		ENTIER
		{
			$$ = ast_new_nombre($1);
		}
	|	ID
		{
			$$ = ast_new_id($1);
		}
	;

expression:
		valeur
		{
			$$ = $1;
		}
	|	operation
		{
			$$ = $1;
		}
	;

operation:
		expression '+' expression
		{
			$$ = ast_new_binop("+",$1,$3);
		}
	|	expression '-' expression
		{
			$$ = ast_new_binop("-",$1,$3);
		}	
	|	expression '*' expression
		{
			$$ = ast_new_binop("*",$1,$3);
		}
	|	expression '/' expression
		{
			$$ = ast_new_binop("/",$1,$3);
		}	
	|	'('expression')'
		{
			$$ = $2;
		}		
	;

condition:
		IF'('comparaison')' '{'action'}'
		{
			$$ = ast_new_if($3,$6,NULL);
		}
	|	IF'('comparaison')' '{'action'}' ELSE '{'action'}'
		{
			$$ = ast_new_if($3,$6,$10);
		}
	;

boucle:
		WHILE'('comparaison')' '{'action'}'
		{
			//j'ai la flemme
		}
	|	DO '{'action'}' WHILE '('comparaison')'';'
		{
			//j'ai la flemme
		}
	|	FOR'('TYPE affectation';' comparaison';' incrementation')' '{'action'}'
		{
			$$ = ast_new_for($4,$6,$8,$11);
		}
	|  	FOR'('TYPE affectation';' comparaison';' operation')' '{'action'}'
		{
			$$ = ast_new_for($4,$6,$8,$11);
		}
	;

incrementation:
		ID INCR
		{
			$$ = ast_new_unop("++",ast_new_id($1));
		}
	|	ID DECR
		{
			$$ = ast_new_unop("--",ast_new_id($1));
		}
	;

comparaison:
		expression EGAL expression
		{
			$$ = ast_new_binop("==",$1,$3);
		}
	|	expression INEGAL expression
		{
			$$ = ast_new_binop("!=",$1,$3);
		}
	|	expression INFEG expression
		{
			$$ = ast_new_binop("<=",$1,$3);
		}
	|	expression SUPEG expression
		{
			$$ = ast_new_binop(">=",$1,$3);
		}
	|	expression '>' expression
		{
			$$ = ast_new_binop(">",$1,$3);
		}
	|	expression '<' expression
		{
			$$ = ast_new_binop("<",$1,$3);
		}
	;

%%

void yyerror(char* c) {
	fprintf(stderr, "%s on %s\n", c, yytext); 
	return;
}

void parsing_ok(ast* ast)
{
	printf("Match!\n");

	printf("\n===== AST =====\n\n");
	ast_print(ast, 0);
	printf("\n===== SYMBOLS =====\n\n");

	printf("\n===== QUADS =====\n\n");

	ast_free(ast);
}

int main(int argc, char* argv[]) {
	FILE *in, *out;
	if(argc <= 1)
	{
		printf("Entrez une expression :\n");
	}
	
	if(argc >= 2)
	{
		in = fopen(argv[1], "r");
		if(in != NULL)
		{
			if(yyin != stdin && yyin != NULL)
			{
				fclose(yyin);
			}
			yyin = in;
		}
		else
		{
			fprintf(stderr,"Error reading file '%s'", argv[1]);
			return EXIT_FAILURE;
		}
	}
	
	if(argc >= 3)
	{
		out = fopen(argv[2], "w+");
		if(out != NULL)
		{
			if(yyout != stdout && yyout != NULL)
			{
				fclose(yyout);
			}
			yyout = out;
		}
		else
		{
			fprintf(stderr,"Error writing int file '%s'", argv[2]);
			return EXIT_FAILURE;
		}
	}


	int code = yyparse();
	yylex_destroy();

	if(in != NULL && in != stdin)
	{
		printf("closing in\n");
		fclose(in);
	}

	if(out != NULL && out != stdout)
	{
		printf("closing out\n");
		fclose(out);
	}

	return code;
}
