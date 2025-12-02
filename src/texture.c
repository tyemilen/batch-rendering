#include "texture.h"

#include <stdlib.h>

#include "core.h"
#include "ygl.h"

void texture_manager_init(TextureManager *mgr) {
	mgr->textures = 0;
	mgr->count = 0;
	mgr->capacity = 0;
}

Texture *texture_create(int id, Image *img, GLenum own_format) {
	Texture *texture = malloc(sizeof(Texture));

	texture->id = id;
	texture->data = 0;
	texture->width = img->width;
	texture->height = img->height;

	glGenTextures(1, &texture->data);
	glBindTexture(GL_TEXTURE_2D, texture->data);

	if (own_format) {
		glTexImage2D(GL_TEXTURE_2D, 0, own_format, texture->width,
					 texture->height, 0, own_format, GL_UNSIGNED_BYTE,
					 img->pixels);
	} else {
		GLenum internalFormat = (img->format == GL_RGB || img->format == GL_BGR)
									? GL_RGB8
									: GL_RGBA8;
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->width,
					 texture->height, 0, img->format, GL_UNSIGNED_BYTE,
					 img->pixels);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}

void texture_update(Texture *texture, Image *img) {
	glBindTexture(GL_TEXTURE_2D, texture->data);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->width, texture->height,
					img->format, GL_UNSIGNED_BYTE, img->pixels);
}

Texture *texture_manager_add(TextureManager *mgr, int id, Image *img) {
	if (mgr->count >= mgr->capacity) {
		size_t new_cap = mgr->capacity ? mgr->capacity * 2 : 16;
		mgr->textures = realloc(mgr->textures, new_cap * sizeof(Texture *));
		mgr->capacity = new_cap;
	}
	Texture *texture = texture_create(id, img, 0);

	mgr->textures[mgr->count] = texture;
	mgr->count++;

	return texture;
}

Texture *texture_manager_get(TextureManager *mgr, int id) {
	for (size_t i = 0; i < mgr->count; i++) {
		if (mgr->textures[i]->id == id) return mgr->textures[i];
	}
	return 0;
}

void texture_manager_update(TextureManager *mgr, int id, Image *img) {
	Texture *texture = texture_manager_get(mgr, id);

	if (!texture) return;

	texture_update(texture, img);
}
