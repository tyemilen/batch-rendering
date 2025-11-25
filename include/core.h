#ifndef CORE_H_
#define CORE_H_

typedef struct Frame {
	int x, y, width, height;
} Frame;

typedef struct Color {
	float r, g, b, a;
} Color;

typedef struct Mouse {
	float x, y;

	struct {
		int pressed;
	} left_button, right_button, middle_button;
} Mouse;

typedef struct Window {
	char *title;
	char *gl_version_str;

	int width, height;
	int is_open;

	Mouse mouse;
} Window;

void core_init_gl(Window window);

Window core_create_window(char *title, int width, int height);
void core_get_mouse(Mouse *mouse);

void core_draw(void);

#define EVENT_NONE 0x0
#define EVENT_EXIT 0x1

typedef struct Event {
	int type;
	void *data1;
	void *data2;
} Event;

int core_poll_event(Event *);
int core_get_key(int k);
typedef struct Image {
	int width, height;

	unsigned char *pixels;

	int format;
} Image;

Image *core_load_image(const char *filename);

#endif // CORE_H_
