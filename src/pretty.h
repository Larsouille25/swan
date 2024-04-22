#ifndef SWAN_PRETTY_H
#define SWAN_PRETTY_H

#include "lexer.h"
#include <stdio.h>

void snpty_display_token(FILE* file, Token* tok);

extern char* snpty_token_names[];
void snpty_dbg_token(FILE* file, Token* t);

#endif // SWAN_PRETTY_H
