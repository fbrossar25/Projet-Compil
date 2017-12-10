#ifndef ERROR_H
#define ERROR_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_MESSAGE_LENGTH 128

void error(char* e, int line);

int get_error_count();

void incr_error_count();

#endif