#include "mips.h"

//génère du code MIPS dans le fichier out
//ou dans stdout si out == NULL
void quad_to_MIPS(symtable* t, code* c, char* out_file)
{
    FILE* out = NULL;
    if(out_file != NULL)
    {
        out = fopen(out_file, "w+");
        if(out == NULL)
        {
            fprintf(stderr, "Erreur lors de l'écriture dans '%s'\n", out_file);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        out = stdout;
    }

    printf("\n\nGENERATING MIPS....");

    //declaration des variables
    fprintf(out, ".data\n");

    symbol* scan = t->first;
    while(scan != NULL)
    {
        if(scan->kind == NAME)
        {
            fprintf(out, "\t%s", scan->u.name);
            fprintf(out, ":");
            fprintf(out, "\t.word 0\n");
        }
        scan = scan->next;
    }

    if(out != stdout)
    {
        fclose(out);
    }

    printf("\n\nMIPS GENERATED\n");
}