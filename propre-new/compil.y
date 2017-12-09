%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "ast.h"
	#include "lib.h"
	#include "error.h"

	//pour éviter un warning avec yylex
	#if YYBISON
		union YYSTYPE;
		int yylex(void);
	#endif

	void yyerror(char *s);
	void parsing_ok();
	void yylex_destroy(void);

	void parsing_ok();
	void cleanup();

	extern char* yytext;
	extern int yylineno;

	extern FILE *yyin, *yyout;

	symtable* t = NULL;
	code* c = NULL;
	ast* root = NULL;

	typedef struct dup_list
	{
		char* dup;
		struct dup_list* next;
	} dup_list;

	static dup_list* dup_alloc_list = NULL;

	void dup_alloc_list_add(char* dup);
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

//%right '=' //préséance inutile d'après yacc
%left '+' '-'
%left '*' '/'
%precedence UMOINS

%%

axiom:
	 	fonction 
		{
			root = $1;
			if(get_error_count() == 0)
			{
				parsing_ok();
			}
			cleanup();
			return 0;
		}
	|	action
		{
			root = $1;
			if(get_error_count() == 0)
			{
				parsing_ok();
			}
			cleanup();
			return 0;
		}
	|	%empty
		{
			printf("Match ! (fichier vide)\n");
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
			symtable_put(t, $2);
			$$ = ast_new_id($2);
		}
	|	TYPE affectation
		{
			ast* ast_id = $2->u.affect.id; //noeud id venant d'un noeud affect
			symtable_put(t, ast_id->u.id);
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
	|	'-' expression %prec UMOINS
		{
			$$ = ast_new_unop("-", $2);
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
			//TODO
		}
	|	DO '{'action'}' WHILE '('comparaison')'';'
		{
			//TODO
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

dup_list* dup_alloc_list_new()
{
	dup_list* new = calloc(1,sizeof(dup_list));
	new->dup = NULL;
	new->next = NULL;
	return new;
}

//En cas d'erreur il se peut que les ID récupèrer doivent être libèrés
//On doit donc systématiquement les lister
void dup_alloc_list_add(char* dup)
{
	if(dup_alloc_list == NULL)
	{
		dup_alloc_list = dup_alloc_list_new();
		dup_alloc_list->dup = dup;
		return;
	}

	dup_list* scan = dup_alloc_list;
	dup_list* new = dup_alloc_list_new();
	new->dup = dup;
	while(scan->next != NULL)
	{
		scan = scan->next;
	}
	scan->next = new;
}

void dup_alloc_list_free()
{
	dup_list* scan = dup_alloc_list;
	dup_list* tmp = scan;
	while(scan != NULL)
	{
		scan = scan->next;
		if(tmp->dup != NULL)
		{
			free(tmp->dup);
		}
		free(tmp);
		tmp = scan;
	}
}

//libère toutes les allocation sauf celles de yacc
void cleanup()
{
	ast_free(root);
	if(get_error_count() != 0)
	{
		//en cas d'erreur les noeuds de l'ast sont libérés ici
		ast_free_ast_alloc();
		dup_alloc_list_free();
	}
	ast_free_ast_alloc_list();
	symtable_free(t);
	code_free(c);
}

//Message d'erreur perso si erreur de syntaxe détectée par yacc
void yyerror(char* c) {
	incr_error_count();
	fprintf(stderr, "yacc erreur %d : %s on %s (ligne %d)\n", get_error_count(), c, yytext, yylineno); 
	cleanup();
}

//Si on a un match alors on appel cette fonction
//Affiche l'ast, la table des symboles et les quads
void parsing_ok()
{
	printf("Match!\n");

	//Affichage
	printf("\n===== AST =====\n\n");
	ast_print(root, 0);
	printf("\n===== SYMBOLS =====\n\n");
	astGencode(root,t,c);
	symtable_dump(t);
	printf("\n===== QUADS =====\n\n");
	code_dump(c);
}

int main(int argc, char* argv[]) {
	//Fichiers d'e/s
	FILE *in = NULL, *out = NULL;

	//On alloue un espace mémoire pour la table des symboles t
	t = symtable_new();
	//et la liste de quad c
	c = code_new();

	//si aucun fichier d'entrée on lit stdin
	if(argc <= 1)
	{
		printf("Entrez une expression :\n");
	}
	
	//si un fichier d'entrée
	if(argc >= 2)
	{
		in = fopen(argv[1], "r");
		if(in != NULL) //si fichier d'entrée ouvert
		{
			if(yyin != stdin && yyin != NULL)
			{
				fclose(yyin);
			}
			yyin = in;
		}
		else
		{
			//fichier d'entrée inexistant ou impossible à ouvrir
			fprintf(stderr,"Error reading file '%s'", argv[1]);
			return EXIT_FAILURE;
		}
	}
	
	//si un fichier de sortie
	if(argc >= 3)
	{
		out = fopen(argv[2], "w+");
		if(out != NULL)  //si fichier de sortie ouvert
		{
			if(yyout != stdout && yyout != NULL)
			{
				fclose(yyout);
			}
			yyout = out;
		}
		else
		{
			//fichier de sortie inexistant ou impossible à ouvrir
			fprintf(stderr,"Error writing int file '%s'", argv[2]);
			return EXIT_FAILURE;
		}
	}


	int code = yyparse();

	//On libère la mémoire utilisé par lex et yacc
	yylex_destroy();

	//On prend soin de fermer les fichier potentiellement ouverts

	if(in != NULL && in != stdin) //attention à ne pas fermer stdin
	{
		fclose(in);
	}

	if(out != NULL && out != stdout) //attention à ne pas fermer stdout
	{
		fclose(out);
	}

	return code;
}
