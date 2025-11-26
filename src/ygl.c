#include "ygl.h"

GLuint(YGL_APIENTRY *glCreateShader)(GLenum);
void(YGL_APIENTRY *glShaderSource)(GLuint, GLsizei, const char **,
								   const GLint *);
void(YGL_APIENTRY *glCompileShader)(GLuint);
void(YGL_APIENTRY *glGetShaderiv)(GLuint, GLenum, GLint *);
void(YGL_APIENTRY *glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, char *);
void(YGL_APIENTRY *glDeleteShader)(GLuint);
GLuint(YGL_APIENTRY *glCreateProgram)(void);
void(YGL_APIENTRY *glAttachShader)(GLuint, GLuint);
void(YGL_APIENTRY *glLinkProgram)(GLuint);
void(YGL_APIENTRY *glUseProgram)(GLuint);
void(YGL_APIENTRY *glGetProgramiv)(GLuint, GLenum, GLint *);
void(YGL_APIENTRY *glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, char *);
void(YGL_APIENTRY *glDeleteProgram)(GLuint);

void(YGL_APIENTRY *glBindVertexArray)(GLuint);
void(YGL_APIENTRY *glGenVertexArrays)(GLsizei, GLuint *);
void(YGL_APIENTRY *glBindBuffer)(GLenum, GLuint);
void(YGL_APIENTRY *glGenBuffers)(GLsizei, GLuint *);
void(YGL_APIENTRY *glEnableVertexAttribArray)(GLuint);
void(YGL_APIENTRY *glVertexAttribDivisor)(GLuint, GLuint);
void(YGL_APIENTRY *glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean,
										  GLsizei, const void *);
GLint(YGL_APIENTRY *glGetUniformLocation)(GLuint, const char *);
void(YGL_APIENTRY *glUniform1i)(GLint, GLint);
void(YGL_APIENTRY *glUniform2f)(GLint, float, float);
void(YGL_APIENTRY *glActiveTexture)(GLenum);
void(YGL_APIENTRY *glBindTexture)(GLenum, GLuint);
void(YGL_APIENTRY *glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei);
void(YGL_APIENTRY *glEnable)(GLenum);
void(YGL_APIENTRY *glBlendFunc)(GLenum, GLenum);
void(YGL_APIENTRY *glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
void(YGL_APIENTRY *glClear)(GLbitfield);
void(YGL_APIENTRY *glGenTextures)(GLsizei, GLuint *);
void(YGL_APIENTRY *glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint,
								 GLenum, GLenum, const void *);
void(YGL_APIENTRY *glTexParameteri)(GLenum, GLenum, GLint);
void(YGL_APIENTRY *glBufferData)(GLenum, GLsizeiptr, const void *, GLenum);
void(YGL_APIENTRY *glViewport)(GLint, GLint, GLsizei, GLsizei);
void(YGL_APIENTRY *glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei,
									GLsizei, GLenum, GLenum, const void *);

void ygl_init(GLProcLoader proc) {
	glCreateShader = (GLuint(YGL_APIENTRY *)(GLenum))proc("glCreateShader");
	glShaderSource = (void(YGL_APIENTRY *)(
		GLuint, GLsizei, const char **, const GLint *))proc("glShaderSource");
	glCompileShader = (void(YGL_APIENTRY *)(GLuint))proc("glCompileShader");
	glGetShaderiv =
		(void(YGL_APIENTRY *)(GLuint, GLenum, GLint *))proc("glGetShaderiv");
	glGetShaderInfoLog = (void(YGL_APIENTRY *)(
		GLuint, GLsizei, GLsizei *, char *))proc("glGetShaderInfoLog");
	glDeleteShader = (void(YGL_APIENTRY *)(GLuint))proc("glDeleteShader");

	glCreateProgram = (GLuint(YGL_APIENTRY *)(void))proc("glCreateProgram");
	glAttachShader =
		(void(YGL_APIENTRY *)(GLuint, GLuint))proc("glAttachShader");
	glLinkProgram = (void(YGL_APIENTRY *)(GLuint))proc("glLinkProgram");
	glUseProgram = (void(YGL_APIENTRY *)(GLuint))proc("glUseProgram");
	glGetProgramiv =
		(void(YGL_APIENTRY *)(GLuint, GLenum, GLint *))proc("glGetProgramiv");
	glGetProgramInfoLog = (void(YGL_APIENTRY *)(
		GLuint, GLsizei, GLsizei *, char *))proc("glGetProgramInfoLog");
	glDeleteProgram = (void(YGL_APIENTRY *)(GLuint))proc("glDeleteProgram");

	glBindVertexArray = (void(YGL_APIENTRY *)(GLuint))proc("glBindVertexArray");
	glGenVertexArrays =
		(void(YGL_APIENTRY *)(GLsizei, GLuint *))proc("glGenVertexArrays");
	glBindBuffer = (void(YGL_APIENTRY *)(GLenum, GLuint))proc("glBindBuffer");
	glGenBuffers =
		(void(YGL_APIENTRY *)(GLsizei, GLuint *))proc("glGenBuffers");
	glEnableVertexAttribArray =
		(void(YGL_APIENTRY *)(GLuint))proc("glEnableVertexAttribArray");
	glVertexAttribDivisor =
		(void(YGL_APIENTRY *)(GLuint, GLuint))proc("glVertexAttribDivisor");
	glVertexAttribPointer =
		(void(YGL_APIENTRY *)(GLuint, GLint, GLenum, GLboolean, GLsizei,
							  const void *))proc("glVertexAttribPointer");

	glGetUniformLocation = (GLint(YGL_APIENTRY *)(GLuint, const char *))proc(
		"glGetUniformLocation");
	glUniform1i = (void(YGL_APIENTRY *)(GLint, GLint))proc("glUniform1i");
	glUniform2f =
		(void(YGL_APIENTRY *)(GLint, float, float))proc("glUniform2f");

	glActiveTexture = (void(YGL_APIENTRY *)(GLenum))proc("glActiveTexture");
	glBindTexture = (void(YGL_APIENTRY *)(GLenum, GLuint))proc("glBindTexture");
	glDrawArraysInstanced = (void(YGL_APIENTRY *)(
		GLenum, GLint, GLsizei, GLsizei))proc("glDrawArraysInstanced");
	glEnable = (void(YGL_APIENTRY *)(GLenum))proc("glEnable");
	glBlendFunc = (void(YGL_APIENTRY *)(GLenum, GLenum))proc("glBlendFunc");
	glClearColor = (void(YGL_APIENTRY *)(GLfloat, GLfloat, GLfloat,
										 GLfloat))proc("glClearColor");
	glClear = (void(YGL_APIENTRY *)(GLbitfield))proc("glClear");
	glGenTextures =
		(void(YGL_APIENTRY *)(GLsizei, GLuint *))proc("glGenTextures");
	glTexImage2D = (void(YGL_APIENTRY *)(GLenum, GLint, GLint, GLsizei, GLsizei,
										 GLint, GLenum, GLenum,
										 const void *))proc("glTexImage2D");
	glTexParameteri =
		(void(YGL_APIENTRY *)(GLenum, GLenum, GLint))proc("glTexParameteri");
	glBufferData = (void(YGL_APIENTRY *)(GLenum, GLsizeiptr, const void *,
										 GLenum))proc("glBufferData");
	glViewport = (void(YGL_APIENTRY *)(GLint, GLint, GLsizei, GLsizei))proc(
		"glViewport");
	glTexSubImage2D = (void(YGL_APIENTRY *)(
		GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum,
		const void *))proc("glTexSubImage2D");
}
