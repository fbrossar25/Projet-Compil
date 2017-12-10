#include "alloc.h"

static alloc_list*  _alloc_list = NULL;

static void alloc_list_new()
{
    _alloc_list = calloc(1, sizeof(alloc_list));
    _alloc_list->capacity = INITIAL_ALLOC_LIST_CAPACITY;
    _alloc_list->size = 0;
    _alloc_list->allocs = calloc(INITIAL_ALLOC_LIST_CAPACITY, sizeof(void*));
}

static void alloc_list_grow()
{
    _alloc_list->capacity += INITIAL_ALLOC_LIST_CAPACITY;
    _alloc_list->allocs = realloc(_alloc_list->allocs, _alloc_list->capacity * sizeof(void*));
}

void add_alloc(void* alloc)
{
    if(_alloc_list == NULL)
    {
        alloc_list_new();
    }
    if(_alloc_list->size + 1 >= _alloc_list->capacity)
    {
        alloc_list_grow();
    }
    _alloc_list->allocs[_alloc_list->size] = alloc;
    _alloc_list->size++;
}

void alloc_list_free()
{
    for(size_t i=0; i<_alloc_list->size; i++)
    {
        free(_alloc_list->allocs[i]);
    }
    free(_alloc_list->allocs);
    free(_alloc_list);
}