#include "objects/texture.h"

#include <stdlib.h>

#include "core.h"
#include "texture.h"
#include "yta.h"

#include "objects/object.h"
#include "graphics/texture.h"

TextureObject *yCreateTexture(float x, float y, float width, float height, Texture *texture) {
	TextureObject *obj = malloc(sizeof(TextureObject));
	*obj = (TextureObject){0};

	obj->x = x;
	obj->y = y;
	obj->width = width;
	obj->height = height;

	obj->uv_x = 0;
	obj->uv_y = 0;
	obj->uv_width = width;
	obj->uv_height = height;

	obj->color = COLOR_MAGENTA;
	obj->texture = texture;
	obj->visible = 1;
	obj->base.visible = 1;

	TextureInstance *instance = malloc(sizeof(TextureInstance));
	*instance = (TextureInstance){
		.box = {x, y, width, height},
		.color = {obj->color.r / 255.f, obj->color.g / 255.f, obj->color.b / 255.f, obj->color.a},
		.uv = {obj->uv_x, obj->uv_y, obj->uv_width, obj->uv_height}
	};

	obj->base.instance = instance;
	obj->base.element =
		YtaCreate(instance, sizeof(TextureInstance), SHADER_TEXTURE, texture);

	obj->base.update = (obj_update_fn)yUpdateTexture;

	YtaRegisterObject(&obj->base);
	return obj;
}

void yUpdateTexture(TextureObject *obj) {
	TextureInstance *instance = obj->base.instance;

	instance->box[0] = obj->x;
	instance->box[1] = obj->y;
	instance->box[2] = obj->width;
	instance->box[3] = obj->height;

	instance->uv[0] = obj->uv_x / obj->texture->width;
	instance->uv[1] = obj->uv_y / obj->texture->height;
	instance->uv[2] = (obj->uv_x + obj->uv_width) / obj->texture->width;
	instance->uv[3] = (obj->uv_y + obj->height) / obj->texture->height;

	MAP_COLOR(instance->color, obj->color);

	obj->base.element->texture = obj->texture;
	obj->base.element->visible = obj->visible;
}
