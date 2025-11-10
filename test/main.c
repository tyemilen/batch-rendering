#include "yta.h"

#include "fs.h"

#include "objects/rect.h"
#include "objects/text.h"

typedef struct Button {
	int x, y, width, height;

	RectObject rect;
	TextObject text;
} Button;

Button create_button(char *text, int x, int y, Atlas *atlas);
void update_button(Button *, Atlas *atlas);

int in(int x1, int y1, int x2, int y2, int width, int height) {
	return x1 >= x2 && x1 <= x2 + width && y1 >= y2 && y1 <= y2 + height;
}

int main(void) {
	yta_init("char *title", 200, 300);
	Atlas atlas =
		font_create_atlas((unsigned char *)fs_read_file("font.ttf"), 64, 1024);

	Button btn = create_button("button", 0, 0, &atlas);

	while (!yta_should_close()) {
		yta_clear(COLOR_BLACK);

		Mouse mouse = yta_get_mouse();

		if (in(mouse.x, mouse.y, btn.x, btn.y, btn.width, btn.height) &&
			mouse.left_button.pressed) {
			btn.x += 10;
		}
		update_button(&btn, &atlas);
	}

	return 0;
}
Button create_button(char *text, int x, int y, Atlas *atlas) {
	Button button = {0};

	button.x = x;
	button.y = y;

	button.rect = create_rect(x, y, 0, 0, COLOR_BLUE, 0);
	button.text = create_text(text, x, y, 16, COLOR_MAGENTA, atlas);

	button.rect.width = button.text.width;
	button.rect.height = button.text.height;
	update_rect(&button.rect);

	button.width = button.rect.width;
	button.height = button.rect.height;

	return button;
}

void update_button(Button *button, Atlas *atlas) {
	button->rect.x = button->x;
	button->rect.y = button->y;
	button->rect.width = button->width;
	button->rect.height = button->height;
	update_rect(&button->rect);
}
