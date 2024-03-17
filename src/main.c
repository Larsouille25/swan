#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "pretty.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: swanc file...\n");
		exit(1);
	}

	char* buf = read_file(argv[1]);
	printf("%s", buf);

	SwanLexer lexer = {0};
	lexer_init(&lexer, "  ((");

	printf("\n");

	Token t = {0};
	while(t.type != TOKEN_EOF) {
		t = lexer_make_token(&lexer);
		dbg_token(stdout, &t);
		printf(", ");
	}
	printf("\n");

	free(buf);
	return 0;
}
