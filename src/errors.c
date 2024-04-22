#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "errors.h"
#define L25_IMPL
#include "l25.h"
#include "config.h"

static void display_snippet(FILE* stream, SwanError* err, L25_Style err_style);

void snerr_display_error(FILE* stream, SwanError* err) {
	l25_set_style(stream, BOLD_STYLE);
	printf("%s:%ld:%ld: ", err->ctx->path, err->pos.start.line, err->pos.start.col);

	l25_reset_style(stream);
	L25_Style err_style = snerr_display_error_type(stream, err->type);

	fputs(err->msg, stream);
	fputc('\n', stream);

	display_snippet(stream, err, err_style);

	fputc('\n', stream);
	l25_reset_style(stream);
}

static void display_snippet(FILE* stream, SwanError* err, L25_Style err_style) {
	SwanPosition* pos = &err->pos;
	SwanHalfPos* lpos = &pos->start;
	int digits = l25_max(l25_digits(pos->start.line), l25_digits(pos->end.line));
	int padding = (SWAN_ERROR_DEFAULT_WIDTH - digits) / 2;
	int left_pad = 0;
	if (digits % 2 == 0) {
		left_pad = 1;
	}

	L25_StringSlice line_str = snerr_get_line(err->ctx->code, lpos->line - 1);

	size_t start, end;
	start = pos->start.col;
	if (pos->start.line == pos->end.line) {
		end = pos->end.col;
	} else {
		end = line_str.len;
	}

	fprintf(stream, "%*s%ld%*s| ", padding + left_pad, "", lpos->line, padding, "");
	L25_StringSlice first_part = {.str = line_str.str, .len = start - 2};
	l25_fputss(first_part, stream);

	l25_set_style_two(stream, BOLD_STYLE, err_style);
	L25_StringSlice colored_part = {.str = line_str.str + start - 1, .len = end - start};
	l25_fputss(colored_part, stream);

	l25_reset_style(stream);
	L25_StringSlice third_part = {.str = line_str.str + end, .len = line_str.len - end};
	l25_fputss(third_part, stream);

	fprintf(stream, "%*s%*s%*s|", padding + left_pad, "", digits, "", padding, "");
	l25_set_style_two(stream, BOLD_STYLE, err_style);
	fprintf(stream, "%*s^", (int)start, "");
	for (size_t i = 0; i < end - start; i++)
		fputc('~', stream);
	l25_reset_style(stream);
	fputc('\n', stream);

	if (pos->end.line - pos->start.line >= 2) {
		fprintf(stream, "%*s%*s%*s| ...\n", padding + left_pad - 1, "", digits, "...", padding - 1, "");
	}
	if (pos->start.line != pos->end.line) {
		line_str = snerr_get_line(err->ctx->code, pos->end.line - 1);

		start = 0;
		end = pos->end.col - 1;

		fprintf(stream, "%*s%ld%*s| ", padding + left_pad, "", pos->end.line, padding, "");

		l25_set_style_two(stream, BOLD_STYLE, err_style);
		L25_StringSlice colored_part = {.str = line_str.str, .len = end};
		l25_fputss(colored_part, stream);

		l25_reset_style(stream);
		L25_StringSlice third_part = {.str = line_str.str + end + 1, .len = line_str.len - end - 1};
		l25_fputss(third_part, stream);

		fprintf(stream, "%*s%*s%*s| ", padding + left_pad, "", digits, "", padding, "");
		l25_set_style_two(stream, BOLD_STYLE, err_style);
		for (size_t i = 0; i <= end - start; i++)
			fputc('~', stream);
		l25_reset_style(stream);
	}
}

L25_Style snerr_display_error_type(FILE* stream, ErrorType errty) {
	switch (errty) {
		case SNERR_ERROR:
			l25_set_style_two(stream, BOLD_STYLE, RED_FG_COLOR);
			fprintf(stream, "error: ");
			l25_reset_style(stream);
			return RED_FG_COLOR;
		case SNERR_WARNING:
			l25_set_style_two(stream, BOLD_STYLE, MAGENTA_FG_COLOR);
			fprintf(stream, "warning: ");
			l25_reset_style(stream);
			return MAGENTA_FG_COLOR;
  }
	printf("Unreachable in theory. %d", errty);
	exit(1);
}

SwanError snerr_new_error(SwanLogCtx* ctx, ErrorType type, char* msg, L25_Range range) {
	SwanError err = {
		.type = type,
		.msg = msg,
		.ctx = ctx,
	};
	assert(l25_check_range(&range));
	snerr_linecol(ctx->code, &err.pos.start, range.start);
	snerr_linecol(ctx->code, &err.pos.end, range.end);
	return err;
}

L25_StringSlice snerr_get_line(L25_StringSlice code, size_t line) {
	L25_StringSlice res = {0};

	// Check if the string is not null.
	if (!code.str && code.len == 0) {
		return res;
	}

	size_t curline = 0;
	const char* ptr = code.str;
	const char* start = NULL;

	// Find the start of the target line
	for (size_t i = 0; i <= code.len; i++) {
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

void snerr_errs_init(SwanErrorStream* ses) {
	l25_vec_init(ses, SWAN_ERROR_STREAM_DEFAULT_CAP);
}

void snerr_errs_push(SwanErrorStream* ses, SwanError err) {
	l25_vec_push(ses, err);
}

void snerr_errs_deinit(SwanErrorStream* ses) {
	l25_vec_deinit(ses);
}

void snerr_errs_render(SwanErrorStream* ses, FILE* stream) {
	for (size_t i = 0; i < ses->len; i++) {
		SwanError* err = l25_vec_get(ses, i);
		snerr_display_error(stream, err);
	}
}

void snerr_linecol(L25_StringSlice code, SwanHalfPos* dest, size_t idx) {
	dest->line = 1;
	dest->col = 0;

	const char* ptr = code.str;
	for (size_t i = 0; i <= code.len; i++) {
		if (idx == i)
			break;
		switch (*ptr) {
			case '\n':
				dest->col = 1;
				dest->line++;
				break;
			default:
				dest->col++;
		}
		ptr++;
	}
}
void pos_from_range(L25_StringSlice code, SwanPosition* dest, L25_Range range) {
	assert(l25_check_range(&range));

	snerr_linecol(code, &dest->start, range.start);
	snerr_linecol(code, &dest->end, range.end);
}

SwanLogCtx snerr_lctx_init(char* path, char* code) {
	return (SwanLogCtx){
		.path = path,
		.code = l25_cstr_to_ss(code),
	};
}
