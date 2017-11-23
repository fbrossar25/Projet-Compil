%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  int yylex ();
  void yyerror(char const *s);

%}

%union {
  char* string;
  int entier;
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
%token TAND 
%token NOT 





%right '='
%left '+' '-'
%left '*' '/'
%left "<=" ">=" '<' '>' "==" "!="
%left OR
%left AND
%right NOT
%left UMOINS '!'

%%

axiom:
        fonction
        { 
            printf("fonction reconnue !\n");
	        exit(0);
        }
    |   instruction
        { 
            printf("instruction reconnue !\n");
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
        TYPE BLANCS ID
    ;

expr:
        expr '+' expr
        {
            printf("OPERATEUR(+)\n");
        }
    |   ENTIER
        {
            printf("ENTIER(%d)\n",$1);
        }
    |   ID
        {
            printf("ID(%s)\n",$1);
        }
    ;
	
%%

int main() {
  printf("Entrez une expression :\n");
  yyparse();
  return 0;
}
