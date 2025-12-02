#include "objects/texture.h"
#define DEBUG 1

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "font.h"
#include "fs.h"
#include "log.h"
#include "texture.h"
#include "yta.h"

#include "objects/grid.h"
#include "objects/rect.h"
#include "objects/text.h"

float lerp(float a, float b, float t) { return a + (b - a) * t; }

#ifdef _WIN32
char *strfmt(const char *fmt, ...) {
#else
__attribute__((format(printf, 1, 2))) char *strfmt(const char *fmt, ...) {
#endif
	va_list ap;
	va_start(ap, fmt);

	va_list ap_copy;
	va_copy(ap_copy, ap);

	int size = vsnprintf(0, 0, fmt, ap_copy);
	va_end(ap_copy);

	if (size < 0) {
		va_end(ap);
		return 0;
	}

	char *buf = malloc(size + 1);
	if (!buf) {
		va_end(ap);
		return 0;
	}

	vsnprintf(buf, size + 1, fmt, ap);
	va_end(ap);

	return buf;
}

int main(void) {
	Window window = YtaInit("char *title", 1024, 768);
	YFile *font_file = fs_read_file("font.ttf");

	Texture *frog_png = YtaLoadTexture(1, "./cat.png");

	Atlas atlas = font_create_atlas(font_file->data, 64, 1024);

	yCreateGrid(0, 0, window.width, window.height, 8, (Color){0, 0, 0, .05},
				COLOR_WHITE);
	yCreateTexture(60, 120, frog_png->width, frog_png->height, frog_png);


	TextObject *fps = yCreateText("FPS: 0", 0, 0, 16, COLOR_RED, &atlas);
	TextObject *pos = yCreateText("pos: x, y", 0, 20, 16, COLOR_RED, &atlas);
	RectObject *rect = yCreateRect(20, 20, 30, 30, COLOR_PINK, 0);

	yCreateRect(0, window.height - 30, window.width, 30, COLOR_MAGENTA, 0);
	LOG_INFO("%d, %d", frog_png->width, frog_png->height);
	LOG_INFO("%d, %d", window.width, window.height);
	while (!YtaShouldClose()) {
		YtaClear(COLOR_WHITE);

		float dt = YtaDelta();

		fps->text = strfmt("FPS: %.0f", 1.0f / dt);
		pos->text = strfmt("pos: %.2f, %.2f", rect->x, rect->y);

		Mouse mouse = YtaGetMouse();

		if (YtaGetKeyState(KEY_1)) {
			LOG_INFO("1");
		}
		if (!mouse.left_button.pressed) {
			rect->x = lerp(rect->x, mouse.x, dt * 10.0f);
			rect->y = lerp(rect->y, mouse.y, dt * 10.0f);
		}
	}

	YtaDestroy();
	return 0;
}