#include "bglib.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "font.h"
#include "fs.h"
#include "log.h"
#include "maler.h"

#include "graphics/rect.h"
#include "graphics/text.h"

float lerp(float a, float b, float t) { return a + (b - a) * t; }

MalerContainer *add_text(Atlas *atlas, float color[4], const char *text,
						 float start_x, float start_y, float font_size) {
	size_t len = strlen(text);

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&atlas->font, &ascent, &descent, &lineGap);

	float base_scale =
		stbtt_ScaleForPixelHeight(&atlas->font, atlas->font_size);
	float scale = font_size / atlas->font_size;

	float cursor_x = start_x;
	float cursor_y = start_y;

	MalerContainer *container = bg_create_container(SHADER_TEXT, -1);
	TextInstance *instances = calloc(len, sizeof(TextInstance));

	for (size_t i = 0; i < len; ++i) {
		unsigned char c = text[i];

		stbtt_aligned_quad q = atlas->quads[c];

		if (c == '\n') {
			cursor_x = start_x;
			cursor_y += font_size;
			continue;
		}

		TextInstance *instance = &instances[i];

		instance->box[0] = cursor_x + q.x0 * scale;
		instance->box[1] = cursor_y + (ascent * base_scale + q.y0) * scale;
		instance->box[2] = (q.x1 - q.x0) * scale;
		instance->box[3] = (q.y1 - q.y0) * scale;

		instance->color[0] = color[0];
		instance->color[1] = color[1];
		instance->color[2] = color[2];
		instance->color[3] = color[3];

		instance->uv[0] = q.s0;
		instance->uv[1] = q.t0;
		instance->uv[2] = q.s1;
		instance->uv[3] = q.t1;

		bg_create_ex(&instances[i], sizeof(TextInstance), SHADER_TEXT,
					 atlas->texture, container);

		cursor_x += atlas->chars[c].xadvance * scale;
	}

	return container;
}

int main(void) {
#ifdef PLATFORM_SDL3
	bg_init("SDL3 window", 800, 600);
#endif
#ifdef PLATFORM_WIN32
	bg_init("WIN32 window", 800, 600);
#endif

	bg_register_shader(SHADER_RECT, shader_rect_get(), shader_rect_bind);
	bg_register_shader(SHADER_TEXT, shader_text_get(), shader_text_bind);

	Atlas atlas =
		font_create_atlas((unsigned char *)fs_read_file("font.ttf"), 64, 1024);

	Window window = bg_get_window();

	RectInstance base_instance = {{0, window.height - 20, 20, 20},
								  {0.0, 1.0, 0.0, 1.0}};
	RectInstance rects[100];
	for (int i = 0; i < 30; ++i) {
		rects[i] = base_instance;

		base_instance.box[0] += i * 2;
		base_instance.color[2] += 0.09;

		bg_create(&rects[i], sizeof(RectInstance), SHADER_RECT, 0);
	}

	const char *val = "hello please dont break me";
	MalerContainer *text =
		add_text(&atlas, (float[4]){1, 1, 1, 1}, val, 200, 400, 32);

	while (!bg_should_close()) {
		Mouse mouse = bg_get_mouse();
		float dt = bg_delta();
		float t = bg_get_time();

		if (mouse.left_button.pressed) {
			for (size_t i = 0; i < text->element_count; ++i) {
				TextInstance *instance = (void *)text->elements[i]->instance;

				float angle = t * 0.5f + i * 1.3f;
				float radius = 190.0f * sinf(t * 2.0f + i);
				float x = sinf(angle * 1.5f + i) * radius;
				float y = cosf(angle * 2.0f + i) * radius;

				instance->box[0] =
					lerp(instance->box[0], mouse.x + x, 5.0f * dt);
				instance->box[1] =
					lerp(instance->box[1], mouse.y + y, 5.0f * dt);
			}
		}

		LOG_INFO("FPS: %f", 1.0 / dt);
	}

	return 0;
}
