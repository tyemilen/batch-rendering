#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "font.h"
#include "fs.h"
#include "log.h"
#include "texture.h"
#include "yta.h"

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
	Window window = YtaInit("char *title", 600, 800);
	Atlas atlas =
		font_create_atlas((unsigned char *)fs_read_file("font.ttf"), 64, 1024);
	TextObject *fps = yCreateText("FPS: 0", 0, 0, 16, COLOR_GREEN, &atlas);

	RectObject *rect = yCreateRect(20, 20, 30, 30, COLOR_PINK, 0);
	yCreateRect(0, window.height - 30, window.width, 30, COLOR_MAGENTA, 0);

	while (!YtaShouldClose()) {
		YtaClear(COLOR_WHITE);

		float dt = YtaDelta();
		fps->text = strfmt("FPS: %.f", 1.0 / dt);

		Mouse mouse = YtaGetMouse();

		rect->x = lerp(rect->x, mouse.x, dt * 10.0f);
		rect->y = lerp(rect->y, mouse.y, dt * 10.0f);
	}

	YtaDestroy();
	return 0;
}