#include "ast.h"

void ast_free_binop(ast* ast);
void ast_free_unop(ast* ast);
void ast_free_for(ast* ast);
void ast_free_if(ast* ast);
void ast_free_fct(ast* ast);
void ast_free_affect(ast* ast);
void ast_free_identifier(ast* ast);
void ast_free_int(ast* ast);
void ast_free_action(ast* ast);
void ast_free_call(ast* ast);

void ast_print_binop(ast* ast, size_t tab);
void ast_print_unop(ast* ast, size_t tab);
void ast_print_for(ast* ast, size_t tab);
void ast_print_if(ast* ast, size_t tab);
void ast_print_fct(ast* ast, size_t tab);
void ast_print_affect(ast* ast, size_t tab);
void ast_print_identifier(ast* ast, size_t tab);
void ast_print_int(ast* ast, size_t tab);
void ast_print_action(ast* ast, size_t tab);
void ast_print_call(ast* ast, size_t tab);

//permet de faire plusieurs tabulations
static void print_indent(size_t indent)
{
	for(size_t i=0; i<indent; i++)
	{
		printf("\t");
	}
}

//alloue un nouvel ast avec type = UNDEFINED
ast* ast_alloc()
{
	ast* new = calloc(1, sizeof(ast));
	new->type = UNDEFINED;
	add_alloc(new);
	return new;
}

ast* ast_new_id(char* id)
{
	ast* new = ast_alloc();
	new->type = IDENTIFIER;
	new->u.id = id;
	return new;
}

ast* ast_new_nombre(int nombre)
{
	ast* new = ast_alloc();
	new->type = INT;
	new->u.nombre = nombre;
	return new;
}

ast* ast_new_affectation(char* id, ast* expr)
{
	ast* new = ast_alloc();
	new->type = AFFECT;
	new->u.affect.id = ast_new_id(id);
	new->u.affect.expr = expr;
	return new;
}

ast* ast_new_fonction(char* name, ast* action, ast* retour)
{
	ast* new = ast_alloc();
	new->type = FCT;
	new->u.fct.name = strdup(name);
	add_alloc(new->u.fct.name);
	new->u.fct.action = action;
	new->u.fct.retour = retour;
	return new;
}

ast* ast_new_call(char* name, ast* arg)
{
	ast* new = ast_alloc();
	new->type = CALL;
	new->u.call.name = name;
	new->u.call.arg = arg;
	return new;
}

ast* ast_new_if(ast* condition, ast* action, ast* els)
{
	ast* new = ast_alloc();
	new->type = IF_STMT;
	new->u.ifstmt.condition = condition;
	new->u.ifstmt.action = action;
	new->u.ifstmt.els = els;
	return new;
}

ast* ast_new_for(ast* min, ast* max, ast* incr, ast* action)
{
	ast* new = ast_alloc();
	new->type = FOR_STMT;
	new->u.forstmt.min = min;
	new->u.forstmt.max = max;
	new->u.forstmt.incr = incr;
	new->u.forstmt.action = action;
	return new;
}

ast* ast_new_unop(char* op, ast* fils)
{
	ast* new = ast_alloc();
	new->type = UN_OP;
	new->u.unop.op = strdup(op);
	add_alloc(new->u.unop.op);
	new->u.unop.fils = fils;
	return new;
}

ast* ast_new_binop(char* op, ast* left, ast* right)
{
	ast* new = ast_alloc();
	new->type = BIN_OP;
	new->u.binop.op = strdup(op);
	add_alloc(new->u.binop.op);
	new->u.binop.left = left;
	new->u.binop.right = right;
	return new;
}

ast* ast_new_action(ast* instruction, ast* action)
{
	ast* new = ast_alloc();
	new->type = ACTION;
	new->u.action.instruction = instruction;
	new->u.action.action = action;
	return new;
}

//génères les quads en parcourant l'ast
struct symbol* astGencode(ast* src,struct symtable* t, struct code* c)
{
	struct symbol* s = NULL;
	if(src !=NULL)
	{
		switch(src->type)
		{
			case BIN_OP://BOP_PLUS, BOP_MINUS, BOP_MULT, BOP_DIV
				s = newtemp(t);
				struct ast* left = src->u.binop.left;
				struct ast* right = src->u.binop.right;
				if(strcmp(src->u.binop.op,"+") == 0)
				{
					gencode(c, BOP_PLUS, s, astGencode(left,t,c), astGencode(right,t,c));
				}
				else if(strcmp(src->u.binop.op, "-") == 0)
				{
					gencode(c, BOP_MINUS, s, astGencode(left,t,c), astGencode(right,t,c));
				}
				else if(strcmp(src->u.binop.op, "*") == 0)
				{
					gencode(c, BOP_MULT, s, astGencode(left,t,c), astGencode(right,t,c));
				}
				else if(strcmp(src->u.binop.op, "/") == 0)
				{
					gencode(c, BOP_DIV, s, astGencode(left,t,c), astGencode(right,t,c));
				}
				else if(strcmp(src->u.binop.op, "+=") == 0)
				{
					symbol* left_symbol = astGencode(left,t,c);
					gencode(c, BOP_PLUS, s, left_symbol, astGencode(right,t,c));
					gencode(c, COPY, left_symbol, s, NULL);
					s = left_symbol;
				}
				else if(strcmp(src->u.binop.op, "-=") == 0)
				{
					symbol* left_symbol = astGencode(left,t,c);
					gencode(c, BOP_MINUS, s, left_symbol, astGencode(right,t,c));
					gencode(c, COPY, left_symbol, s, NULL);
					s = left_symbol;
				}
				break;
			case UN_OP:
				s = newtemp(t);
				if(strcmp(src->u.unop.op, "-") == 0)
				{
					gencode(c, UOP_MINUS, s, astGencode(src->u.binop.right, t, c), NULL);
				}
				else if(strcmp(src->u.unop.op, "++") == 0)
				{
					symbol* fils = astGencode(src->u.unop.fils, t, c);
					gencode(c, BOP_PLUS, s, fils, symtable_const(t,1));
					gencode(c, COPY, fils, s, NULL);
					s = fils;
				}
				else if(strcmp(src->u.unop.op, "--") == 0)
				{
					symbol* fils = astGencode(src->u.unop.fils, t, c);
					gencode(c, BOP_MINUS, s, fils, symtable_const(t,1));
					gencode(c, COPY, fils, s, NULL);
					s = fils;
				}
				break;
			case FOR_STMT:
				fprintf(stderr,"@gencode Erreur : la gestion des structures de contrôles n'est pas encore implémentée\n");
				break;
			case IF_STMT:
				fprintf(stderr,"@gencode Erreur : la gestion des structures de contrôles n'est pas encore implémentée\n");
				break;
			case FCT:
				astGencode(src->u.fct.action,t, c);
				break;
			case AFFECT:
				gencode(c, COPY, astGencode(src->u.affect.id, t, c),
								astGencode(src->u.affect.expr, t, c), NULL);
				break;
			case IDENTIFIER:
				s = symtable_get(t, src->u.id);
				break;
			case INT:
				s = symtable_const(t, src->u.nombre);
				break;
			case ACTION:
				astGencode(src->u.action.instruction, t, c);
				s = astGencode(src->u.action.action, t, c);
				break;
			case WHILE_STMT:
				fprintf(stderr,"@gencode Erreur : la gestion des structures de contrôles n'est pas encore implémentée\n");
				break;
			case CALL:
				if( strcmp(src->u.call.name, "printf") == 0 ||
					strcmp(src->u.call.name, "printi") == 0 )
				{
					s = astGencode(src->u.call.arg, t, c);
					if(s == NULL)
					{
						fprintf(stderr, "@gencode Erreur fatale : l'argument de print est NULL\nAST correspondant:\n");
						ast_print(src->u.call.arg, 0);
						exit(EXIT_FAILURE);
					}
					gencode(c, CALL_PRINT, s, NULL, NULL);
				}
				else
				{
					fprintf(stderr,"@gencode Erreur : la gestion des fonction définie par l'utilisateur n'est pas encore implémentée\n");
				}
				break;
			default:
				fprintf(stderr, "@gencode Erreur : type ast non renseigné\n");
		}
	}
	return s ;
}


//Fait appel aux fonctions ast_free_[TYPE AST]
//puis appel free avec ast donné en paramètre
//pas nécessaire si on fait appel à ast_alloc_free()
void ast_free(ast *ast)
{
	if(ast == NULL)
	{
		return;
	}

	switch(ast->type)
	{
		case BIN_OP:
			ast_free_binop(ast);
			break;
		case UN_OP:
			ast_free_unop(ast);
			break;
		case FOR_STMT:
			ast_free_for(ast);
			break;
		case IF_STMT:
			ast_free_if(ast);
			break;
		case FCT:
			ast_free_fct(ast);
			break;
		case AFFECT:
			ast_free_affect(ast);
			break;
		case IDENTIFIER:
			ast_free_identifier(ast);
			break;
		case INT:
			ast_free_int(ast);
			break;
		case ACTION:
			ast_free_action(ast);
			break;
		case CALL:
			ast_free_call(ast);
			break;
		default:
			fprintf(stderr, "ast non reconnus\n");
			return;
	}
	if(ast != NULL)
	{
		free(ast);
		ast = NULL;
	}
}

void ast_print(ast* ast, size_t tab)
{
	if(ast == NULL)
	{
		return; //rien à afficher
	}

	print_indent(tab);

	switch(ast->type)
	{
		case BIN_OP:
			ast_print_binop(ast, tab);
			break;
		case UN_OP:
			ast_print_unop(ast, tab);
			break;
		case FOR_STMT:
			ast_print_for(ast, tab);
			break;
		case IF_STMT:
			ast_print_if(ast, tab);
			break;
		case FCT:
			ast_print_fct(ast, tab);
			break;
		case AFFECT:
			ast_print_affect(ast, tab);
			break;
		case IDENTIFIER:
			ast_print_identifier(ast, tab);
			break;
		case INT:
			ast_print_int(ast, tab);
			break;
		case ACTION:
			ast_print_action(ast, tab);
			break;
		case CALL:
			ast_print_call(ast,tab);
			break;
		default:
			fprintf(stderr, "ast non reconnus\n");
	}
}

//=====================================================================
//Fonctions utilitaires pour allèger les autres fonctions

//Les fonctions ast_free_[TYPE AST] ne devrait pas avoir à faire de vérifs en théorie
void ast_free_binop(ast* ast)
{
	free(ast->u.binop.op);
	ast->u.binop.op = NULL;
	ast_free(ast->u.binop.left);
	ast_free(ast->u.binop.right);
}

void ast_free_unop(ast* ast)
{
	free(ast->u.unop.op);
	ast->u.unop.op = NULL;
	ast_free(ast->u.unop.fils);
}

void ast_free_fct(ast* ast)
{
	free(ast->u.fct.name);
	ast->u.fct.name = NULL;
	ast_free(ast->u.fct.action);
	ast_free(ast->u.fct.retour);
}

void ast_free_for(ast* ast)
{
	ast_free(ast->u.forstmt.min);
	ast_free(ast->u.forstmt.max);
	ast_free(ast->u.forstmt.incr);
	ast_free(ast->u.forstmt.action);
}

void ast_free_if(ast* ast)
{
	ast_free(ast->u.ifstmt.condition);	
	ast_free(ast->u.ifstmt.action);
	ast_free(ast->u.ifstmt.els);
}

void ast_free_affect(ast* ast)
{
	ast_free(ast->u.affect.id);
	ast_free(ast->u.affect.expr);
}

void ast_free_identifier(ast* ast)
{
	free(ast->u.id);
	ast->u.id = NULL;
}

void ast_free_int(ast* ast)
{
	// rien à faire à priori
	// free(ast) est appelée dans ast_free(ast)
	// qui libère au passage le champ nombre
}

void ast_free_action(ast* ast)
{
	ast_free(ast->u.action.instruction);
	ast_free(ast->u.action.action);
}

void ast_free_call(ast* ast)
{
	ast->u.call.name = NULL;
	ast_free(ast->u.call.arg);
}


void ast_print_binop(ast* ast, size_t tab)
{
	printf("%s\n",ast->u.binop.op);
	ast_print(ast->u.binop.left, tab+1);
	ast_print(ast->u.binop.right, tab+1);
}

void ast_print_unop(ast* ast, size_t tab)
{
	printf("%s\n",ast->u.unop.op);
	ast_print(ast->u.unop.fils, tab+1);
}

void ast_print_for(ast* ast, size_t tab)
{
	printf("FOR\n");
	print_indent(tab);
	printf("MIN :\n");
	ast_print(ast->u.forstmt.min, tab+1);
	print_indent(tab);
	printf("MAX :\n");
	ast_print(ast->u.forstmt.max, tab+1);
	print_indent(tab);
	printf("INCR :\n");
	ast_print(ast->u.forstmt.incr, tab+1);
	print_indent(tab);
	printf("ACTION :\n");
	ast_print(ast->u.forstmt.action, tab+1);
}

void ast_print_if(ast* ast, size_t tab)
{
	printf("IF :\n");
	ast_print(ast->u.ifstmt.condition, tab+1);
	print_indent(tab);
	printf("THEN :\n");
	ast_print(ast->u.ifstmt.action, tab+1);
	print_indent(tab);
	printf("ELSE :\n");
	ast_print(ast->u.ifstmt.els, tab+1);
}

void ast_print_fct(ast* ast, size_t tab)
{
	printf("FONCTION %s :\n", ast->u.fct.name);
	ast_print(ast->u.fct.action, tab+1);
	print_indent(tab);
	printf("RETURN :\n");
	ast_print(ast->u.fct.retour, tab+1);
}

void ast_print_affect(ast* ast, size_t tab)
{
	printf("=\n");
	ast_print(ast->u.affect.id, tab+1);
	ast_print(ast->u.affect.expr, tab+1);
}

void ast_print_identifier(ast* ast, size_t tab)
{
	printf("%s\n", ast->u.id);
}

void ast_print_int(ast* ast, size_t tab)
{
	printf("%d\n", ast->u.nombre);
}

void ast_print_action(ast* ast, size_t tab)
{
	ast_print(ast->u.action.instruction, tab);
	ast_print(ast->u.action.action, tab);
}

void ast_print_call(ast* ast, size_t tab)
{
	printf("call %s\n", ast->u.call.name);
	ast_print(ast->u.call.arg, tab+1);
}