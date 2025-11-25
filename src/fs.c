#include "fs.h"

#include <stdio.h>
#include <stdlib.h>

YFile *fs_read_file(char *path) {
	FILE *file = fopen(path, "rb");
	if (!file) return NULL;

	if (fseek(file, 0, SEEK_END) != 0) {
		fclose(file);
		return NULL;
	}

	long file_size = ftell(file);
	if (file_size == -1) {
		fclose(file);
		return NULL;
	}

	rewind(file);

	unsigned char *buffer = (unsigned char *)malloc(file_size + 1);
	if (!buffer) {
		fclose(file);
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, file_size, file);
	if (bytes_read != (size_t)file_size) {
		free(buffer);
		fclose(file);
		return NULL;
	}

	buffer[bytes_read] = '\0';
	fclose(file);

	YFile *yfile = malloc(sizeof(YFile));

	yfile->data = buffer;
	yfile->size = bytes_read;

	return yfile;
}
