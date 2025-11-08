#ifndef OBJECT_RECT_H_
#define OBJECT_RECT_H_

#include "core.h"
#include "maler.h"
#include "texture.h"

#include "graphics/rect.h"

typedef struct RectObject {
	float x, y, width, height;
	Color color;
	Texture *texture;

	int visible;

	RectInstance *instance;
	MalerElement *element;
} RectObject;

RectObject create_rect(float x, float y, float width, float height, Color color,
					   Texture *texture);
void update_rect(RectObject *obj);

#endif // OBJECT_RECT_H_
