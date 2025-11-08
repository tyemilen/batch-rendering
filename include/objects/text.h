#ifndef OBJECT_TEXT_H_
#define OBJECT_TEXT_H_

#include <stddef.h>

#include "core.h"
#include "font.h"
#include "maler.h"

#include "graphics/text.h"

typedef struct TextContainer {
	MalerContainer *container;

	float width;
	float height;

	size_t length;
} TextContainer;

typedef struct TextObject {
	char *text;
	float font_size;

	float x, y, width, height;
	Color color;

	TextInstance *instance;
	TextContainer *container;
} TextObject;

TextObject create_text(char *text, float start_x, float start_y,
					   float font_size, Color color, Atlas *atlas);
void update_text(TextObject *obj, Atlas *atlas);

#endif // OBJECT_RECT_H_
