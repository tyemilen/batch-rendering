#ifndef OBJECT_GRID_H_
#define OBJECT_GRID_H_

#include "core.h"
#include "objects/object.h"

typedef struct GridObject {
	ObjectBase base;

	int visible;
	float x, y, width, height;
	Color color;
	Color bg_color;
	float cell_size;
} GridObject;

GridObject *yCreateGrid(float x, float y, float width, float height,
						float cell_size, Color color, Color bg_color);

void yUpdateGrid(GridObject *obj);

#endif // OBJECT_GRID_H_