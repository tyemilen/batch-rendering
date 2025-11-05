#include "bglib.h"

#include <stdbool.h>
#include <stddef.h>

#include "core.h"
#include "graphics/rect.h"
#include "log.h"
#include "maler.h"
#include "texture.h"

float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

int main(void) {
	bg_init("cool window", 800, 600);
	bg_register_shader(SHADER_RECT, shader_rect_get(), shader_rect_bind);

	Texture *cat_texture = bg_load_texture(1, "cats.jpg");
	Texture *frog_texture = bg_load_texture(2, "frog.png");

	RectInstance r1 = {{0, 0, 220, 220}, {1.0, 0.0, 1.0, 1.0}};
	bg_create(&r1, sizeof(RectInstance), SHADER_RECT, cat_texture);

	RectInstance r2 = {{50, 0, 20, 20}, {1.0, 0.0, 0.0, 1.0}};
	MalerElement* rect = bg_create(&r2, sizeof(RectInstance), SHADER_RECT, frog_texture);

	RectInstance base_instance = {{150, 10, 20, 20}, {0.0, 1.0, 0.0, 1.0}};
	RectInstance rects[30];
	for (int i = 0; i < 30; ++i) {
		rects[i] = base_instance;

		base_instance.box[0] *= 1.1;
		base_instance.box[1] *= 1.1;
		base_instance.box[2] *= 1.1;
		base_instance.box[3] *= 1.1;

		base_instance.color[2] += 0.09;
		bg_create(&rects[i], sizeof(RectInstance), SHADER_RECT, 0);
	}

	while (bg_should_close()) {
		float dt = bg_delta();

		Mouse mouse = bg_get_mouse();
		float speed = 10.0f;
		r2.box[0] = lerp(r2.box[0], mouse.x, speed * dt);
		r2.box[1] = lerp(r2.box[1], mouse.y, speed * dt);

		LOG_INFO("fps: %f", 1.0f / dt);
	}

	return 0;
}
