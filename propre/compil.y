%{
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>

   #include "ast.h"
  

%}

%union {

  char* string;
  int entier;
  struct ast* ast ;
}

%token END
%token OP_EGAL
%token OP_DIFF
%token OP_SUPEG
%token OP_INFEG
%token BLANCS
%token <string> ID
%token <entier> ENTIER
%token TYPE
%token FOR
%token IF
%token ELSE
%token WHILE
%token DO
%token TRUE
%token FALSE 
%token OR
%token AND 
%token NOT 


%type <ast> expr 
%type <ast> declaration




%left OR
%left AND
%left '+' '-'
%left '*' '/'
%precedence UMINUS

%%

axiom:
	fonction
	{ 
		printf("fonction reconnue !\n");
		exit(0);
	}
	| instruction
	{ 
		printf("instruction reconnue !\n");
		exit(0);
	}
	|expr '\n' 
	{ 
			printf("Chaine reconnue !\n");
			ast_print($1, 0);
			exit(0);
	}
	| expr    
	{ 
			printf("Chaine reconnue !\n");
			ast_print($1, 0);
			exit(0);
	}
    ;

fonction:
        TYPE  ID '(' ')' '{' instruction '}'
        {
            printf("fonction\n");
        }
    |   TYPE BLANCS ID '(' ')' '{' '}' '\n'
        {
            printf("fonction\n");
        }
    ;

instruction:
        expr ';'
    |   declaration ';' 	
    ;

declaration:
        TYPE ID 			  { $$ = ast_new_operation("TYPE", NULL ,ast_new_id($2));}
    ;

expr:
	expr '+' expr             { $$ = ast_new_operation("+", $1, $3); }
	| expr '-' expr           { $$ = ast_new_operation("-", $1, $3); }
	| expr '/' expr           { $$ = ast_new_operation("/", $1, $3); }
	| expr '*' expr           { $$ = ast_new_operation("*", $1, $3); }
	| expr '%' expr           { $$ = ast_new_operation("%", $1, $3); }
	| expr OR expr            { $$ = ast_new_operation("||", $1, $3); }
	| expr AND expr           { $$ = ast_new_operation("&&", $1, $3); }
	| '!' expr          	  { $$ = ast_new_operation("!", $2, NULL); }
	| '(' expr ')'            { $$ = $2; }
	| '-' expr %prec UMINUS   { $$ = ast_new_operation("-", $2, NULL); }
	| ID                      { $$ = ast_new_id($1); }
	| ENTIER                  { $$ = ast_new_number($1); }
	;
	
%%


int main() {
  printf("Entrez une expression :\n");
  return yyparse();
}
