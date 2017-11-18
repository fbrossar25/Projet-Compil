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
%type <ast> expr

//%left '|'
//%left '&'
%left '+' '-'
%left '*' '/' '%'
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
  ;

expr:
    expr '+' expr           { $$ = ast_new_operation("+", $1, $3); }
  | expr '-' expr           { $$ = ast_new_operation("-", $1, $3); }
  | '(' expr ')'            { $$ = $2; }
  | '-' expr %prec UMINUS   { $$ = ast_new_operation("-", $2, NULL); }
  | ID                      { $$ = ast_new_id($1); }
  | NUMBER                  { $$ = ast_new_number($1); }
  | TYPE DECLARATION_LIST   { $$ = ast_new_declaration($1, $2); }
  | DECLARATION_LIST
  ;
%%

int main() {
  printf("Entrez une expression :\n");
  yyparse();
  return 0;
}
