#ifndef GRAPHIC_RECT_H_
#define GRAPHIC_RECT_H_

#include "core.h"
#include "maler.h"
#define SHADER_RECT 0x1

typedef struct {
	float box[4];
	float color[4];
} RectInstance;

typedef struct TextObject {
	float x, y, width, height;
	Color color;

	RectInstance instance;
} TextObject;

GLuint shader_rect_get(void);
void shader_rect_bind(MalerContainer *);

#endif // GRAPHIC_RECT_H_
