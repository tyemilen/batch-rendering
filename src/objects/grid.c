#include "objects/grid.h"

#include <stdlib.h>

#include "core.h"
#include "yta.h"

#include "graphics/grid.h"
#include "objects/object.h"

GridObject *yCreateGrid(float x, float y, float width, float height,
						float cell_size, Color color, Color bg_color) {
	GridObject *obj = malloc(sizeof(GridObject));
	*obj = (GridObject){0};

	obj->x = x;
	obj->y = y;
	obj->width = width;
	obj->height = height;
	obj->cell_size = cell_size;
	obj->color = color;
	obj->bg_color = bg_color;
	obj->visible = 1;

	GridInstance *instance = malloc(sizeof(GridInstance));
	*instance = (GridInstance){
		.box = {x, y, width, height},
		.color = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a},
		.bg_color = {bg_color.r / 255.f, bg_color.g / 255.f, bg_color.b / 255.f,
					 color.a},
		.cell_size = cell_size};

	obj->base.instance = instance;
	obj->base.element =
		YtaCreate(instance, sizeof(GridInstance), SHADER_GRID, 0);

	obj->base.update = (obj_update_fn)yUpdateGrid;

	YtaRegisterObject(&obj->base);

	return obj;
}

void yUpdateGrid(GridObject *obj) {
	GridInstance *instance = obj->base.instance;

	instance->box[0] = obj->x;
	instance->box[1] = obj->y;
	instance->box[2] = obj->width;
	instance->box[3] = obj->height;

	MAP_COLOR(instance->color, obj->color);
	MAP_COLOR(instance->bg_color, obj->bg_color);

	instance->cell_size = obj->cell_size;

	obj->base.element->visible = obj->visible;
}