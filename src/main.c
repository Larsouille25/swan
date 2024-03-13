#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: swanc file...\n");
		exit(1);
	}

	char* buf = read_file(argv[1]);
	printf("%s", buf);

	free(buf);
	return 0;
}
