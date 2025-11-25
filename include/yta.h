#ifndef YTA_H_
#define YTA_H_

#include <stddef.h>

#include "core.h"
#include "maler.h"
#include "texture.h"

#include "objects/object.h"
#include "ygl.h"

#define COLOR_RED (Color){255, 0, 0, 1}
#define COLOR_GREEN (Color){0, 255, 0, 1}
#define COLOR_BLUE (Color){0, 0, 255, 1}
#define COLOR_YELLOW (Color){255, 255, 0, 1}
#define COLOR_CYAN (Color){0, 255, 255, 1}
#define COLOR_MAGENTA (Color){255, 0, 255, 1}
#define COLOR_ORANGE (Color){255, 165, 0, 1}
#define COLOR_PURPLE (Color){128, 0, 128, 1}
#define COLOR_PINK (Color){255, 192, 203, 1}
#define COLOR_BROWN (Color){165, 42, 42, 1}
#define COLOR_GRAY (Color){128, 128, 128, 1}
#define COLOR_BLACK (Color){0, 0, 0, 1}
#define COLOR_NONE (Color){0, 0, 0, 0}
#define COLOR_WHITE (Color){255, 255, 255, 1}

#define MAX_KEYBOARD_KEYS 256

typedef enum KeyboardKey {
	KEY_NULL = 0,

	KEY_SPACE,
	KEY_APOSTROPHE,
	KEY_COMMA,
	KEY_MINUS,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_SEMICOLON,
	KEY_EQUAL,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_LEFT_BRACKET,
	KEY_BACKSLASH,
	KEY_RIGHT_BRACKET,
	KEY_GRAVE,

	KEY_ESCAPE,
	KEY_ENTER,
	KEY_TAB,
	KEY_BACKSPACE,
	KEY_INSERT,
	KEY_DELETE,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_HOME,
	KEY_END,
	KEY_CAPS_LOCK,
	KEY_SCROLL_LOCK,
	KEY_NUM_LOCK,
	KEY_PRINT_SCREEN,
	KEY_PAUSE,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_LEFT_SHIFT,
	KEY_LEFT_CONTROL,
	KEY_LEFT_ALT,
	KEY_LEFT_SUPER,
	KEY_RIGHT_SHIFT,
	KEY_RIGHT_CONTROL,
	KEY_RIGHT_ALT,
	KEY_RIGHT_SUPER,
	KEY_MENU,

	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_KP_DECIMAL,
	KEY_KP_DIVIDE,
	KEY_KP_MULTIPLY,
	KEY_KP_SUBTRACT,
	KEY_KP_ADD,
	KEY_KP_ENTER,
	KEY_KP_EQUAL
} KeyboardKey;

void YtaRegisterObject(ObjectBase *obj);
Window YtaInit(char *title, int width, int height);
void YtaRegisterShader(int key, GLuint prog, void (*bind)(MalerContainer *));
MalerElement *YtaCreate(void *instance, int instance_size, int type,
						Texture *texture);
MalerElement *YtaCreateEx(void *instance, int instance_size, int type,
						  Texture *texture, MalerContainer *container);
MalerContainer *YtaCreateContainer(int shader_type, int texture_id);
void YtaDestroyElement(MalerContainer *container, size_t index);
void YtaDestroy(void);
Mouse YtaGetMouse(void);
Texture *YtaLoadTexture(int id, const char *filename);
Texture *YtaGetTexture(int id);
Window YtaGetWindow(void);
void YtaRun(void);
void YtaClear(Color color);
int YtaGetKeyState(KeyboardKey key);
int YtaShouldClose(void);
double YtaGetTime(void);
float YtaDelta(void);
#endif // YTA_H_
