#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//4 est le minimum pour écrire tmp_
//on peut donc déduire le nombre max de temporaire :
//max = 10^(TEMP_NAME_LENGTH_LIMIT - 4)
//avec TEMP_NAME_LENGTH_LIMIT 13 on donne 1 millions de temporaires
//10^(13 - 4) = 10^9 = 1 000 000 000
#define TEMP_NAME_LENGTH_LIMIT 13

typedef enum symbol_kind { NAME, CONSTANT } symbol_kind;

typedef struct symbol
{
    symbol_kind kind;
    union
    {
        char* name;
        int value;
    } u;
    struct symbol* next;
} symbol;

typedef struct symtable
{
    symbol* first;
    int temp_num;
} symtable;

struct symtable* symtable_new();

symbol* symtable_const(symtable* t, int v);

symbol* symtable_get(symtable* t, const char * s);

symbol* symtable_put(symtable* t, const char * s);

void symtable_dump(symtable* t);

void symtable_free(symtable* t);

symbol* newtemp(symtable * t);

void symbol_dump(symbol* s);

#endif