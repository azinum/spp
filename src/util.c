// util.c

#include "common.h"
#include "memory.h"
#include "util.h"

char* read_file(const char* path) {
	u32 buffer_size = 0;
  u32 read_size = 0;
	FILE* file = fopen(path, "rb");
	if (file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	buffer_size = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(sizeof(char) * (buffer_size + 1));

	read_size = fread(buffer, sizeof(char), buffer_size, file);
	buffer[read_size] = '\0';

	fclose(file);
	return buffer;
}
