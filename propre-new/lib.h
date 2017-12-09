#ifndef LIB_H
#define LIB_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol.h"

/* TABLE DES SYMBOLES */
/*
typedef char name_t[8];

typedef enum symbol_kind { NAME, CONSTANT } symbol_kind;

typedef struct symbol {
  symbol_kind kind;
    union {
        name_t name;
        long int value;
    } u;
} symbol;

typedef struct symtable {
    unsigned int capacity;
    unsigned int temporary; 
    unsigned int size;
    symbol* symbols;
} symtable;

struct symtable* symtable_new();

symbol* symtable_const(symtable* t, long int v);

symbol* symtable_get(symtable* t, const char * s);

symbol* symtable_put(symtable* t, const char * s);

void symtable_dump(symtable* t);

void symtable_free(symtable* t);
*/

/* QUADRUPLETS ET CODE */

typedef enum quad_kind { BOP_PLUS, BOP_MINUS, BOP_MULT, BOP_DIV, UOP_MINUS, COPY, CALL_PRINT } quad_kind;

typedef struct quad {
  quad_kind kind;
  symbol* sym1;
  symbol* sym2;
  symbol* sym3;
} quad;

typedef struct code {
    unsigned int capacity;
    unsigned int nextquad;
    quad* quads;
} code;

code* code_new();

void gencode(code* c, quad_kind k, symbol* s1, symbol* s2, symbol* s3);

//symbol* newtemp(struct symtable * t);

void code_dump(code* c);

void code_free(code* c);

#endif