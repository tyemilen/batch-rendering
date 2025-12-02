#ifndef OBJECT_TEXTURE_H_
#define OBJECT_TEXTURE_H_

#include "core.h"
#include "objects/object.h"
#include "../texture.h"

typedef struct TextureObject {
	ObjectBase base;

	int visible;
	float x, y, width, height;
	float uv_x, uv_y, uv_width, uv_height;
	Color color;
	Texture *texture;
} TextureObject;

TextureObject *yCreateTexture(float x, float y, float width, float height, Texture *texture);

void yUpdateTexture(TextureObject *obj);

#endif // OBJECT_TEXTURE_H_
