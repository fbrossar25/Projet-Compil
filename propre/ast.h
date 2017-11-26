#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//type : INT | FCT | ID | operateur (+,-,=, etc...)

typedef struct ast {
    char* type;
    union {
        struct {
            struct ast* left;
            struct ast* right;
        } op;
        struct {
            struct ast* block;
            char * id;
        } fct;
        struct {
            struct ast* expr;
            char* id;
        } affect;
        int number;
    } u;
    struct ast* nextInstr;
} ast;

ast* ast_alloc();
ast* ast_new_operation(char* op, ast* left, ast* right);
ast* ast_new_fonction(char* fct, ast* bloc);
ast* ast_new_id(char* id, ast* expr);
ast* ast_new_entier(int entier);
void ast_print(ast* src, int indent);

void ast_free_node(ast* node);
void ast_destroy(ast* root);
int  ast_eval(ast* src);

#endif
