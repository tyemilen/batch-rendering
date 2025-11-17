#include "objects/rect.h"

#include <stdlib.h>

#include "core.h"
#include "texture.h"
#include "yta.h"
#include "log.h"

#include "objects/object.h"
#include "graphics/rect.h"

RectObject *yCreateRect(float x, float y, float width, float height,
						Color color, Texture *texture) {
	RectObject *obj = malloc(sizeof(RectObject));
	*obj = (RectObject){0};

	obj->x = x;
	obj->y = y;
	obj->width = width;
	obj->height = height;

	obj->color = color;
	obj->texture = texture;
	obj->visible = 1;
	obj->base.visible = 1;

	RectInstance *instance = malloc(sizeof(RectInstance));
	*instance = (RectInstance){
		.box = {x, y, width, height},
		.color = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a}};

	obj->base.instance = instance;
	obj->base.element =
		YtaCreate(instance, sizeof(RectInstance), SHADER_RECT, texture);

	obj->base.update = (obj_update_fn)yUpdateRect;

	YtaRegisterObject(&obj->base);
	return obj;
}

void yUpdateRect(RectObject *obj) {
	RectInstance *instance = obj->base.instance;

	instance->box[0] = obj->x;
	instance->box[1] = obj->y;
	instance->box[2] = obj->width;
	instance->box[3] = obj->height;

	MAP_COLOR(instance->color, obj->color);

	obj->base.element->texture = obj->texture;
	obj->base.element->visible = obj->visible;
}
