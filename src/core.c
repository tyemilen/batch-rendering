#include "core.h"

#ifdef PLATFORM_SDL3
#include "platform/sdl3.h"
#endif
#ifdef PLATFORM_WIN32
#include "platform/win32.h"
#endif

GLuint (*glCreateShader)(GLenum);
void (*glShaderSource)(GLuint, GLsizei, const char **, const GLint *);
void (*glCompileShader)(GLuint);
void (*glGetShaderiv)(GLuint, GLenum, GLint *);
void (*glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, char *);
void (*glDeleteShader)(GLuint);

GLuint (*glCreateProgram)(void);
void (*glAttachShader)(GLuint, GLuint);
void (*glLinkProgram)(GLuint);
void (*glUseProgram)(GLuint);
void (*glGetProgramiv)(GLuint, GLenum, GLint *);
void (*glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, char *);
void (*glDeleteProgram)(GLuint);

void (*glBindVertexArray)(GLuint);
void (*glGenVertexArrays)(GLsizei, GLuint *);
void (*glBindBuffer)(GLenum, GLuint);
void (*glGenBuffers)(GLsizei, GLuint *);
void (*glEnableVertexAttribArray)(GLuint);
void (*glVertexAttribDivisor)(GLuint, GLuint);
void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei,
							  const void *);

GLint (*glGetUniformLocation)(GLuint, const char *);
void (*glUniform1i)(GLint, GLint);
void (*glUniform2f)(GLint, float, float);

void (*glActiveTexture)(GLenum);
void (*glBindTexture)(GLenum, GLuint);
void (*glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei);
void (*glEnable)(GLenum);
void (*glBlendFunc)(GLenum, GLenum);
void (*glClearColor)(float, float, float, float);
void (*glClear)(GLbitfield);
void (*glGenTextures)(GLsizei, GLuint *);
void (*glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
					 GLenum, const void *);
void (*glTexParameteri)(GLenum, GLenum, GLint);
void (*glBufferData)(GLenum, GLsizeiptr, const void *, GLenum);
void (*glViewport)(GLint, GLint, GLsizei, GLsizei);

void core_load_gl(GLProcLoader proc) {
	glCreateShader = (GLuint (*)(GLenum))proc("glCreateShader");
	glShaderSource = (void (*)(GLuint, GLsizei, const char **,
							   const GLint *))proc("glShaderSource");
	glCompileShader = (void (*)(GLuint))proc("glCompileShader");
	glGetShaderiv = (void (*)(GLuint, GLenum, GLint *))proc("glGetShaderiv");
	glGetShaderInfoLog = (void (*)(GLuint, GLsizei, GLsizei *, char *))proc(
		"glGetShaderInfoLog");
	glDeleteShader = (void (*)(GLuint))proc("glDeleteShader");
	glCreateProgram = (GLuint (*)(void))proc("glCreateProgram");
	glAttachShader = (void (*)(GLuint, GLuint))proc("glAttachShader");
	glLinkProgram = (void (*)(GLuint))proc("glLinkProgram");
	glUseProgram = (void (*)(GLuint))proc("glUseProgram");
	glGetProgramiv = (void (*)(GLuint, GLenum, GLint *))proc("glGetProgramiv");
	glGetProgramInfoLog = (void (*)(GLuint, GLsizei, GLsizei *, char *))proc(
		"glGetProgramInfoLog");
	glDeleteProgram = (void (*)(GLuint))proc("glDeleteProgram");
	glBindVertexArray = (void (*)(GLuint))proc("glBindVertexArray");
	glGenVertexArrays = (void (*)(GLsizei, GLuint *))proc("glGenVertexArrays");
	glBindBuffer = (void (*)(GLenum, GLuint))proc("glBindBuffer");
	glGenBuffers = (void (*)(GLsizei, GLuint *))proc("glGenBuffers");
	glEnableVertexAttribArray =
		(void (*)(GLuint))proc("glEnableVertexAttribArray");
	glVertexAttribDivisor =
		(void (*)(GLuint, GLuint))proc("glVertexAttribDivisor");
	glVertexAttribPointer =
		(void (*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void *))proc(
			"glVertexAttribPointer");
	glGetUniformLocation =
		(GLint (*)(GLuint, const char *))proc("glGetUniformLocation");
	glUniform1i = (void (*)(GLint, GLint))proc("glUniform1i");
	glUniform2f = (void (*)(GLint, float, float))proc("glUniform2f");
	glActiveTexture = (void (*)(GLenum))proc("glActiveTexture");
	glBindTexture = (void (*)(GLenum, GLuint))proc("glBindTexture");
	glDrawArraysInstanced = (void (*)(GLenum, GLint, GLsizei, GLsizei))proc(
		"glDrawArraysInstanced");
	glEnable = (void (*)(GLenum))proc("glEnable");
	glBlendFunc = (void (*)(GLenum, GLenum))proc("glBlendFunc");
	glClearColor = (void (*)(float, float, float, float))proc("glClearColor");
	glClear = (void (*)(GLbitfield))proc("glClear");
	glGenTextures = (void (*)(GLsizei, GLuint *))proc("glGenTextures");
	glTexImage2D = (void (*)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint,
							 GLenum, GLenum, const void *))proc("glTexImage2D");
	glTexParameteri = (void (*)(GLenum, GLenum, GLint))proc("glTexParameteri");
	glBufferData = (void (*)(GLenum, GLsizeiptr, const void *, GLenum))proc(
		"glBufferData");
	glViewport = (void (*)(GLint, GLint, GLsizei, GLsizei))proc("glViewport");
}

void core_init_gl(Window window) {
	core_load_gl((GLProcLoader)platform_gl_proc);
	glViewport(0, 0, window.width, window.height);
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

Window core_create_window(char *title, int width, int height) {
	Window window = {.title = title, .width = width, .height = height};
	window.mouse = (Mouse){0};

	platform_init(&window);

	core_get_mouse(&window.mouse);
	return window;
}

void core_draw(void) { platform_draw(); }

int core_poll_event(Event *event) {
	platform_poll_event(event);
	return event->type;
}

Image *core_load_image(const char *filename) {
	return platform_load_image(filename);
}

void core_get_mouse(Mouse *mouse) { platform_get_mouse(mouse); }
