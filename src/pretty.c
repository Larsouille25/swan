#include "pretty.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

char* dbg_token_names[] = {
	"LPAREN", "RPAREN", "LBRACKET", "RBRACKET", "LBRACE", "LBRACE",

	"COLON", "SEMICOLON", "COMMA", "AT", "AMPERSAND", "ASTERISK", "CARET", "DOT",
	"DOTASTERISK", "EQUAL", "EQUAL2", "BANG", "BANGEQUAL", "LARROW", "LARROW2",
	"LARROWEQUAL", "RARROW", "RARROW2", "RARROWEQUAL", "MINUS", "PERCENT", "PIPE",
	"PLUS", "SLASH",

	"KW_FN", "KW_EXTERN", "KW_VAR", "KW_CONST", "KW_STRUCT", "KW_ENUM",
	"KW_RETURN", "KW_IF", "KW_ELSE", "KW_WHILE", "KW_FOR", "KW_PUB", "KW_ASYNC",
	"KW_AWAIT", "KW_IMPL", "KW_TRUE", "KW_FALSE", "KW_BREAK",
	"KW_CONTINUE", "KW_PACKAGE", "KW_IMPORT",

	"INT_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", "CHAR_LITERAL",

	"IDENT", "EOF"
};

void pretty_token(FILE* file, Token* tok) {
	fprintf(file, "%s", pretty_token_type[(int)tok->type]);

	if (tok->lexeme) {
		fprintf(file, "(");

		switch (tok->type) {
		case TOKEN_INT:
			fprintf(file, "%ld", (long int)tok->lexeme);
			break;
		case TOKEN_FLOAT:
			fprintf(file, "%f", (double)(long int)tok->lexeme);
			break;
		case TOKEN_STRING:
			fprintf(file, "%s", (char*)tok->lexeme);
			break;
		case TOKEN_CHAR:
			fprintf(file, "%c", *(char*)tok->lexeme);
			break;
		case TOKEN_IDENT:
			fprintf(file, "%s", (char*)tok->lexeme);
			break;
		default:
			printf("%s doesn't support lexemes", pretty_token_type[(int)tok->type]);
			exit(1);
    }

		fprintf(file, ")");
	}
}

void dbg_token(FILE* file, Token* t) {
	fprintf(file, "%s", dbg_token_names[(int)t->type]);

	if (t->lexeme) {
		fprintf(file, "(");

		switch (t->type) {
		case TOKEN_INT:
			fprintf(file, "%ld", (long int)t->lexeme);
			break;
		case TOKEN_FLOAT:
			fprintf(file, "%f", (double)(long int)t->lexeme);
			break;
		case TOKEN_STRING:
			fprintf(file, "%s", (char*)t->lexeme);
			break;
		case TOKEN_CHAR:
			fprintf(file, "%c", *(char*)t->lexeme);
			break;
		case TOKEN_IDENT:
			fprintf(file, "%s", (char*)t->lexeme);
			break;
		default:
			printf("%s doesn't support lexemes", pretty_token_type[(int)t->type]);
			exit(1);
    }

		fprintf(file, ")");
	}
}
