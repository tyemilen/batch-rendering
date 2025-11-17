#include <windows.h>
#include <libloaderapi.h>
#include <minwinbase.h>
#include <stdlib.h>
#include <wingdi.h>

#include "core.h"
#include "ygl.h"

#include "log.h"

typedef struct {
	HWND hwnd;
	HDC hdc;
	HGLRC glContext;
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
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = window->title;
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc)) return 0;

	platform.hwnd = CreateWindowEx(
		0, wc.lpszClassName, window->title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, window->width, window->height, NULL, NULL,
		wc.hInstance, NULL);
	if (!platform.hwnd) return 0;

	ShowWindow(platform.hwnd, SW_SHOW);

	RECT rect = {0};
	GetClientRect(platform.hwnd, &rect);
	window->width = rect.right;
	window->height = rect.bottom;

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

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);
	if (!wglMakeCurrent(platform.hdc, modernContext)) return 0;

	platform.glContext = modernContext;
	window->gl_version_str = "#version 330";

	return 1;
}

Image *platform_load_image(const char *filename) {
	Image *image = malloc(sizeof(Image));
	if (!image) {
		return NULL;
	}

	image->format = GL_BGRA;
	image->width = 2;
	image->height = 2;
	image->pixels = (unsigned char *)"cool placeholder";

	return image;
}

void platform_draw(void) { SwapBuffers(platform.hdc); }

void platform_poll_event(Event *event) {
	MSG msg;
	event->type = EVENT_NONE;
	event->data1 = 0;
	event->data2 = 0;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			event->type = EVENT_EXIT;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			LOG_INFO("msg=%u", msg.message);
		}
	}
}

void platform_get_mouse(Mouse *mouse) {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(platform.hwnd, &p);
	mouse->x = p.x;
	mouse->y = p.y;
}

void *platform_gl_proc(const char *name) {
	void *proc = (void *)wglGetProcAddress(name);
	if (!proc) {
		static HMODULE module = NULL;
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
