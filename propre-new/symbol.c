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
    new->u.string.content = NULL;
    new->u.string.string_id = NULL;
    new->u.name = NULL;
    return new;
}

symbol* symtable_const(symtable* t, int v)
{
    if(t->first == NULL) //si table vide
    {
        symbol* new = symbol_new();
        new->kind = CONSTANT;
        new->u.value = v;
        t->first = new;
        return new;
    }
    else //sinon on met new en fin de liste sauf s'il existe deja
    {
        symbol* scan = t->first;
        while(scan->next != NULL)
        {
            if(scan->kind == CONSTANT && scan->u.value == v)
            {
                return scan;
            }
            scan = scan->next;
        }
        symbol* new = symbol_new();
        new->kind = CONSTANT;
        new->u.value = v;
        scan->next = new;
        return new;
    }
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
        else if(scan->kind == STRING_SYMBOL && strcmp(scan->u.string.string_id, s) == 0)
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

symbol* symtable_put_string(symtable* t, char* string_content)
{
    static size_t string_num = 0;
    symbol* new = symbol_new();
    new->kind = STRING_SYMBOL;

    char string_id[TEMP_NAME_LENGTH_LIMIT];
    snprintf(string_id, TEMP_NAME_LENGTH_LIMIT, "str%zu", string_num);

    new->u.string.string_id = strdup(string_id);
    new->u.string.content = string_content;

    if(t->first == NULL)
    {
        t->first = new;
    }
    else
    {
        symbol* scan = t->first;
        while(scan->next != NULL)
        {
            scan = scan->next;
        }
        scan->next = new;
    }

    string_num++;
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
            printf("%d", s->u.value);
            break;
        case NAME:
            printf("%s", s->u.name);
            break;
        case STRING_SYMBOL:
            printf("%s : %s", s->u.string.string_id, s->u.string.content);
            break;
    }
}

void symbol_dump_file(symbol* s, FILE* out)
{
    if(s == NULL)
    {
        return;
    }
    switch(s->kind)
    {
        case CONSTANT:
            fprintf(out, "%d", s->u.value);
            break;
        case NAME:
            fprintf(out, "%s", s->u.name);
            break;
        case STRING_SYMBOL:
            fprintf(out, "%s", s->u.string.content);
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
        case STRING_SYMBOL:
            free(s->u.string.content);
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
    snprintf(temp_name, TEMP_NAME_LENGTH_LIMIT,
                "tmp%d", t->temp_num);
    t->temp_num++;
    return symtable_put(t, temp_name);
}