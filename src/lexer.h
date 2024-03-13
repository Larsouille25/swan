#ifndef SWAN_LEXER_H
#define SWAN_LEXER_H

typedef struct {
	char* code;
	int column;
	int line;
} SwanLexer;

/*
Reads a file from the given filepath, returns a c string allocated with malloc
and NULL in case an error occured.
*/
char* read_file(const char* filepath);

#endif // SWAN_LEXER_H
