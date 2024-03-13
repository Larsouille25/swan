#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* pretty_token_type[] = {
	"(", ")", "[", "]", "{", "}",

	":", ";", ",", "@", "&", "*", "^", ".", ".*", "=", "==", "!", "!=", "<", "<<",
	"<=", ">", ">>", ">=", "-", "%", "|", "+", "/",

	KW_FN, KW_EXTERN, KW_VAR, KW_CONST, KW_STRUCT, KW_ENUM, KW_RETURN, KW_IF, KW_ELSE,
	KW_WHILE, KW_FOR, KW_PUB, KW_ASYNC, KW_AWAIT, KW_IMPL, KW_TRUE, KW_FALSE,	KW_BREAK,
	KW_CONTINUE, KW_PACKAGE, KW_IMPORT,

	"integer literal", "float literal", "string literal", "char literal",

	"identifier", "end of file"
};

char* read_file(const char* filepath) {
	size_t lenght = 0, readBytes = 0;
	char* buf = NULL;
	FILE* fp = fopen(filepath, "rb");

	if (fp == NULL) {
		printf("Cannot open file: %s\n", filepath);
		return NULL;
	}

	if (!fseek(fp, 0, SEEK_END)) {
		lenght = ftell(fp);
		rewind(fp);
	}

	buf = (char*)malloc(lenght + 1);
	if (buf == NULL) {
		printf("Cannot allocate memory for file: %s\n", filepath);
		return NULL;
	}

	memset(buf, 0, lenght + 1);
	readBytes = fread(buf, 1, lenght, fp);
	if (readBytes < lenght) {
		printf("Cannot read file: %s; read = %ld, len = %ld\n", filepath, readBytes, lenght);
		return NULL;
	}

	if (fclose(fp)) {
		printf("Cannot close the file descriptor\n");
		free(buf);
		return NULL;
	}

	return buf;
}

void lexer_init(SwanLexer* l, const char* code) {
	l->code = code;
	l->current = 0;
	l->line = 1;
	l->lenght = strlen(l->code);
}

Token lexer_token(SwanLexer* l, TokenType type, void* lexeme) {
	Token tok = {0};
	tok.type = type;
	tok.lexeme = lexeme;
	tok.column = l->current;
	tok.line = l->line;
	return tok;
}

Token lexer_token_simple(SwanLexer* l, TokenType type) {
	return lexer_token(l, type, NULL);
}

void lexer_skip_whitespace(SwanLexer* l) {
	while (!IS_AT_END(l->current, l->lenght)) {
	switch (l->code[l->current]) {
    case ' ':
		case '\r':
		case '\t':
			l->current++;
			break;
		case '\n':
			l->line++;
		default:
			return;
    }
  }
}

Token lexer_make_token(SwanLexer* l) {
	lexer_skip_whitespace(l);

	Token tokn = {0};

	if (IS_AT_END(l->current, l->lenght)) {
		tokn = lexer_token_simple(l, TOKEN_EOF);
		return tokn;
	}

	switch (l->code[l->current]) {
	case '(':
		tokn = lexer_token_simple(l, TOKEN_LPAREN);
		break;
	default:
		printf("Make an error system. '%c'\n", l->code[l->current]);
		exit(1);
	}

	l->current++;

	return tokn;
}
