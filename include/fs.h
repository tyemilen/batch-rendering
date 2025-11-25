#ifndef FS_H_
#define FS_H_

#include <stddef.h>

struct YFile {
	unsigned char *data;
	size_t size;
} typedef YFile;

YFile *fs_read_file(char *path);

#endif // FS_H_
