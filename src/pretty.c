#include "pretty.h"
#include "lexer.h"
#include <stdio.h>

void pretty_token(FILE* file, Token* tok) {
	fprintf(file, "%s", pretty_token_type[(int)tok->type]);

	if (tok->lexeme) {
		fprintf(file, "(");

		switch (tok->type) {
		case TOKEN_INT:
			fprintf(file, "\"%d\"", *(int*)tok->lexeme);
			break;
		case TOKEN_FLOAT:
			fprintf(file, "\"%f\"", *(double*)tok->lexeme);
			break;
		case TOKEN_STRING:
			fprintf(file, "\"%s\"", (char*)tok->lexeme);
			break;
		case TOKEN_CHAR:
			fprintf(file, "\"%c\"", *(char*)tok->lexeme);
			break;
		case TOKEN_IDENT:
			fprintf(file, "\"%s\"", (char*)tok->lexeme);
			break;
		default:
			printf("%s doesn't support lexemes", pretty_token_type[(int)tok->type]);
    }

		fprintf(file, ")");
	}
}
