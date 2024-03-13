#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	switch (l->code[l->current]) {
	case '(':
		tokn = lexer_token_simple(l, TOKEN_LPAREN);
		break;
	default:
		printf("Make an error system.");
		exit(1);
	}

	l->current++;

	return tokn;
}
