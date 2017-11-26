#include "ast.h"

ast* ast_alloc()
{
  ast* new = malloc(sizeof(ast));
  new->type = NULL;
  new->nextInstr = NULL;
  return new;
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

void ast_destroy(ast* src)
{
  if(src == NULL)
  {
    return;
  }

  if(src->type == NULL)
  {
    fprintf(stderr, "erreur @ast_destroy: un type n'a pas été renseigné !\n");
  }
  else if(strcmp(src->type, "INT") == 0)
  {
    free(src->type);
  }
  else if(strcmp(src->type, "ID") == 0)
  {
    free(src->type);
    free(src->u.affect.id);
    ast_destroy(src->u.affect.expr);
  }
  else if(strcmp(src->type, "FCT") == 0)
  {
    free(src->type);
    free(src->u.fct.id);
    ast_destroy(src->u.fct.block);
  }
  else //OP
  {
    free(src->type);
    ast_destroy(src->u.op.left);
    ast_destroy(src->u.op.right);
  }

  if(src->nextInstr != NULL)
  {
    ast_destroy(src->nextInstr);
  }

  free(src);
}