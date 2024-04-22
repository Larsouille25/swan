#include <stdio.h>
#include <stdlib.h>

#include "l25.h"
#include "lexer.h"
#include "pretty.h"
#include "errors.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: swanc file...\n");
		exit(1);
	}

	char* buf = read_file(argv[1]);
	SwanLexer* l = snlxr_init(buf);

	TokenStream* ts = snlxr_lex(l);

	Token* t;
	for (size_t i = 0; i < ts->len; i++) {
		t = l25_vec_get(ts, i);
		dbg_token(stdout, t);
		printf("(%ld..%ld)", t->span.start, t->span.end);
		fputs(", ", stdout);
	}

	free(ts);
	ts = NULL;
	snlxr_deinit(l);

	free(buf);
	return 0;
}
