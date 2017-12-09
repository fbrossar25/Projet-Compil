#include "mips.h"

//Fonctions utilitaires pour écrire du MIPS

//écrit la ligne qui charge le symbole s dans le registre reg
void load_symbol(symbol* s, const char* reg, FILE* out)
{
    if (s->kind==NAME){
        fprintf(out, "lw $%s, ",reg);
    } else {
        fprintf(out, "li $%s, ",reg);
    }
    symbol_dump_file(s, out);      
    fprintf(out, "\n");

}

//écrit la ligne que range la valeur du registre reg dans le variable MIPS s
void store_reg_to_symbol(const char* reg, symbol* s, FILE* out)
{
    fprintf(out, "sw $t0, ");
    symbol_dump_file(s, out); 
    fprintf(out, "\n");
}

//écrit la ligne correspondant à un opérateur à 3 registre
void mips_3reg_op(const char* op, const char* res, const char* reg1, const char* reg2, FILE* out)
{
    fprintf(out, "%s $%s, $%s, $%s\n", op, res, reg1, reg2);
}

void mips_2reg_op(const char* op, const char* res, const char* reg, FILE* out)
{
    fprintf(out, "%s $%s, $%s\n", op, res, reg);
}

//éxecute le syscall n
void mips_syscall(int n, FILE* out)
{
    fprintf(out, "li $v0, %d\nsyscall\n", n);
}

//éxecute un syscall 10
void mips_exit(FILE* out)
{
    fprintf(out, "#EXIT\n");
    mips_syscall(10, out);
}


//Fonction générant du MIPS

//génère du code MIPS dans le fichier out
//ou dans stdout si out == NULL
void quad_to_MIPS(symtable* t, code* c, char* out_file)
{
    printf("\n\nGENERATING MIPS....");

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

    //symboles -> variable MIPS
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

    //quad -> MIPS

    quad* q = NULL;

    fprintf(out, "\n.text\n\nmain:\n\n");
    for(size_t i=0; i<c->nextquad; i++)
    {
        q = &(c->quads[i]);
        switch(q->kind)
        {
            case BOP_PLUS:
                fprintf(out, "#BOP_PLUS\n");
                load_symbol(q->sym2, "t0", out);
                load_symbol(q->sym3, "t1", out);
                mips_3reg_op("add", "t0", "t0", "t1", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case BOP_MINUS:
                fprintf(out, "#BOP_MINUS\n");
                load_symbol(q->sym2, "t0", out);
                load_symbol(q->sym3, "t1", out);
                mips_3reg_op("sub", "t0", "t0", "t1", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case BOP_MULT:
                fprintf(out, "#BOP_MULT\n");
                load_symbol(q->sym2, "t0", out);
                load_symbol(q->sym3, "t1", out);
                mips_3reg_op("mul", "t0", "t0", "t1", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case BOP_DIV:
                fprintf(out, "#BOP_DIV\n");
                load_symbol(q->sym2, "t0", out);
                load_symbol(q->sym3, "t1", out);
                mips_3reg_op("div", "t0", "t0", "t1", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case UOP_MINUS:
                fprintf(out, "#UOP_MINUS\n");
                load_symbol(q->sym2, "t0", out);
                mips_2reg_op("neg", "t0", "t0", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case COPY:
                fprintf(out, "#COPY\n");
                load_symbol(q->sym2, "t0", out);
                store_reg_to_symbol("t0", q->sym1, out);
                break;
            case CALL_PRINT:
                fprintf(out, "#CALL_PRINT\n");
                load_symbol(q->sym1, "a0", out);
                mips_syscall(1, out);
                break;
            default:
                fprintf(stderr, "quad %zu non reconnus !\n",i);
        }
        fprintf(out, "\n");
    } //for each quad end

    mips_exit(out);

    if(out != stdout)
    {
        fclose(out);
    }

    printf("\n\nMIPS GENERATED\n");
}