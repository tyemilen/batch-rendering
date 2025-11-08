#include "objects/rect.h"

#include <stdlib.h>

#include "core.h"
#include "texture.h"
#include "yta.h"

#include "graphics/rect.h"

RectObject create_rect(float x, float y, float width, float height, Color color,
					   Texture *texture) {
	RectObject obj = {0};

	obj.x = x;
	obj.y = y;
	obj.width = width;
	obj.height = height;

	obj.color = color;
	obj.texture = texture;

	obj.visible = 1;

	RectInstance *instance = malloc(sizeof(RectInstance));
	*instance = (RectInstance){
		{obj.x, obj.y, obj.width, obj.height},
		{color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a}};
	obj.instance = instance;
	obj.element =
		yta_create(instance, sizeof(RectInstance), SHADER_RECT, texture);

	return obj;
}

void update_rect(RectObject *obj) {
	obj->instance->box[0] = obj->x;
	obj->instance->box[1] = obj->y;
	obj->instance->box[2] = obj->width;
	obj->instance->box[3] = obj->height;

	obj->instance->color[0] = obj->color.r / 255;
	obj->instance->color[1] = obj->color.g / 255;
	obj->instance->color[2] = obj->color.b / 255;
	obj->instance->color[3] = obj->color.a;

	obj->element->texture = obj->texture;

	obj->element->visible = obj->visible;
}
