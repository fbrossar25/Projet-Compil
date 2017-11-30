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
   void yylex_destroy(void);

   extern char* yytext;
   extern int yylineno;

   extern FILE *yyin, *yyout;
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
%token TYPE RETURN MAIN
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
%type <ast> retour

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
		TYPE MAIN arguments bloc
		{
			$$ = ast_new_fonction("main", $4);
		}
	;

arguments:
		'(' ')'
		{

		}
	;

bloc:
		'{' retour ';' '}'
		{
			$$ = $2;
		}
	|	'{'	instructions retour ';' '}'
		{
			$$ = $2;
			$$->nextInstr = $3;
		}
	;

retour:
		RETURN ENTIER
		{
			$$ = ast_new_retour($2);
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
		assignation ';'
		{
			$$ = $1;
		}
	|	declaration ';'
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
/*<<<<<<< Updated upstream*/
	//int val = ast_eval(src);
	//printf("valeur de l'expression : %d !\n", val);
	printf("========== TABLE =========\n");
/*=======*/
	int val = ast_eval(src);
	printf("valeur de l'expression : %d !\n", val);
/*>>>>>>> Stashed changes*/
	struct symtable* t = symtable_new();
	struct code * c = code_new();
	astGencode(src,t,c);
	code_dump(c);
/*<<<<<<< Updated upstream*/
	printf("=========================\n");
	
	//ici on désalloue toutes allocations
/*=======
>>>>>>> Stashed changes*/
	ast_destroy(src);
	code_free(c);
	symtable_free(t);
	destroy_ast_list(); //résoud le problème d'un double free
	yylex_destroy();
	//sans allocation de notre part, il ne devrai y avoir que
	//4o definitely lost en 2 blocks
	//et 5 octets still reachable en 1 blocks
	exit(EXIT_SUCCESS);
}

void yyerror(char *s)
{
	fprintf(stderr, "ligne %d : %s avec \"%s\"\n", yylineno, s, yytext);
}

int main(int argc, char* argv[])
{
	FILE *in, *out;
	if(argc <= 1)
	{
		printf("Entrez une expression :\n");
	}
	
	if(argc >= 2)
	{
		if((in = fopen(argv[1], "r")) != NULL)
		{
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
		if((out = fopen(argv[2], "w+")) != NULL)
		{
			yyout = out;
		}
		else
		{
			fprintf(stderr,"Error writing int file '%s'", argv[2]);
			return EXIT_FAILURE;
		}
	}

	int code = yyparse();

	if(yyin != stdin)
	{
		fclose(yyin);
	}

	if(yyout != stdout)
	{
		fclose(yyout);
	}

	return code;
}
