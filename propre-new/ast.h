#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib.h"
#include "symbol.h"
#include "error.h"

//pour le token STRING on réutilise le champ id (type IDENTIFIER)
typedef enum ast_type {
	BIN_OP, UN_OP, FOR_STMT,
	IF_STMT, FCT, AFFECT, IDENTIFIER,
	INT, ACTION, WHILE_STMT, UNDEFINED, CALL
} ast_type;

typedef struct ast {
    ast_type type;
    union{
		struct { //opérateur unaire ( le - unaire, goto)
			char* op;
			struct ast* fils;
		}unop;
		struct { //opérateur binaire
			char* op;
			struct ast* left;
			struct ast* right;
		}binop;
		struct {
			//char* type; //type de retour
			char* name; //nom de la fonction
			struct ast* action; //block d'instruction
			struct ast* retour; //instruction de retour
		}fct;
		struct{
			struct ast* condition; //condition
			struct ast* action; //block if
			struct ast* els; //block else
		}ifstmt;
		struct {
			struct ast* min; //int i = 0;
			struct ast* max; //i < x;
			struct ast* incr; //i++
			struct ast* action; //block for
		}forstmt;
		struct{
			struct ast* id; // id = expr
			struct ast* expr;
		}affect;
		struct{
			struct ast* instruction; //instruction en cours
			struct ast* action; //instruction suivante
		}action;
		struct{
			char* name; //nom de la fonction
			struct ast* arg; //argument de la fonction
		}call;
		char* id; //identifieur
		int nombre; //entier
    }u;
} ast;

typedef struct ast_list
{
	ast* node;
	struct ast_list* next;
} ast_list;

ast* ast_alloc();
ast* ast_new_id(char* id);
ast* ast_new_nombre(int nombre);
ast* ast_new_affectation(char* id, ast* expr);
ast* ast_new_fonction(char* name, ast* action, ast* retour);
ast* ast_new_if(ast* condition, ast* action, ast* els);
ast* ast_new_for(ast* min, ast* max, ast* incr, ast* action);
ast* ast_new_unop(char* op, ast* fils);
ast* ast_new_binop(char* op, ast* left, ast* right);
ast* ast_new_action(ast* instruction, ast* action);
ast* ast_new_call(char* name, ast* arg);
struct symbol* astGencode(ast* src, struct symtable* t, struct code* c);
void ast_print(ast* ast, int tab);
void ast_free(ast* ast);
void ast_free_ast_alloc();
void ast_free_ast_alloc_list();

#endif