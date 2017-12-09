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

void ast_print_binop(ast* ast, int tab);
void ast_print_unop(ast* ast, int tab);
void ast_print_for(ast* ast, int tab);
void ast_print_if(ast* ast, int tab);
void ast_print_fct(ast* ast, int tab);
void ast_print_affect(ast* ast, int tab);
void ast_print_identifier(ast* ast, int tab);
void ast_print_int(ast* ast, int tab);
void ast_print_action(ast* ast, int tab);

void print_indent(int indent)
{
	for(int i=0; i<indent; i++)
	{
		printf("\t");
	}
}

ast* ast_alloc()
{
	ast* new = calloc(1,sizeof(ast)); // calloc initialise tout à 0
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
	new->u.fct.action = action;
	new->u.fct.retour = retour;
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
	new->u.unop.fils = fils;
	return new;
}

ast* ast_new_binop(char* op, ast* left, ast* right)
{
	ast* new = ast_alloc();
	new->type = BIN_OP;
	new->u.binop.op = strdup(op);
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
				break;
			case UN_OP:
				if(strcmp(src->u.unop.op, "-") == 0)
				{
					s = newtemp(t);
					gencode(c, UOP_MINUS, s, astGencode(src->u.binop.right, t, c), NULL);
				}
				break;
			case FOR_STMT:
				//TODO
				break;
			case IF_STMT:
				//TODO
				break;
			case FCT:
				s = symtable_put(t,src->u.fct.name);
				astGencode(src->u.fct.action ,t,c);
				break;
			case AFFECT:
				gencode(c,COPY,astGencode(src->u.affect.id,t,c),
								astGencode(src->u.affect.expr,t,c),NULL);
				break;
			case IDENTIFIER:
				s = symtable_get(t,src->u.id);
				printf("trying to get symbol'%s', got ",src->u.id);
				if(s == NULL)
					printf("NULL\n");
				else if(s->kind == CONSTANT)
					printf("constant %d\n", s->u.value);
				else
					printf("name '%s'\n", s->u.name);
				break;
			case INT:
				s = symtable_const(t,src->u.nombre);
				break;
			case ACTION:
				astGencode(src->u.action.instruction,t,c);
				s = astGencode(src->u.action.action,t,c);
				break;
			case WHILE_STMT:
				//TODO
				break;
			default:
				fprintf(stderr, "Erreur : type ast non renseigné\n");
		}
	}
	return s ;
}


//Fait appel aux fonctions ast_free_[TYPE AST]
//puis appel free avec ast donné en paramètre
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
		default:
			fprintf(stderr, "ast non reconnus\n");
			return;
	}
	if(ast != NULL)
	{
		free(ast);
	}
}

void ast_print(ast* ast, int tab)
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
	ast_free(ast->u.binop.left);
	ast_free(ast->u.binop.right);
}

void ast_free_unop(ast* ast)
{
	free(ast->u.unop.op);
	ast_free(ast->u.unop.fils);
}

void ast_free_fct(ast* ast)
{
	free(ast->u.fct.name);
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



void ast_print_binop(ast* ast, int tab)
{
	printf("%s\n",ast->u.binop.op);
	ast_print(ast->u.binop.left, tab+1);
	ast_print(ast->u.binop.right, tab+1);
}

void ast_print_unop(ast* ast, int tab)
{
	printf("%s\n",ast->u.unop.op);
	ast_print(ast->u.unop.fils, tab+1);
}

void ast_print_for(ast* ast, int tab)
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

void ast_print_if(ast* ast, int tab)
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

void ast_print_fct(ast* ast, int tab)
{
	printf("FONCTION %s :\n", ast->u.fct.name);
	ast_print(ast->u.fct.action, tab+1);
	print_indent(tab);
	printf("RETURN :\n");
	ast_print(ast->u.fct.retour, tab+1);
}

void ast_print_affect(ast* ast, int tab)
{
	printf("=\n");
	ast_print(ast->u.affect.id, tab+1);
	ast_print(ast->u.affect.expr, tab+1);
}

void ast_print_identifier(ast* ast, int tab)
{
	printf("%s\n", ast->u.id);
}

void ast_print_int(ast* ast, int tab)
{
	printf("%d\n", ast->u.nombre);
}


void ast_print_action(ast* ast, int tab)
{
	ast_print(ast->u.action.instruction, tab);
	ast_print(ast->u.action.action, tab);
}