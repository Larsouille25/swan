#ifndef SWAN_ERRORS_H
#define SWAN_ERRORS_H

#include "l25.h"
#include <stdio.h>

#ifdef _WIN32
#error "The compiler doesn't support windows for now."
#endif

typedef enum {
	SWER_ERROR,
	SWER_WARNING,
} ErrorType;

L25_Style display_error_type(FILE* file, ErrorType errty);
L25_StringSlice get_line(const char* code, size_t line);

typedef struct {
	char* path;
	int line;
	int col;
	ErrorType type;
	char* msg;
	const char* code; // points to the entire file
	short cur_start;
	short cur_end;
} SwanError;

void display_error(FILE* stream, SwanError* err);

#endif // SWAN_ERRORS_H
