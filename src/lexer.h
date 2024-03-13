#ifndef SWAN_LEXER_H
#define SWAN_LEXER_H

#include <stdbool.h>
#include <stddef.h>

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
	TOKEN_FN,
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
	TOKEN_PACKAGE,
	TOKEN_IMPORT,

	// Literals
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STRING,
	TOKEN_CHAR,

	// Specials
	TOKEN_IDENT,
	TOKEN_EOF,
} TokenType;

typedef struct {
	TokenType type;
	void* lexeme;
	int column;
	int line;
} Token;

typedef struct {
	const char* code;
	size_t lenght;
	int current;
	int line;
} SwanLexer;

void lexer_init(SwanLexer* l, const char* code);
void lexer_skip_whitespace(SwanLexer* l);
Token lexer_token(SwanLexer* l, TokenType type, void* lexeme);
Token lexer_token_simple(SwanLexer* l, TokenType type);
Token lexer_make_token(SwanLexer* l);

#define IS_AT_END(x, codelenght) (((size_t)x) >= (codelenght))

#define KW_FN "fn"
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
#define KW_PACKAGE "package"
#define KW_IMPORT "import"
/*
Reads a file from the given filepath, returns a c string allocated with malloc
and NULL in case an error occured.
*/
char* read_file(const char* filepath);

#endif // SWAN_LEXER_H
