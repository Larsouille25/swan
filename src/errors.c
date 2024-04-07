#include "errors.h"
#include <stdio.h>

#define L25_IMPL
#include "l25.h"

void display_error(FILE* stream, SwanError* err) {
	l25_set_style(stream, BOLD_STYLE);
	printf("%s:%d:%d: ", err->path, err->line, err->col);

	l25_reset_style(stream);
	L25_Style err_style = display_error_type(stream, err->type);

	fputs(err->msg, stream);
	fputc('\n', stream);

	int digits = l25_digits(err->line);
	int width = 7;
	int padding = (width - digits) / 2;
	int left_pad = 0;
	if (digits % 2 == 0) {
		left_pad = 1;
	}
	fprintf(stream, "%*s%d%*s| ", padding + left_pad, "", err->line, padding, "");

	L25_StringSlice line_str = get_line(err->code, err->line - 1);

	L25_StringSlice first_part = {.str = line_str.str, .len = err->cur_start - 2};
	l25_fputss(first_part, stream);

	l25_set_style_two(stream, BOLD_STYLE, err_style);
	L25_StringSlice colored_part = {.str = line_str.str + err->cur_start - 1, .len = err->cur_end - err->cur_start};
	l25_fputss(colored_part, stream);

	l25_reset_style(stream);
	L25_StringSlice third_part = {.str = line_str.str + err->cur_end, .len = line_str.len - err->cur_end - 1};
	l25_fputss(third_part, stream);

	fputc('\n', stream);

	fprintf(stream, "%*s%*s%*s|", padding + left_pad, "", digits, "", padding, "");
	fprintf(stream, "%*s", err->cur_start, "");
	l25_set_style_two(stream, BOLD_STYLE, err_style);
	fprintf(stream, "^");
	for (int i = 0; i < err->cur_end - err->cur_start; i++)
		fputc('~', stream);

	fputc('\n', stream);
	l25_reset_style(stream);
}

L25_Style display_error_type(FILE* stream, ErrorType errty) {
	switch (errty) {
		case SWER_ERROR:
			l25_set_style_two(stream, BOLD_STYLE, RED_FG_COLOR);
			fprintf(stream, "error: ");
			l25_reset_style(stream);
			return RED_FG_COLOR;
		case SWER_WARNING:
			l25_set_style_two(stream, BOLD_STYLE, MAGENTA_FG_COLOR);
			fprintf(stream, "warning: ");
			l25_reset_style(stream);
			return MAGENTA_FG_COLOR;
  }
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
