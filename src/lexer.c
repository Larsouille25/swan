#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_file(const char* filepath) {
	size_t lenght = 0, readBytes = 0;
	char* buf = NULL;
	FILE* fp = fopen(filepath, "rb");

	if (fp == NULL) {
		printf("Cannot open file: %s\n", filepath);
		return NULL;
	}

	if (!fseek(fp, 0, SEEK_END)) {
		lenght = ftell(fp);
		rewind(fp);
	}

	buf = (char*)malloc(lenght + 1);
	if (buf == NULL) {
		printf("Cannot allocate memory for file: %s\n", filepath);
		return NULL;
	}

	memset(buf, 0, lenght + 1);
	readBytes = fread(buf, 1, lenght, fp);
	if (readBytes < lenght) {
		printf("Cannot read file: %s; read = %ld, len = %ld\n", filepath, readBytes, lenght);
		return NULL;
	}

	if (fclose(fp)) {
		printf("Cannot close the file descriptor\n");
		free(buf);
		return NULL;
	}

	return buf;
}
