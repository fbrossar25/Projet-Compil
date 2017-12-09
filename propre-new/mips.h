#ifndef MIPS_H
#define MIPS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib.h"
#include "symbol.h"
#include "error.h"

void quad_to_MIPS(symtable* t, code* c, FILE* out);

#endif