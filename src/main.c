#include "texture.h"
#include "yta.h"

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "font.h"
#include "fs.h"

#include "objects/rect.h"
#include "objects/text.h"

float lerp(float a, float b, float t) { return a + (b - a) * t; }

__attribute__((format(printf, 1, 2))) char *strfmt(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (size < 0) return NULL;
	char *buf = malloc(size + 1);
	if (!buf) return NULL;

	va_start(args, fmt);
	vsnprintf(buf, size + 1, fmt, args);
	va_end(args);

	return buf;
}

int main(void) {
#ifdef PLATFORM_SDL3
	yta_init("SDL3 window", 800, 600);
#endif
#ifdef PLATFORM_WIN32
	yta_init("WIN32 window", 800, 600);
#endif
	Atlas atlas =
		font_create_atlas((unsigned char *)fs_read_file("font.ttf"), 64, 1024);

	RectObject rect = create_rect(20, 30, 200, 200, (Color){255, 0, 255, 1}, 0);
	TextObject text = create_text("Cool text", 0, 0, 36, COLOR_BLACK, &atlas);

	float text_vx = 2.0f;
	float text_vy = 2.0f;

	Window window = yta_get_window();

	TextObject fps = create_text("FPS: 0", 0, 0, 32, COLOR_BLACK, &atlas);

	yta_load_texture(0, "cats.jpg");

	while (!yta_should_close()) {
		float t = yta_get_time() * 2.0f;
		float dt = yta_delta();

		yta_clear(COLOR_WHITE);

		fps.text = strfmt("FPS: %.f", 1.0 / dt);
		fps.x = window.width - fps.width - 50;

		text.x += text_vx;
		text.y += text_vy;

		if (text.x < 0 || text.x + text.width > window.width) {
			text_vx = -text_vx;
			text.x = fmax(0, fmin(text.x, window.width - text.width));
			text.text = "foo";
		}
		if (text.y < 0 || text.y + text.height > window.height) {
			text_vy = -text_vy;
			text.y = fmax(0, fmin(text.y, window.height - text.height));
			text.text = "bar";
		}
		text.color.r = ((sinf(t * 5.0f) * 0.5f + 0.5f) * 255);
		text.color.g =
			((sinf((t * 2.0f) + 2.0f * M_PI / 3.0f) * 0.5f + 0.5f) * 255);
		text.color.b =
			((sinf((t * 6.0f) + 4.0f * M_PI / 3.0f) * 0.5f + 0.5f) * 255);


		Mouse mouse = yta_get_mouse();
		if (mouse.left_button.pressed) {
			rect.x = lerp(rect.x, mouse.x, dt * 10.0f);
			rect.y = lerp(rect.y, mouse.y, dt * 10.0f);

			rect.texture = yta_get_texture(0);
			text.text = "Cool text 2";
		}

		rect.color.r = ((sinf(t) * 0.5f + 0.5f) * 255);
		rect.color.g = ((sinf(t + 2.0f * M_PI / 3.0f) * 0.5f + 0.5f) * 255);
		rect.color.b = ((sinf(t + 4.0f * M_PI / 3.0f) * 0.5f + 0.5f) * 255);

		update_rect(&rect);
		update_text(&text, &atlas);
		update_text(&fps, &atlas);
	}

	yta_destroy();
	return 0;
}
