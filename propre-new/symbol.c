#include "symbol.h"

symtable* symtable_new()
{
    symtable* new = calloc(1, sizeof(symtable));
    new->first = NULL;
    new->temp_num = 0;
    return new;
}

static symbol* symbol_new()
{
    symbol* new = calloc(1, sizeof(symbol));
    new->next = NULL;
    return new;
}

symbol* symtable_const(symtable* t, int v)
{
    symbol* new = symbol_new();
    new->kind = CONSTANT;
    new->u.value = v;
    if(t->first == NULL) //si table vide
    {
        t->first = new;
    }
    else //sinon on met new en fin de liste
    {
        symbol* scan = t->first;
        while(scan->next != NULL)
        {
            scan = scan->next;
        }
        scan->next = new;
    }
    return new;
}

symbol* symtable_get(symtable* t, const char * s)
{
    symbol* scan = t->first;
    while(scan != NULL)
    {
        if(scan->kind == NAME && strcmp(scan->u.name, s) == 0)
        {
            return scan;
        }
        scan = scan->next;
    }
    return NULL;
}

symbol* symtable_put(symtable* t, const char * s)
{
    symbol* new = symbol_new();
    new->kind = NAME;
    new->u.name = strdup(s);
    if(t->first == NULL) //si table vide
    {
        t->first = new;
    }
    else //sinon on met new en fin de liste
    {
        symbol* scan = t->first;
        while(scan->next != NULL)
        {
            scan = scan->next;
        }
        scan->next = new;
    }
    return new;
}

void symtable_dump(struct symtable * t)
{
    symbol* scan = t->first;
    while (scan != NULL)
    {
        printf("    %p : ",scan);
        symbol_dump(scan);
        printf("\n");
        scan = scan->next;
    }
}

void symbol_dump(symbol* s)
{
    if(s == NULL)
    {
        printf("NULL");
        return;
    }
    switch(s->kind)
        {
            case CONSTANT:
                printf("%d",s->u.value);
                break;
            case NAME:
                printf("%s",s->u.name);
                break;
        }
}

static void symbol_free(symbol* s)
{
    switch(s->kind)
    {
        case CONSTANT:
            //rien à faire
            break;
        case NAME:
            free(s->u.name);
            break;
    }
    free(s);
}

void symtable_free(symtable* t)
{
    symbol* scan = t->first;
    symbol* tmp = scan;
    while(tmp != NULL)
    {
        scan = scan->next;
        symbol_free(tmp);
        tmp = scan;
    }
    free(t);
}

symbol* newtemp(symtable * t)
{
    char temp_name[TEMP_NAME_LENGTH_LIMIT];
    snprintf(temp_name, TEMP_NAME_LENGTH_LIMIT, "tmp%d", t->temp_num);
    t->temp_num++;
    return symtable_put(t, temp_name);
}