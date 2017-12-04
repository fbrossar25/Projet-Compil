#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct ast {
    enum {
			BIN_OP, UN_OP, FOR_STMT,
			IF_STMT, FCT, AFFECT, IDENTIFIER,
			INT, ACTION, WHILE_STMT
	} type;
    union{
		struct {
			char* op;
			struct ast* fils;
		}unop;
		struct {
			char* op;
			struct ast* left;
			struct ast* right;
		}binop;
		struct {
			char* name;
			struct ast* action;
			struct ast* retour;
		}fct;
		struct{
			struct ast* condition;
			struct ast* action;
			struct ast* els;
		}ifstmt;
		struct {
			struct ast* min;
			struct ast* max;
			struct ast* incr;
			struct ast* action;
		}forstmt;
		struct{
			struct ast* expr;
			struct ast* id;
		}affect;
		struct{
			struct ast* instruction;
			struct ast* action;
		}action;
		char* id;
		int nombre;
    }u;
} ast;

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
struct symbol*  astGencode(ast* src,struct symtable* t, struct code* c);
void ast_print(ast* ast, int tab);
void ast_free(ast* ast);

#endif