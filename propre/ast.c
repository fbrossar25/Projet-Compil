#include "ast.h"

ast* ast_alloc()
{
  ast* new = malloc(sizeof(ast));
  new->u.op.left = NULL;
  new->u.op.right = NULL;
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

ast* ast_new_fonction(char* fct)
{
  ast* new = ast_alloc();
  new->type = strdup("FCT");
  new->u.id = strdup(fct);
  return new;
}

ast* ast_new_id(char* id)
{
  ast* new = ast_alloc();
  new->type = strdup("ID");
  new->u.id = strdup(id);
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
    //erreur
  }
  else if(strcmp(src->type, "INT"))
  {
    //affichage entier
  }
  else if(strcmp(src->type, "ID"))
  {
    //affichage id
  }
  else if(strcmp(src->type, "FCT"))
  {
    //affichage fonction
  }
  else
  {
    //affichage operation
  }
}

void ast_destroy(ast* src)
{
  if(src == NULL)
  {
    return;
  }

  if(src->u.op.left != NULL)
  {
    ast_destroy(src->u.op.left);
  }
  if(src->u.op.right!= NULL)
  {
    ast_destroy(src->u.op.right);
  }
  
  ast_free_node(src);
  free(src);
}

void ast_free_node(ast* node)
{
  if(node->type != NULL)
  {
    free(node->type);
    node->type = NULL;
  }

  if(node->u.id != NULL)
  {
    free(node->u.id);
    //free(node->u); // à vérifier
  }
}