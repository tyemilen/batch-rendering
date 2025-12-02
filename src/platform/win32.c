#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

#include "core.h"
#include "fs.h"
#include "log.h"
#include "ygl.h"
#include "yta.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
typedef struct {
	HWND hwnd;
	HDC hdc;
	HGLRC glContext;

	int keys_state[MAX_KEYBOARD_KEYS];
} PlatformData;

PlatformData platform = {0};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC,
														 HGLRC hShareContext,
														 const int *attribList);
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

int platform_init(Window *window) {
	WNDCLASS wc = {0};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = window->title;
	wc.style = CS_OWNDC;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	if (!RegisterClass(&wc)) return 0;

	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	DWORD exStyle = 0;

	RECT r = {0, 0, window->width, window->height};
	AdjustWindowRectEx(&r, style, FALSE, exStyle);

	int outer_w = r.right - r.left;
	int outer_h = r.bottom - r.top;

	platform.hwnd = CreateWindowEx(exStyle, wc.lpszClassName, window->title,
								   style, CW_USEDEFAULT, CW_USEDEFAULT, outer_w,
								   outer_h, 0, 0, wc.hInstance, 0);

	if (!platform.hwnd) return 0;

	ShowWindow(platform.hwnd, SW_SHOW);

	RECT rect = {0};
	GetClientRect(platform.hwnd, &rect);
	window->width = rect.right - rect.left;
	window->height = rect.bottom - rect.top;

	platform.hdc = GetDC(platform.hwnd);
	if (!platform.hdc) return 0;

	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(platform.hdc, &pfd);
	if (!pf) return 0;
	if (!SetPixelFormat(platform.hdc, pf, &pfd)) return 0;

	HGLRC tempContext = wglCreateContext(platform.hdc);
	if (!tempContext) return 0;
	if (!wglMakeCurrent(platform.hdc, tempContext)) return 0;

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress(
			"wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB) {
		wglDeleteContext(tempContext);
		return 0;
	}

	int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
					 3,
					 WGL_CONTEXT_MINOR_VERSION_ARB,
					 3,
					 WGL_CONTEXT_PROFILE_MASK_ARB,
					 WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
					 0};

	HGLRC modernContext = wglCreateContextAttribsARB(platform.hdc, 0, attribs);
	if (!modernContext) {
		wglDeleteContext(tempContext);
		return 0;
	}

	wglMakeCurrent(0, 0);
	wglDeleteContext(tempContext);

	if (!wglMakeCurrent(platform.hdc, modernContext)) return 0;
	platform.glContext = modernContext;
	window->gl_version_str = "#version 330";

	return 1;
}

Image *platform_load_image(const char *filename) {
	if (!filename) return 0;

	YFile *file = fs_read_file((char *)filename);
	if (!file) return 0;

	Image *image = malloc(sizeof(Image));
	if (!image) {
		free(file->data);
		free(file);
		return 0;
	}

	int temp_chs = 0;
	image->pixels =
		stbi_load_from_memory(file->data, (int)file->size, &image->width,
							  &image->height, &temp_chs, 4);
	free(file->data);
	free(file);

	if (!image->pixels) {
		free(image);
		return 0;
	}

	image->format = GL_RGBA;
	return image;
}

void platform_draw(void) { SwapBuffers(platform.hdc); }

KeyboardKey vkey_to_yta(WPARAM wkey) {
	switch (wkey) {
	case VK_SPACE:
		return KEY_SPACE;
	case VK_OEM_7:
		return KEY_APOSTROPHE;
	case VK_OEM_COMMA:
		return KEY_COMMA;
	case VK_OEM_MINUS:
		return KEY_MINUS;
	case VK_OEM_PERIOD:
		return KEY_PERIOD;
	case VK_OEM_2:
		return KEY_SLASH;
	case '0':
		return KEY_0;
	case '1':
		return KEY_1;
	case '2':
		return KEY_2;
	case '3':
		return KEY_3;
	case '4':
		return KEY_4;
	case '5':
		return KEY_5;
	case '6':
		return KEY_6;
	case '7':
		return KEY_7;
	case '8':
		return KEY_8;
	case '9':
		return KEY_9;
	case VK_OEM_1:
		return KEY_SEMICOLON;
	case VK_OEM_PLUS:
		return KEY_EQUAL;
	case 'A':
		return KEY_A;
	case 'B':
		return KEY_B;
	case 'C':
		return KEY_C;
	case 'D':
		return KEY_D;
	case 'E':
		return KEY_E;
	case 'F':
		return KEY_F;
	case 'G':
		return KEY_G;
	case 'H':
		return KEY_H;
	case 'I':
		return KEY_I;
	case 'J':
		return KEY_J;
	case 'K':
		return KEY_K;
	case 'L':
		return KEY_L;
	case 'M':
		return KEY_M;
	case 'N':
		return KEY_N;
	case 'O':
		return KEY_O;
	case 'P':
		return KEY_P;
	case 'Q':
		return KEY_Q;
	case 'R':
		return KEY_R;
	case 'S':
		return KEY_S;
	case 'T':
		return KEY_T;
	case 'U':
		return KEY_U;
	case 'V':
		return KEY_V;
	case 'W':
		return KEY_W;
	case 'X':
		return KEY_X;
	case 'Y':
		return KEY_Y;
	case 'Z':
		return KEY_Z;
	case VK_OEM_4:
		return KEY_LEFT_BRACKET;
	case VK_OEM_5:
		return KEY_BACKSLASH;
	case VK_OEM_6:
		return KEY_RIGHT_BRACKET;
	case VK_OEM_3:
		return KEY_GRAVE;
	case VK_ESCAPE:
		return KEY_ESCAPE;
	case VK_RETURN:
		return KEY_ENTER;
	case VK_TAB:
		return KEY_TAB;
	case VK_BACK:
		return KEY_BACKSPACE;
	case VK_INSERT:
		return KEY_INSERT;
	case VK_DELETE:
		return KEY_DELETE;
	case VK_RIGHT:
		return KEY_RIGHT;
	case VK_LEFT:
		return KEY_LEFT;
	case VK_DOWN:
		return KEY_DOWN;
	case VK_UP:
		return KEY_UP;
	case VK_PRIOR:
		return KEY_PAGE_UP;
	case VK_NEXT:
		return KEY_PAGE_DOWN;
	case VK_HOME:
		return KEY_HOME;
	case VK_END:
		return KEY_END;
	case VK_CAPITAL:
		return KEY_CAPS_LOCK;
	case VK_SCROLL:
		return KEY_SCROLL_LOCK;
	case VK_NUMLOCK:
		return KEY_NUM_LOCK;
	case VK_SNAPSHOT:
		return KEY_PRINT_SCREEN;
	case VK_PAUSE:
		return KEY_PAUSE;
	case VK_F1:
		return KEY_F1;
	case VK_F2:
		return KEY_F2;
	case VK_F3:
		return KEY_F3;
	case VK_F4:
		return KEY_F4;
	case VK_F5:
		return KEY_F5;
	case VK_F6:
		return KEY_F6;
	case VK_F7:
		return KEY_F7;
	case VK_F8:
		return KEY_F8;
	case VK_F9:
		return KEY_F9;
	case VK_F10:
		return KEY_F10;
	case VK_F11:
		return KEY_F11;
	case VK_F12:
		return KEY_F12;
	case VK_LSHIFT:
		return KEY_LEFT_SHIFT;
	case VK_LCONTROL:
		return KEY_LEFT_CONTROL;
	case VK_LMENU:
		return KEY_LEFT_ALT;
	case VK_LWIN:
		return KEY_LEFT_SUPER;
	case VK_RSHIFT:
		return KEY_RIGHT_SHIFT;
	case VK_RCONTROL:
		return KEY_RIGHT_CONTROL;
	case VK_RMENU:
		return KEY_RIGHT_ALT;
	case VK_RWIN:
		return KEY_RIGHT_SUPER;
	case VK_APPS:
		return KEY_MENU;
	case VK_NUMPAD0:
		return KEY_KP_0;
	case VK_NUMPAD1:
		return KEY_KP_1;
	case VK_NUMPAD2:
		return KEY_KP_2;
	case VK_NUMPAD3:
		return KEY_KP_3;
	case VK_NUMPAD4:
		return KEY_KP_4;
	case VK_NUMPAD5:
		return KEY_KP_5;
	case VK_NUMPAD6:
		return KEY_KP_6;
	case VK_NUMPAD7:
		return KEY_KP_7;
	case VK_NUMPAD8:
		return KEY_KP_8;
	case VK_NUMPAD9:
		return KEY_KP_9;
	case VK_DECIMAL:
		return KEY_KP_DECIMAL;
	case VK_DIVIDE:
		return KEY_KP_DIVIDE;
	case VK_MULTIPLY:
		return KEY_KP_MULTIPLY;
	case VK_SUBTRACT:
		return KEY_KP_SUBTRACT;
	case VK_ADD:
		return KEY_KP_ADD;
	case VK_SEPARATOR:
		return KEY_KP_ENTER;
	case VK_OEM_NEC_EQUAL:
		return KEY_KP_EQUAL;
	default:
		return KEY_NULL;
	}
}

void handle_key(WPARAM wparam, int state) {
	KeyboardKey key = vkey_to_yta(wparam);

	platform.keys_state[key] = state;
}

int platform_get_key(int k) { return platform.keys_state[k]; }

void platform_poll_event(Event *event) {
	MSG msg;
	event->type = EVENT_NONE;
	event->data1 = 0;
	event->data2 = 0;

	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		switch (msg.message) {
		case WM_QUIT:
			event->type = EVENT_EXIT;
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			handle_key(msg.wParam, 1);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			handle_key(msg.wParam, 0);
			break;
		default:
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void platform_get_mouse(Mouse *mouse) {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(platform.hwnd, &p);
	mouse->x = p.x;
	mouse->y = p.y;
	mouse->left_button.pressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

void *platform_gl_proc(const char *name) {
	void *proc = (void *)wglGetProcAddress(name);
	if (!proc) {
		static HMODULE module = 0;
		if (!module) module = LoadLibraryA("opengl32.dll");
		if (module) proc = (void *)GetProcAddress(module, name);
	}
	LOG_INFO("GL proc: %s => %p", name, proc);

	return proc;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
							LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
