#include "objects/text.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "font.h"
#include "maler.h"
#include "yta.h"

#include "graphics/text.h"

TextContainer *create_text_container(Atlas *atlas, float color[4],
									 const char *text, float start_x,
									 float start_y, float font_size) {
	size_t len = strlen(text);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&atlas->font, &ascent, &descent, &lineGap);

	float base_scale =
		stbtt_ScaleForPixelHeight(&atlas->font, atlas->font_size);
	float scale = font_size / atlas->font_size;

	float cursor_x = start_x;
	float cursor_y = start_y;

	MalerContainer *container = yta_create_container(SHADER_TEXT, -1);
	float total_width = 0;
	float height = 0;

	for (size_t i = 0; i < len; ++i) {
		unsigned char c = text[i];

		stbtt_aligned_quad q = atlas->quads[c];

		if (c == '\n') {
			cursor_x = start_x;
			cursor_y += font_size;
			continue;
		}

		TextInstance *instance = calloc(1, sizeof(TextInstance));

		instance->box[0] = cursor_x + q.x0 * scale;
		instance->box[1] = cursor_y + (ascent * base_scale + q.y0) * scale;
		instance->box[2] = (q.x1 - q.x0) * scale;
		instance->box[3] = (q.y1 - q.y0) * scale;

		total_width += instance->box[2];
		height = fmax(height, instance->box[3]);

		instance->color[0] = color[0];
		instance->color[1] = color[1];
		instance->color[2] = color[2];
		instance->color[3] = color[3];

		instance->uv[0] = q.s0;
		instance->uv[1] = q.t0;
		instance->uv[2] = q.s1;
		instance->uv[3] = q.t1;

		yta_create_ex(instance, sizeof(TextInstance), SHADER_TEXT,
					  atlas->texture, container);

		cursor_x += atlas->chars[c].xadvance * scale;
	}

	TextContainer *result = malloc(sizeof(TextContainer));

	result->container = container;
	result->width = total_width;
	result->height = height;
	result->length = len;

	return result;
}

TextObject create_text(char *text, float start_x, float start_y,
					   float font_size, Color color, Atlas *atlas) {
	TextObject obj = {0};

	obj.text = text;

	obj.x = start_x;
	obj.y = start_y;
	obj.font_size = font_size;

	obj.color = color;

	TextContainer *container = create_text_container(
		atlas, (float[4]){color.r / 255, color.g / 255, color.b / 255, color.a},
		text, obj.x, obj.y, obj.font_size);

	obj.width = container->width;	// short
	obj.height = container->height; // cut

	obj.container = container;

	return obj;
}

void update_text(TextObject *obj, Atlas *atlas) {
	size_t len = strlen(obj->text);
	TextContainer *tc = obj->container;

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&atlas->font, &ascent, &descent, &lineGap);

	float base_scale =
		stbtt_ScaleForPixelHeight(&atlas->font, atlas->font_size);
	float scale = obj->font_size / atlas->font_size;
	float cursor_x = obj->x;
	float cursor_y = obj->y;
	float total_height = obj->font_size;
	float total_width = 0;

	for (size_t i = 0; i < len; ++i) {
		unsigned char c = obj->text[i];

		if (c == '\n') {
			cursor_x = obj->x;
			cursor_y += obj->font_size;
			total_height += obj->font_size;
			continue;
		}

		stbtt_aligned_quad q = atlas->quads[c];

		TextInstance *instance;
		if (i < tc->length) {
			instance = (TextInstance *)tc->container->elements[i]->instance;
		} else {
			instance = calloc(1, sizeof(TextInstance));
			yta_create_ex(instance, sizeof(TextInstance), SHADER_TEXT,
						  atlas->texture, tc->container);
			++tc->length;
		}

		instance->box[0] = cursor_x + q.x0 * scale;
		instance->box[1] = cursor_y + (ascent * base_scale + q.y0) * scale;
		instance->box[2] = (q.x1 - q.x0) * scale;
		instance->box[3] = (q.y1 - q.y0) * scale;

		total_width += instance->box[2];

		instance->color[0] = obj->color.r / 255.0f;
		instance->color[1] = obj->color.g / 255.0f;
		instance->color[2] = obj->color.b / 255.0f;
		instance->color[3] = obj->color.a;

		instance->uv[0] = q.s0;
		instance->uv[1] = q.t0;
		instance->uv[2] = q.s1;
		instance->uv[3] = q.t1;

		cursor_x += atlas->chars[c].xadvance * scale;
	}

	if (len < tc->length) {
		for (size_t i = len; i < tc->length; ++i) {
			yta_destroy_element(tc->container, i);
		}
		tc->container->element_count = len;
		tc->container->elements =
			realloc(tc->container->elements, len * sizeof(MalerElement *));
	}

	tc->length = len;
	tc->height = total_height;
	tc->width = total_width;

	obj->width = tc->width;
	obj->height = tc->height;
}
