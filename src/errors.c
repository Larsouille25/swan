#include "errors.h"
#include <stdio.h>

#define L25_IMPL
#include "l25.h"

void display_error(FILE* stream, SwanError* err) {
	set_style(stream, BOLD_STYLE);
	printf("%s:%d:%d: ", err->path, err->line, err->col);

	reset_style(stream);
	display_error_type(stream, err->type);

	fputs(err->msg, stream);

	reset_style(stream);
}

void display_error_type(FILE* stream, ErrorType errty) {
	switch (errty) {
		case SWER_ERROR:
			set_style_two(stream, BOLD_STYLE, RED_FG_COLOR);
			fprintf(stream, "error: ");
			break;
		case SWER_WARNING:
			set_style_two(stream, BOLD_STYLE, MAGENTA_FG_COLOR);
			fprintf(stream, "warning: ");
			break;
  }
	reset_style(stream);
}

L25_StringSlice get_line(const char* code, size_t line) {
	L25_StringSlice res = {0};

	// Check if the string is not null.
	if (!code && code[0] == '\0') {
		return res;
	}

	size_t curline = 0;
	const char* ptr = code;
	const char* start = NULL;

	// Find the start of the target line
	while (*ptr != '\0') {
		if (curline == line) {
			start = ptr;
			break;
		}
		if (*ptr == '\n') {
			curline++;
		}
		ptr++;
	}

	if (!start) {
		return res;
	}

	const char* end = start;
	while (*end != '\n' && *end != '\0') {
		end++;
	}

	res.len = end - start;
	res.str = start;

	return res;
}
