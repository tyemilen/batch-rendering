#ifndef GRAPHICS_TEXT_H_
#define GRAPHICS_TEXT_H_

#include "core.h"
#include "maler.h"

#define SHADER_TEXT 0x2

typedef struct {
	float box[4];
	float uv[4];
	float color[4];
} TextInstance;

GLuint shader_text_get(void);
void shader_text_bind(MalerContainer *);

#endif // GRAPHICS_TEXT_H_
