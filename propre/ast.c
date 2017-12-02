#include "ast.h"
static ast_list* astList = NULL;

ast* ast_alloc()
{
  ast* new = malloc(sizeof(ast));
  new->type = NULL;
  new->nextInstr = NULL;
	add_to_ast_list(new);
  return new;
}

void add_to_ast_list(ast* new)
{
	if(astList == NULL)
	{
		astList = malloc(sizeof(ast_list));
		astList->node = new;
		astList->next = NULL;
		return;
	}

	ast_list* scan = astList;
	while(scan->next != NULL)
	{
		scan = scan->next;
	}
	scan->next = malloc(sizeof(ast_list));
	scan = scan->next;
	scan->node = new;
	scan->next = NULL;
}

void destroy_ast_list()
{
	ast_list* tmp;
	while(astList != NULL)
	{
		tmp = astList->next;
		if(astList->node != NULL)
		{
			free(astList->node);
		}
		free(astList);
		astList = tmp;
	}
}

ast* ast_new_operation(char* op, ast* left, ast* right)
{
  ast* new = ast_alloc();
  new->type = strdup(op);
  new->u.op.left = left;
  new->u.op.right = right;
  return new;
}

ast* ast_new_fonction(char* fct, ast* bloc)
{
  ast* new = ast_alloc();
  new->type = strdup("FCT");
  new->u.fct.id = strdup(fct);
  new->u.fct.block = bloc;
  return new;
}

ast* ast_new_id(char* id, ast* expr)
{
  ast* new = ast_alloc();
  new->type = strdup("ID");
  new->u.affect.id = strdup(id);
  new->u.affect.expr = expr;
  return new;
}


ast* ast_new_entier(int entier)
{
  ast* new = ast_alloc();
  new->type = strdup("INT");
  new->u.number = entier;
  return new;
}

ast* ast_new_retour(int entier)
{
  ast* new = ast_alloc();
  new->type = strdup("RETURN");
  new->u.ret.retVal = ast_new_entier(entier);
  return new;
}

void ast_print(ast* src, int indent)
{
  if(src == NULL)
  {
    return;
  }

  for(int i = 0; i<indent; i++)
  {
    printf("\t");
  }

  if(src->type == NULL)
  {
    printf("Erreur: type non renseigné\n");
  }
  else if(strcmp(src->type, "INT") == 0)
  {
    printf("INT(%d)\n",src->u.number);
  }
  else if(strcmp(src->type, "ID") == 0)
  {
    printf("ID(%s)\n",src->u.affect.id);
    ast_print(src->u.affect.expr, indent + 1);
  }
  else if(strcmp(src->type, "FCT") == 0)
  {
    printf("FCT(%s)\n", src->u.fct.id);
    ast_print(src->u.fct.block, indent + 1);
  }
  else
  {
    printf("OP(%s)\n", src->type);
    ast_print(src->u.op.left, indent + 1);
    ast_print(src->u.op.right, indent + 1);
  }
  
  if(src->nextInstr != NULL)
  {
    ast_print(src->nextInstr, indent);
  }
}

struct symbol*  astGencode(ast* src,struct symtable* t, struct code* c)
{
	struct symbol* s;
	if(src !=NULL)
	{
		
		if(src->type == NULL)
		{
			free(src);
		}
		else if(strcmp(src->type, "RETURN") == 0)
		{
			s = symtable_const(t,src->u.ret.retVal->u.number);		
		}
		else if(strcmp(src->type, "INT") == 0)
		{
			s = symtable_const(t,src->u.number);		
		}
		else if(strcmp(src->type, "ID") == 0)
		{
			s = symtable_put(t,src->u.affect.id );
			gencode(c,EQUAL,s,astGencode(src->u.affect.expr,t,c),newtemp(t));
		}
		else if(strcmp(src->type, "FCT") == 0)
		{
			s = symtable_put(t,src->u.affect.id );
			astGencode(src->u.fct.block ,t,c);
		}
		else if(strcmp(src->type, "+") == 0)
		{
		
			gencode(c, BOP_PLUS, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "/") == 0)
		{
			gencode(c, BOP_DIV, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "*") == 0)
		{
			gencode(c, BOP_MULT, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "-") == 0)
		{
			if(src->u.op.left != NULL)
			{
				 gencode(c, BOP_MINUS, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
			}
			else
			{
				gencode(c, UOP_MINUS, NULL, astGencode(src->u.op.right, t, c), newtemp(t));
			}
		}
			
	}
		
	if(src->nextInstr != NULL)
	{
		s=astGencode(src->nextInstr,t,c);
	}
	return s ;
}

void ast_destroy(ast* src)
{
	if(src !=NULL)
	{
		if(src->type == NULL)
		{
			free(src);
		}
		else if(strcmp(src->type, "INT") == 0)
		{
			ast_destroy(src->u.ret.retVal);
			free(src->type);
			free(src);
		}
		else if(strcmp(src->type, "INT") == 0)
		{
			free(src->type);
			free(src);
		}
		else if(strcmp(src->type, "ID") == 0)
		{
			free(src->type);
			free(src->u.affect.id);
			ast_destroy(src->u.affect.expr );
		}
		else if(strcmp(src->type, "FCT") == 0)
		{
			free(src->type);
			free(src->u.fct.id);
			ast_destroy( src->u.fct.block);
		}
		else
		{
			if(src->u.op.left != NULL)
			{
				free(src->type);
				ast_destroy(src->u.op.left);	
				ast_destroy(src->u.op.right);	
			}
			else
			{
				free(src->type);
				ast_destroy(src->u.op.right);
			}
		}
		
		if(src->nextInstr != NULL) //Invalid read ici... pourquoi pas...
		{
			ast_destroy(src->nextInstr);
		}
	}
}

int ast_eval(ast* src)
{
	int val = 0;
	if(src !=NULL)
	{
		
		if(src->type == NULL)
		{
			free(src);
		}
		else if(strcmp(src->type, "INT") == 0)
		{
			val = src->u.number;
		}
		else if(strcmp(src->type, "ID") == 0)
		{
	
			val = ast_eval(src->u.affect.expr );
		}
		else if(strcmp(src->type, "FCT") == 0)
		{
			val = ast_eval(src->u.fct.block);
		}
		else if(strcmp(src->type, "+") == 0)
		{
			val = ast_eval(src->u.op.left) + ast_eval(src->u.op.right) ; 
		}
		else if(strcmp(src->type, "/") == 0)
		{
			val = ast_eval(src->u.op.left) / ast_eval(src->u.op.right) ; 
		}
		else if(strcmp(src->type, "*") == 0)
		{
			val = ast_eval(src->u.op.left) * ast_eval(src->u.op.right) ; 
		}
		else if(strcmp(src->type, "-") == 0)
		{
			if(src->u.op.left != NULL)
			{
				val = ast_eval(src->u.op.left) - ast_eval(src->u.op.right) ; 
			}
			else
			{
				val = - (ast_eval(src->u.op.left)) ; 
			}
		}
		
		if(src->nextInstr != NULL)
		{
			val = ast_eval(src->nextInstr);
		}
	}
	return val ;
}

