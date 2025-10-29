#include "texture.h"
#include "log.h"

#include <SDL3/SDL_pixels.h>
#include <stdlib.h>

void texture_manager_init(TextureManager *mgr) {
	mgr->textures = NULL;
	mgr->count = 0;
	mgr->capacity = 0;
}
int texture_manager_add(TextureManager *mgr, int id, int width, int height,
						const unsigned char *pixels, SDL_PixelFormat format) {
	if (mgr->count >= mgr->capacity) {
		size_t new_cap = mgr->capacity ? mgr->capacity * 2 : 16;
		mgr->textures = realloc(mgr->textures, new_cap * sizeof(Texture*));
		mgr->capacity = new_cap;
	}

	Texture *texture = malloc(sizeof(Texture));

	texture->id = id;
	texture->data = 0;

	glGenTextures(1, &texture->data);

	glBindTexture(GL_TEXTURE_2D, texture->data);

	GLenum gl_format;
	switch (format) {
		case SDL_PIXELFORMAT_RGBA32: gl_format = GL_RGBA; break;
		case SDL_PIXELFORMAT_ARGB32: gl_format = GL_BGRA; break;
		case SDL_PIXELFORMAT_RGB24:  gl_format = GL_RGB;  break;
		case SDL_PIXELFORMAT_BGR24:  gl_format = GL_BGR;  break;
		default:
			LOG_ERROR("Unsupported SDL pixel format");
			return -1;
	}
	GLenum internalFormat = (gl_format == GL_RGB || gl_format == GL_BGR) ? GL_RGB8 : GL_RGBA8;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
				gl_format, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	mgr->textures[mgr->count] = texture;
	mgr->count++;

	return id;
}

Texture *texture_manager_get(TextureManager *mgr, int id) {
	for (size_t i = 0; i < mgr->count; i++) {
		if (mgr->textures[i]->id == id) return mgr->textures[i];
	}
	return NULL;
}
