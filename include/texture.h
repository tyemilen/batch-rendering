#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <stddef.h>

#include "core.h"

typedef struct Texture {
	int id;
	int width, height;
	GLuint data;
} Texture;

typedef struct TextureManager {
	Texture **textures;
	size_t count;
	size_t capacity;
} TextureManager;

void texture_manager_init(TextureManager *manager);

Texture *texture_create(int id, Image *img, GLenum own_format);

Texture *texture_manager_add(TextureManager *mgr, int id, Image *img);
Texture *texture_manager_get(TextureManager *mgr, int id);

#endif // TEXTURE_H_
