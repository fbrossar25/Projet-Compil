#include "error.h"

static int error_count = 0;

//Message d'erreur perso (sémantique)
void error(char* e, int line)
{
	error_count++;
	fprintf(stderr, "erreur %d : %s (ligne %d)",error_count, e, line);
	return;
}

int get_error_count()
{
    return error_count;
}

void incr_error_count()
{
    error_count++;
}