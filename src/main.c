#include "bglib.h"

#include <stdbool.h>
#include <stddef.h>

#include "graphics/rect.h"

int main(void) {
	bg_init("cool window", 800, 600);
	bg_register_shader(SHADER_RECT, shader_rect_get(), shader_rect_bind);

	bg_load_texture(1, "cats.jpg");
	bg_load_texture(2, "frog.png");

	RectInstance r1 = {{0, 0, 220, 220}, {1.0, 0.0, 1.0, 1.0}};
	bg_create(&r1, sizeof(RectInstance), SHADER_RECT, bg_get_texture(1));

	RectInstance r2 = {{50, 0, 20, 20}, {1.0, 0.0, 0.0, 1.0}};
	bg_create(&r2, sizeof(RectInstance), SHADER_RECT, bg_get_texture(2));

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
	}

	return 0;
}
