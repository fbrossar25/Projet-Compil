#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void error(char* e, int line);

int get_error_count();

void incr_error_count();

#endif