#ifndef YGL_H_
#define YGL_H_

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_ARRAY_BUFFER 0x8892
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_TRUE 1
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TRIANGLES 0x0004
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_MULTISAMPLE 0x809D
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE0 0x84C0
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_BGRA 0x80E1
#define GL_BGR 0x80E0
#define GL_RGBA8 0x8058
#define GL_RGB8 0x8051
#define GL_UNSIGNED_BYTE 0x1401
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_LINEAR 0x2601
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_RED 0x1903

typedef long long GLsizeiptr;
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

extern GLuint (*glCreateShader)(GLenum);
extern void (*glShaderSource)(GLuint, GLsizei, const char **, const GLint *);
extern void (*glCompileShader)(GLuint);
extern void (*glGetShaderiv)(GLuint, GLenum, GLint *);
extern void (*glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, char *);
extern void (*glDeleteShader)(GLuint);
extern GLuint (*glCreateProgram)(void);
extern void (*glAttachShader)(GLuint, GLuint);
extern void (*glLinkProgram)(GLuint);
extern void (*glUseProgram)(GLuint);
extern void (*glGetProgramiv)(GLuint, GLenum, GLint *);
extern void (*glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, char *);
extern void (*glDeleteProgram)(GLuint);
extern void (*glBindVertexArray)(GLuint);
extern void (*glGenVertexArrays)(GLsizei, GLuint *);
extern void (*glBindBuffer)(GLenum, GLuint);
extern void (*glGenBuffers)(GLsizei, GLuint *);
extern void (*glEnableVertexAttribArray)(GLuint);
extern void (*glVertexAttribDivisor)(GLuint, GLuint);
extern void (*glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei,
									 const void *);
extern GLint (*glGetUniformLocation)(GLuint, const char *);
extern void (*glUniform1i)(GLint, GLint);
extern void (*glUniform2f)(GLint, float, float);
extern void (*glActiveTexture)(GLenum);
extern void (*glBindTexture)(GLenum, GLuint);
extern void (*glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei);
extern void (*glEnable)(GLenum);
extern void (*glBlendFunc)(GLenum, GLenum);
extern void (*glClearColor)(float, float, float, float);
extern void (*glClear)(GLbitfield);
extern void (*glGenTextures)(GLsizei, GLuint *);
extern void (*glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint,
							GLenum, GLenum, const void *);
extern void (*glTexParameteri)(GLenum, GLenum, GLint);
extern void (*glBufferData)(GLenum, GLsizeiptr, const void *, GLenum);
extern void (*glViewport)(GLint, GLint, GLsizei, GLsizei);
extern void (*glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
							   GLenum, GLenum, const void *);

typedef void *(*GLProcLoader)(const char *name);

void ygl_init(GLProcLoader proc);

#endif // YGL_H_
