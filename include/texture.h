#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
#include <SDL3/SDL_pixels.h>
#include <stddef.h>

typedef struct Texture {
	int id;
	GLuint data;
} Texture;

typedef struct TextureManager {
	Texture **textures;
	size_t count;
	size_t capacity;
} TextureManager;

void texture_manager_init(TextureManager *manager);
int texture_manager_add(TextureManager *mgr, int id, int width, int height,
						const unsigned char *pixels, SDL_PixelFormat format);

Texture *texture_manager_get(TextureManager *mgr, int id);

#endif // TEXTURE_H_
