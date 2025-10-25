#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>
#include <stddef.h>

typedef struct ShaderEntry {
	int key;
	GLuint value;
	int occupied;
} ShaderEntry;

typedef struct ShaderRegistry {
	ShaderEntry* entries;
	size_t size;
	size_t count;
} ShaderRegistry;

ShaderRegistry* shader_create_registry(size_t size);
void shader_register(ShaderRegistry* registry, int key, GLuint prog);
GLuint shader_get(ShaderRegistry* registry, int type);
void shader_registry_destroy(ShaderRegistry* registry);
int* shader_registry_get_keys(ShaderRegistry *registry, size_t *out_count);

GLuint shader_compile(GLenum type, const char *src);
GLuint shader_create(const char *vert_src, const char *frag_src);

#endif // SHADER_H_
