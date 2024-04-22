#include "lexer.h"
#include "l25.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* pretty_token_type[] = {
	"(", ")", "[", "]", "{", "}",

	":", ";", ",", "@", "&", "*", "^", ".", ".*", "=", "==", "!", "!=", "<", "<<",
	"<=", ">", ">>", ">=", "-", "%", "|", "+", "/",

	KW_FUN, KW_EXTERN, KW_VAR, KW_CONST, KW_STRUCT, KW_ENUM, KW_RETURN, KW_IF, KW_ELSE,
	KW_WHILE, KW_FOR, KW_PUB, KW_ASYNC, KW_AWAIT, KW_IMPL, KW_TRUE, KW_FALSE,	KW_BREAK,
	KW_CONTINUE,

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

SwanLexer* snlxr_init(const char* code) {
	SwanLexer* l = malloc(sizeof(SwanLexer));
	l->code = code;
	l->codelen = strlen(code);
	l->idx = 0;
	l->prev_idx = 0;
	return l;
}

Token snlxr_token(SwanLexer* l, TokenType type, void* lexeme) {
	Token tok = {0};
	tok.type = type;
	tok.lexeme = lexeme;
	tok.span = (L25_Range){ .start = l->prev_idx, .end = l->idx };
	return tok;
}

Token snlxr_simpletokn(SwanLexer* l, TokenType type) {
	return snlxr_token(l, type, NULL);
}

char snlxr_peek(SwanLexer* l) {
	return l->code[l->idx];
}

char snlxr_pop(SwanLexer* l) {
	char c = snlxr_peek(l);
	l->idx++;
	return c;
}

SwanLexerRes snlxr_make_token(SwanLexer* l) {
	SwanLexerRes res = { .tag = SLR_TOK };
	if (l->idx >= l->codelen){
		res.tok = snlxr_simpletokn(l, TOKEN_EOF);
		return res;
	}
	switch (snlxr_pop(l)) {
		case 0x09:
		case 0x0A:
		case 0x0B:
		case 0x0C:
		case 0x0D:
		case 0x20:
			res.tag = SLR_WHITESPACE;
			break;
		case '(':
			res.tok = snlxr_simpletokn(l, TOKEN_LPAREN);
			break;
		case ')':
			res.tok = snlxr_simpletokn(l, TOKEN_RPAREN);
			break;
		default:
			exit(123);
	}
	return res;
}

bool snlxr_push_tok(SwanLexer* l, TokenStream* ts, Token tok) {
	if (tok.type == TOKEN_EOF) {
		tok.span = (L25_Range){.start = l->codelen - 1, .end = l->codelen };
		l25_vec_push(ts, tok);
		return true;
	}
	l25_vec_push(ts, tok);
	return false;
}

TokenStream* snlxr_lex(SwanLexer* l) {
	TokenStream* ts = malloc(sizeof(TokenStream));
	l25_vec_init(ts, SWAN_TOKEN_STREAM_CAP);
	SwanLexerRes res = {0};

	for(;;) {
		l->prev_idx = l->idx;
		res = snlxr_make_token(l);
		switch (res.tag) {
			case SLR_TOK:
				if(snlxr_push_tok(l, ts, res.tok))
					// reached end of file
					goto end;
				break;
			case SLR_ERROR:
				exit(1);
			case SLR_PART_SUCCESS:
				exit(15);
			case SLR_COMMENT:
			case SLR_WHITESPACE:
				break;
		}
	}
end:

	return ts;
}

void snlxr_deinit(SwanLexer* l) {
	free(l);
	l = NULL;
}
