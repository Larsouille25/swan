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
L25_StringSlice get_line(L25_StringSlice code, size_t line);

typedef struct {
	size_t line;
	size_t col;
} SwanHalfPos;

typedef struct {
	SwanHalfPos start;
	SwanHalfPos end;
} SwanPosition;

void linecol(L25_StringSlice code, SwanHalfPos* dest, size_t idx);
void pos_from_range(L25_StringSlice code, SwanPosition* dest, L25_Range range);

typedef struct {
	char* path;
	L25_StringSlice code;
} SwanLogCtx;
SwanLogCtx swan_lctx_init(char* path, char* code);

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
