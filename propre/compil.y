%{
   #include <string.h>
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

   extern char* yytext;
   extern int yylineno;
%}

%union {
  char* string;
  int entier;
  struct ast* ast ;
}

%token END
%token OP_EGAL OP_DIFF OP_SUPEG OP_INFEG
%token BLANCS
%token <string> ID
%token <entier> ENTIER
%token TYPE
%token FOR WHILE DO
%token IF ELSE
%token TRUE FALSE 
%token OR AND
%token NOT

%type <ast> instruction
%type <ast> instructions
%type <ast> expr
%type <ast> fonction
%type <ast> bloc
%type <ast> declaration
%type <ast> assignation

//les précédences inutiles (fonctionne de la même manière avec ou sans) sont commentées
//%right '='
%left '+' '-'
%left '*' '/'
//%left OP_EGAL OP_DIFF OP_SUPEG OP_INFEG '<' '>'
//%left OR AND
//%right NOT
//%left '!'
%precedence UMOINS

%%

axiom:
		fonction
		{
			parsing_ok($1);
		}
	|	instruction
		{
			parsing_ok($1);
		}
	;


fonction:
		TYPE ID arguments bloc
		{
			$$ = ast_new_fonction($2, $4);
		}
	;

arguments:
		'(' ')'
		{

		}
	;

bloc:
		'{' '}'
		{
			$$ = NULL;
		}
	|	'{'	instructions '}'
		{
			$$ = $2;
		}
	;

instructions:
		instruction
		{
			$$ = $1;
		}
	|	instruction instructions
		{
			$$ = $1;
			$$->nextInstr = $2;
		}
	;

instruction:
		declaration ';'
		{
			$$ = $1;
		}
	|	expr ';'
		{
			$$ = $1;
		}
	|	appel ';'
		{
			// à faire (nouveau type de noeud appel ?)
		}
	;

appel:
		ID arguments ';'
		{
			// à faire
		}
	;

declaration:
		TYPE ID
		{
			$$ = ast_new_id($2, NULL);
		}
	|	TYPE assignation
		{
			$$ = $2;
		}
	;

assignation:
		ID '=' expr
		{
			$$ = ast_new_id($1, $3);
		}
	;

expr:
		expr '+' expr
		{
			$$ = ast_new_operation("+",$1,$3);
		}
	|	expr '-' expr
		{
			$$ = ast_new_operation("-",$1,$3);
		}
	|	expr '*' expr
		{
			$$ = ast_new_operation("*",$1,$3);
		}
	|	expr '/' expr
		{
			$$ = ast_new_operation("/",$1,$3);
		}
	|	'(' expr ')'
		{
			$$ = $2;
		}
	|	'-' expr %prec UMOINS
		{
			$$ = ast_new_operation("-",NULL,$2);
		}
	|	ENTIER
		{
			$$ = ast_new_entier($1);
		}
	|	ID
		{
			$$ = ast_new_id($1, NULL);
		}
	;
	
%%

void parsing_ok(ast* src)
{
	printf("Parsing OK !\n");
	printf("%d lignes reconnues !\n", yylineno);
	printf("========== AST ==========\n");
	if(src == NULL)
	{
		printf("Aucun AST n'as été produit\n");
	}
	ast_print(src,0);
	ast_destroy(src); // à corriger
	printf("=========================\n");
	exit(EXIT_SUCCESS);
}

void yyerror(char *s)
{
	fprintf(stderr, "ligne %d : %s avec \"%s\"\n", yylineno, s, yytext);
}

int main()
{
  printf("Entrez une expression :\n");
  return yyparse();
}
