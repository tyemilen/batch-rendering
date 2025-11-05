#include "texture.h"

#include <stdlib.h>

#include "core.h"

void texture_manager_init(TextureManager *mgr) {
	mgr->textures = NULL;
	mgr->count = 0;
	mgr->capacity = 0;
}

Texture *texture_manager_add(TextureManager *mgr, int id, Image *img) {
	if (mgr->count >= mgr->capacity) {
		size_t new_cap = mgr->capacity ? mgr->capacity * 2 : 16;
		mgr->textures = realloc(mgr->textures, new_cap * sizeof(Texture *));
		mgr->capacity = new_cap;
	}

	Texture *texture = malloc(sizeof(Texture));

	texture->id = id;
	texture->data = 0;
	texture->width = img->width;
	texture->height = img->height;

	glGenTextures(1, &texture->data);

	glBindTexture(GL_TEXTURE_2D, texture->data);

	GLenum internalFormat =
		(img->format == GL_RGB || img->format == GL_BGR) ? GL_RGB8 : GL_RGBA8;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->width,
				 texture->height, 0, img->format, GL_UNSIGNED_BYTE,
				 img->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	mgr->textures[mgr->count] = texture;
	mgr->count++;

	return texture;
}

Texture *texture_manager_get(TextureManager *mgr, int id) {
	for (size_t i = 0; i < mgr->count; i++) {
		if (mgr->textures[i]->id == id) return mgr->textures[i];
	}
	return NULL;
}
