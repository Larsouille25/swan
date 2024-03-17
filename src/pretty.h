#ifndef SWAN_PRETTY_H
#define SWAN_PRETTY_H

#include "lexer.h"
#include <stdio.h>

void pretty_token(FILE* file, Token* tok);

extern char* dbg_token_names[];
void dbg_token(FILE* file, Token* t);

#endif // SWAN_PRETTY_H
