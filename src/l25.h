/*
Larsouille25's c library, "l25.h", it's a very simple one file library
containing useful things, like colors, strings, string manipulation,
dynamic arrays and maybe allocators.
*/

#ifndef L25_H_
#define L25_H_

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//       COLOR & STYLE

typedef enum  {
	RESET_STYLE = 0,
	BOLD_STYLE = 1,
	ITALIC_STYLE = 3,
	UNDERLINE_STYLE = 4,

	BLACK_FG_COLOR = 30,
	RED_FG_COLOR = 31,
	GREEN_FG_COLOR = 32,
	YELLOW_FG_COLOR = 33,
	BLUE_FG_COLOR = 34,
	MAGENTA_FG_COLOR = 35,
	CYAN_FG_COLOR = 36,
	WHITE_FG_COLOR = 37,

	BLACK_BG_COLOR = 40,
	RED_BG_COLOR = 41,
	GREEN_BG_COLOR = 42,
	YELLOW_BG_COLOR = 43,
	BLUE_BG_COLOR = 44,
	MAGENTA_BG_COLOR = 45,
	CYAN_BG_COLOR = 46,
	WHITE_BG_COLOR = 47,
} L25_Style;

#define l25_brighten(color) ((color) + 60)

#define l25_set_style_two(stream, lhs, rhs) fprintf(stream, "\x1B[%d;%dm", lhs, rhs);
#define l25_set_style(stream, style) fprintf(stream, "\x1B[%dm", style);
#define l25_reset_style(stream) fprintf(stream, "\x1B[0;0m");

//       STRING SLICE

typedef enum {
	L25_OK,
	L25_ERROR,
	L25_ALLOC_ERR,
	L25_BOUNDARY_ERR,
} L25_Res;

typedef struct {
	const char* str;
	size_t len;
} L25_StringSlice;

// Converts a null terminated string into a string slice.
L25_StringSlice l25_cstr_to_ss(char* c_str);
// Write to the stream the string slice.
void l25_fputss(L25_StringSlice ss, FILE* stream);

//       DYNAMIC STRING

typedef struct {
	char *str;
	size_t len;
	size_t cap;
} L25_DynString;

// Initiate a growable string and allocating the capacity passed in arg.
L25_Res l25_ds_init(L25_DynString* ds, size_t cap);
// Push a character to the string, allocates more memory if necessary.
L25_Res l25_ds_push(L25_DynString* ds, char c);
// Appends the string slice to the dynamic string and allocates memory if
// necesseray.
L25_Res l25_ds_append_ss(L25_DynString* ds, L25_StringSlice ss);
// Converts a dynamic string into a String Slice.
L25_Res l25_ds_to_ss(L25_DynString* ds, L25_StringSlice* ss);
// Take a slice of the dynamic string and return a string slice of it.
L25_StringSlice l25_ds_slice(L25_DynString* ds, size_t start, size_t end);
// Frees the memory and put zeros in all fields.
void l25_ds_free(L25_DynString* ds);
// Converts a null terminated string into a dynamic string.
L25_Res l25_ds_from_cstr(L25_DynString* ds, char* cstr);
// Write to the stream the string slice.
void l25_fputds(L25_DynString* ds, FILE* stream);

//       DYNAMIC ARRAY, aka VECTOR or VEC

// Macro to initialize a Vector type.
#define l25_vec_init(vec, vecCap) \
		do { \
			(vec)->items = malloc((vecCap) * sizeof(*(vec)->items)); \
			(vec)->len = 0; \
			(vec)->cap = (vecCap); \
		} while (0)

// Macro to push an item to a Vector type.
#define l25_vec_push(vec, item) \
		do { \
			if ((vec)->len >= (vec)->cap) { \
				(vec)->cap = ((vec)->cap == 0) ? 1 : (vec)->cap * 2; \
				(vec)->items = realloc((vec)->items, (vec)->cap * sizeof(*(vec)->items)); \
			} \
			(vec)->items[(vec)->len++] = (item); \
		} while (0)

// Macro to deinitialize a Vector type.
#define l25_vec_deinit(vec) \
		do { \
			(vec)->len = 0; \
			(vec)->cap = 0; \
			free((vec)->items); \
			(vec)->items = NULL; \
		} while (0)

#define l25_vec_get(vec, idx) \
		(assert(idx < (vec)->len && "Index out of bounds"), &(vec)->items[(idx)])

//       RANGES

typedef struct {
	size_t start, end;
} L25_Range;

bool l25_check_range(L25_Range* range);

//       MISCELLANEOUS

// Takes an integer as argument and returns how many digits you need to write
// in base 10.
long int l25_digits(long int num);

#define l25_max(a, b) \
		({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; \
		})

#define l25_min(a, b) \
		({ __typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; \
		})

#endif // L25_H_

#ifdef L25_IMPL

L25_StringSlice l25_cstr_to_ss(char* cstr) {
	return (L25_StringSlice){.str = cstr, .len = strlen(cstr)};
}

void l25_fputss(L25_StringSlice ss, FILE* stream) {
	for (size_t i = 0; i <= ss.len; i++) {
		fputc(ss.str[i], stream);
	}
}

void l25_fputds(L25_DynString* ds, FILE* stream) {
	for (size_t i = 0; i <= ds->len; i++) {
		fputc(ds->str[i], stream);
	}
}

L25_Res l25_ds_init(L25_DynString* ds, size_t cap) {
	ds->str = (char*)malloc(cap * sizeof(char));
	if (!ds->str) {
		return L25_ALLOC_ERR;
	}
	ds->len = 0;
	ds->cap = cap;
	return L25_OK;
}

L25_Res l25_ds_push(L25_DynString* ds, char c) {
	if (ds->len >= ds->cap) {
		ds->cap = ds->cap * 2;
		ds->str = (char*)realloc(ds->str, ds->cap * sizeof(char));
		if (!ds->str) {
			return L25_ALLOC_ERR;
		}
	}
	ds->str[ds->len++] = c;
	return L25_OK;
}

L25_Res l25_ds_append_ss(L25_DynString* ds, L25_StringSlice ss) {
	if (ds->cap - ds->len > ss.len) {
		ds->cap = ds->len + ss.len;
		ds->str = (char*)realloc(ds->str, ds->cap * sizeof(char));
		if (!ds->str) {
			return L25_ALLOC_ERR;
		}
	}
	memcpy(ds->str + ds->len, ss.str, ss.len);
	ds->len += ss.len;
	return L25_OK;
}

L25_Res l25_ds_from_cstr(L25_DynString* ds, char* cstr) {
	ds->len = strlen(cstr);
	ds->cap = ds->len;
	ds->str = (char*) malloc(ds->cap * sizeof(char));
	if (!ds->str) {
		return L25_ALLOC_ERR;
	}
	memcpy(ds->str, cstr, ds->cap);
	return L25_OK;
}

void l25_ds_free(L25_DynString* ds) {
	ds->len = 0;
	ds->cap = 0;
	free(ds->str);
	ds->str = NULL;
}

long int l25_digits(long int num) {
	long int count = 0;
	if (num == 0)
		return 1;
	if (num < 0)
		num = -num;
	while (num != 0) {
		num /= 10;
		count++;
	}
	return count;
}

bool l25_check_range(L25_Range* range) {
	return range->start < range->end;
}

#endif // L25_IMPL
