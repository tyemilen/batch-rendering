#ifndef OBJECT_RECT_H_
#define OBJECT_RECT_H_

#include "core.h"
#include "objects/object.h"
#include "texture.h"

typedef struct RectObject {
	ObjectBase base;

	int visible;
	float x, y, width, height;
	Color color;
	Texture *texture;
} RectObject;

RectObject *yCreateRect(float x, float y, float width, float height,
						Color color, Texture *texture);
void yUpdateRect(RectObject *obj);

#endif // OBJECT_RECT_H_
