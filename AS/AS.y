%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "ast.h"
%}

%union {
  char* string;
  int value;
  struct ast* ast;
}

%token <string> ID
%token <string> OPERATOR
%token <value> NUMBER
%token <string> TYPE 
%token <string> STENCIL

%type <ast> expr 
%type <ast> declaration 
%type <ast> affectation 

//%left '|'
//%left '&'

%left '+' '-'
%left '*' '/' '%'
%left '='
%left UMINUS

%%

axiom:
    expr '\n' { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }
  | expr      { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }
   declaration '\n' { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }
  | declaration      { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }
    affectation '\n' { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }
  | affectation      { 
                printf("Chaine reconnue !\n");
                ast_print($1, 0);
	            exit(0);
              }

  ;

expr:
    expr '+' expr           { $$ = ast_new_operation("+", $1, $3); }
  | expr '-' expr           { $$ = ast_new_operation("-", $1, $3); }
  | expr '/' expr           { $$ = ast_new_operation("/", $1, $3); }
  | expr '*' expr           { $$ = ast_new_operation("*", $1, $3); }
  | expr '%' expr           { $$ = ast_new_operation("%", $1, $3); }
  | '(' expr ')'            { $$ = $2; }
  | '-' expr %prec UMINUS   { $$ = ast_new_operation("-", $2, NULL); }
  | ID                      { $$ = ast_new_id($1); }
  | NUMBER                  { $$ = ast_new_number($1); }
	;
declaration :
	TYPE ID 				{ $$ = ast_new_operation("TYPE", NULL ,ast_new_id($2));}
	|STENCIL ID				{ $$ = ast_new_operation("STENCIL", NULL ,ast_new_id($2));}
	//je ne comprends pas prk je n'ai pas le meme resultat que type 
	;
affectation :
	declaration '=' expr    { $$ = ast_new_operation("=",$1,$3);}
	|ID '=' expr 			{ $$ = ast_new_operation("=",ast_new_id($1),$3);}
	;
	
%%

int main() {
  printf("Entrez une expression :\n");
  yyparse();
  return 0;
}
