#ifndef SWAN_LEXER_H
#define SWAN_LEXER_H

#include "l25.h"
#include "errors.h"

#include <stdbool.h>
#include <stddef.h>

/*
 * /!\ If you want to add a token type, add the pretty printing for it in
 * the global 'pretty_token_type', below this enum.
*/
typedef enum {
	// Delimiters
	TOKEN_LPAREN, // (
	TOKEN_RPAREN, // )
	TOKEN_LBRACKET, // [
	TOKEN_RBRACKET, // ]
	TOKEN_LBRACE, // {
	TOKEN_RBRACE, // }

	// Punctuation & Operators
	TOKEN_COLON, // :
	TOKEN_SEMICOLON, // ;
	TOKEN_COMMA, // ,
	TOKEN_AT, // @
	TOKEN_AMPERSAND, // &
	TOKEN_ASTERISK, // *
	TOKEN_CARET, // ^
	TOKEN_DOT, // .
	TOKEN_DOTASTERISK, // .*
	TOKEN_EQUAL, // =
	TOKEN_EQUAL2, // ==
	TOKEN_BANG, // !
	TOKEN_BANGEQUAL, // !=
	TOKEN_LARROW, // <
	TOKEN_LARROW2, // <<
	TOKEN_LARROWEQUAL, // <=
	TOKEN_RARROW, // >
	TOKEN_RARROW2, // >>
	TOKEN_RARROWEQUAL, // >=
	TOKEN_MINUS, // -
	TOKEN_PERCENT, // %
	TOKEN_PIPE, // |
	TOKEN_PLUS, // +
	TOKEN_SLASH, // /

	// Keywords
	TOKEN_FUN,
	TOKEN_EXTERN,
	TOKEN_VAR,
	TOKEN_CONST,
	TOKEN_STRUCT,
	TOKEN_ENUM,
	TOKEN_RETURN,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_WHILE,
	TOKEN_FOR,
	TOKEN_PUB,
	TOKEN_ASYNC,
	TOKEN_AWAIT,
	TOKEN_IMPL,
	TOKEN_TRUE,
	TOKEN_FALSE,
	TOKEN_BREAK,
	TOKEN_CONTINUE,

	// Literals
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STRING,
	TOKEN_CHAR,

	// Specials
	TOKEN_IDENT,
	TOKEN_EOF,
} TokenType;

extern char* pretty_token_type[];

typedef struct {
	TokenType type;
	void* lexeme;
	L25_Range span;
} Token;

typedef struct {
	Token* items;
	size_t len;
	size_t cap;
} TokenStream;

typedef struct {
	enum {
		SLR_TOK,
		SLR_ERROR,
		SLR_PART_SUCCESS,
		SLR_WHITESPACE,
		SLR_COMMENT,
	} tag;

	union {
		Token tok;
		SwanError error;
		struct {
			SwanErrorStream errs;
			Token tok;
		} part_success;
	};
} SwanLexerRes;

typedef struct {
	const char* code;
	size_t codelen;
	size_t prev_idx;
	size_t idx;
} SwanLexer;

SwanLexer* snlxr_init(const char* code);
Token snlxr_token(SwanLexer* l, TokenType type, void* lexeme);
Token snlxr_simpletokn(SwanLexer* l, TokenType type);
char snlxr_peek(SwanLexer* l);
char snlxr_pop(SwanLexer* l);
SwanLexerRes snlxr_make_token(SwanLexer* l);
bool snlxr_push_tok(SwanLexer* l, TokenStream* ts, Token tok);
TokenStream* snlxr_lex(SwanLexer* l);
void snlxr_deinit(SwanLexer* l);

#define IS_AT_END(x, codelength) ((x) >= (codelength))

#define KW_FUN "fun"
#define KW_EXTERN "extern"
#define KW_VAR "var"
#define KW_CONST "const"
#define KW_STRUCT "struct"
#define KW_ENUM "enum"
#define KW_RETURN "return"
#define KW_IF "if"
#define KW_ELSE "else"
#define KW_WHILE "while"
#define KW_FOR "for"
#define KW_PUB "pub"
#define KW_ASYNC "async"
#define KW_AWAIT "await"
#define KW_IMPL "impl"
#define KW_TRUE "true"
#define KW_FALSE "false"
#define KW_BREAK "break"
#define KW_CONTINUE "continue"
/*
Reads a file from the given filepath, returns a c string allocated with malloc
and NULL in case an error occured.
*/
char* read_file(const char* filepath);

#endif // SWAN_LEXER_H
