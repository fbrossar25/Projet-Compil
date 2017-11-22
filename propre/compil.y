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

%token <string> ID
%token <string> OPERATEUR
%token <entier> ENTIER
%token <string> TYPE

%right '='
%left '+' '-'
%left '*' '/'
%left "<=" ">=" '<' '>' "==" "!="
%left "||"
%left "&&"
%left UMOINS '!'

%%

axiom:
        fonction '\n'
        { 
            printf("Chaine reconnue !\n");
	        exit(0);
        }
    |   fonction
        { 
            printf("Chaine reconnue !\n");
	        exit(0);
        }
    |   ID '\n'
        {
            printf("ID(%s)\n",$1);
            exit(0);
        }

fonction:
        TYPE ID '(' ')' '{' '}'
        {
            printf("fonction\n");
        }

	
%%

int main() {
  printf("Entrez une expression :\n");
  yyparse();
  return 0;
}