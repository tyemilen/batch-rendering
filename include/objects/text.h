#ifndef OBJECT_TEXT_H_
#define OBJECT_TEXT_H_

#include <stddef.h>

#include "core.h"
#include "font.h"
#include "maler.h"

#include "objects/object.h"

typedef struct TextContainer {
	MalerContainer *container;

	float width;
	float height;

	size_t length;
} TextContainer;

typedef struct TextObject {
	ObjectBase base;

	Atlas *atlas;
	char *text;
	float font_size;

	float x, y, width, height;
	Color color;

	TextContainer *container;
} TextObject;

TextObject *yCreateText(char *text, float start_x, float start_y,
						float font_size, Color color, Atlas *atlas);
void yUpdateText(TextObject *obj);

#endif // OBJECT_RECT_H_
