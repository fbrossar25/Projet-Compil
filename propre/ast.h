#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

//type : INT | FCT | ID | RETURN | operateur (+,-,=, etc...)

typedef struct ast_list{
    struct ast* node;
    struct ast_list* next;
} ast_list;

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
        struct {
            struct ast* retVal;
        } ret;
        struct {
            struct ast* instr;
            struct ast* next;
        } instr;
        int number;
    } u;
} ast;

ast* ast_alloc();
ast* ast_new_operation(char* op, ast* left, ast* right);
ast* ast_new_fonction(char* fct, ast* bloc);
ast* ast_new_id(char* id, ast* expr);
ast* ast_new_entier(int entier);
ast* ast_new_retour(int entier);
ast* ast_new_instr(ast* instr, ast* next);
void ast_print(ast* src, int indent);
struct symbol*  astGencode(ast* src,struct symtable* t, struct code* c);
void ast_free_node(ast* node);
void ast_destroy(ast* root);
int  ast_eval(ast* src);
void destroy_ast_list();
void add_to_ast_list(ast* new);

#endif
