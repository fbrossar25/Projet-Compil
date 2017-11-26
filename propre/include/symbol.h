#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SYMBOL_SIZE 32

typedef struct symbol
{
	char* identifier;
	bool isConstant;
	int value;
	struct symbol* next;
} symbol;

symbol* sybol_alloc();
symbol* symbol_newtemp(symbol** table);
symbol* symbol_lookup(symbol* table, char* name);
symbol* symbol_add(symbol** table, char* name);
void symbol_destroy(symbol* table);
void symbol_print(symbol* symbol);

#endif