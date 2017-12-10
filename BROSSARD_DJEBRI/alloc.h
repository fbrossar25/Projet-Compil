#ifndef ALLOC_H
#define ALLOC_H

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_ALLOC_LIST_CAPACITY 16

typedef struct alloc_list {
    size_t capacity;
    size_t size;
    void** allocs;
} alloc_list;

void add_alloc(void* alloc);

void alloc_list_free();

#endif