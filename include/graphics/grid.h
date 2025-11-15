#ifndef GRAPHIC_GRID_H_
#define GRAPHIC_GRID_H_

#include "maler.h"
#include "ygl.h"

#define SHADER_GRID 0x3

typedef struct {
	float box[4];
	float color[4];
	float bg_color[4];
	float cell_size;
} GridInstance;

GLuint shader_grid_get(void);
void shader_grid_bind(MalerContainer *);

#endif // GRAPHICS_GRID_H_
