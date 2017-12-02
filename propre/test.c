struct symbol*  astGencode(ast* src,struct symtable* t, struct code* c)
{
	struct symbol* s;
	if(src !=NULL)
	{
		
		if(src->type == NULL)
		{
			free(src);
		}
		else if(strcmp(src->type, "RETURN") == 0)
		{
			s = symtable_const(t,src->u.ret.retVal->u.number);		
		}
		else if(strcmp(src->type, "INT") == 0)
		{
			s = symtable_const(t,src->u.number);		
		}
		else if(strcmp(src->type, "ID") == 0)
		{
			s = symtable_put(t,src->u.affect.id );
			gencode(c,EQUAL,s,astGencode(src->u.affect.expr,t,c),newtemp(t));
		}
		else if(strcmp(src->type, "FCT") == 0)
		{
			s = symtable_put(t,src->u.affect.id );
			astGencode(src->u.fct.block ,t,c);
		}
		else if(strcmp(src->type, "+") == 0)
		{
			gencode(c, BOP_PLUS, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "/") == 0)
		{
			gencode(c, BOP_DIV, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "*") == 0)
		{
			gencode(c, BOP_MULT, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
		}
		else if(strcmp(src->type, "-") == 0)
		{
			if(src->u.op.left != NULL)
			{
				 gencode(c, BOP_MINUS, astGencode(src->u.op.left, t, c), astGencode(src->u.op.right, t, c), newtemp(t));
			}
			else
			{
				gencode(c, UOP_MINUS, NULL, astGencode(src->u.op.right, t, c), newtemp(t));
			}
		}
			
	}
		
	if(src->nextInstr != NULL)
	{
		s=astGencode(src->nextInstr,t,c);
	}
	return s ;
}


static void quad_dump(struct quad * q)
{

    switch ( q->kind )
    {
        case BOP_PLUS:
            symbol_dump(q->sym3);
            printf(" := ");
            symbol_dump(q->sym1);
            printf(" + ");
            symbol_dump(q->sym2);
            break;
        case BOP_MINUS:
            symbol_dump(q->sym3);
            printf(" := ");
            symbol_dump(q->sym1);
            printf(" - ");
            symbol_dump(q->sym2);
            break;
        case EQUAL:
            symbol_dump(q->sym3);
            printf(" := ");
            symbol_dump(q->sym2);
            printf(" = ");
            symbol_dump(q->sym1);
            break;
        case BOP_MULT:
            symbol_dump(q->sym3);
            printf(" := ");
            symbol_dump(q->sym1);
            printf(" * ");
            symbol_dump(q->sym2);
            break;
        case BOP_DIV:
            symbol_dump(q->sym3);
            printf(" := ");
            symbol_dump(q->sym1);
            printf(" / ");
            symbol_dump(q->sym2);
            break;
        case UOP_MINUS:
            symbol_dump(q->sym3);
            printf(" := ");
            printf("- ");
            symbol_dump(q->sym2);
            break;
       /* case CALL_PRINT:
            printf("print ");
            symbol_dump(q->sym1);
            break;
        case COPY:
            symbol_dump(q->sym1);
            printf(" := ");
            symbol_dump(q->sym2);
            break;*/
        default:
            printf("BUG\n");
            break;
    }
}
