#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ast {
    char* type;
    union {
        struct {
            struct ast* left;
            struct ast* right;
        } operation;
        struct {
            struct ast** leafs;
        } leafs;
        int number;
        char* id;
    } u;
} ast;

ast* ast_new_operation(char*, ast*, ast*);
ast* ast_new_declaration(char*, ast*);
ast* ast_new_number(int);
ast* ast_new_id(char*);
void ast_print(ast*, int);
int yylex();
void yyerror(char*);

#endif