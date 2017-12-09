#include "mips.h"

//génère du code MIPS dans le fichier out
//ou dans stdout si out == NULL
void quad_to_MIPS(symtable* t, code* c, FILE* out)
{
    if(out == NULL)
    {
        out = stdout;
    }
}