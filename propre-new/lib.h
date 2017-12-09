#ifndef LIB_H
#define LIB_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "symbol.h"

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

void code_dump(code* c);

void code_free(code* c);

#endif