#ifndef SWAN_ERRORS_H
#define SWAN_ERRORS_H

#include <stdio.h>
#include <stdlib.h>

#include "l25.h"
#include "config.h"

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
	size_t line;
	size_t col;
} SwanHalfPos;

typedef struct {
	SwanHalfPos start;
	SwanHalfPos end;
} SwanPosition;

void linecol(char* code, SwanHalfPos* dest, size_t idx);
void pos_from_range(char* code, SwanPosition* dest, size_t start, size_t end);

typedef struct {
	char* path;
	char* code;
} SwanLogCtx;

typedef struct {
	SwanPosition pos;
	ErrorType type;
	char* msg;
	SwanLogCtx* ctx;
} SwanError;

void display_error(FILE* stream, SwanError* err);
SwanError new_error(SwanLogCtx* ctx, ErrorType type, char* msg, L25_Range span);

typedef struct {
	SwanError* items;
	size_t len;
	size_t cap;
} SwanErrorStream;

void error_stream_init(SwanErrorStream* ses);
void error_stream_push(SwanErrorStream* ses, SwanError err);
void error_stream_deinit(SwanErrorStream* ses);
void error_stream_render(SwanErrorStream* ses, FILE* stream);

#endif // SWAN_ERRORS_H
