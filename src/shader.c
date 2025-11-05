#include "shader.h"

#include <stdint.h>
#include <stdlib.h>

#include "core.h"
#include "maler.h"

#ifdef DEBUG
#include "log.h"
#endif

static inline uint32_t hash(uint32_t key) {
	key ^= key >> 16;
	key *= 0x85ebca6b;
	key ^= key >> 13;
	key *= 0xc2b2ae35;
	key ^= key >> 16;
	return key;
}

ShaderRegistry *shader_create_registry(size_t size) {
	ShaderRegistry *registry = malloc(sizeof(ShaderRegistry));
	registry->size = size;
	registry->entries = calloc(size, sizeof(ShaderEntry));

	return registry;
}

void shader_register(ShaderRegistry *registry, int key, GLuint prog, void (*bind)(MalerContainer *)) {
	if (registry->count + 1 >= registry->size) {
		ShaderEntry *old_entries = registry->entries;
		size_t old_size = registry->size;

		registry->entries = calloc(registry->size * 2, sizeof(ShaderEntry));
		registry->size = registry->size * 2;
		registry->count = 0;

		for (size_t i = 0; i < old_size; ++i) {
			if (old_entries[i].occupied) {
				shader_register(registry, old_entries[i].key,
								old_entries[i].prog, old_entries[i].bind);
			}
		}

		free(old_entries);
	}

	uint32_t idx = hash((uint32_t)key) & (registry->size - 1);

	while (registry->entries[idx].occupied &&
		   registry->entries[idx].key != key) {
		idx = (idx + 1) & (registry->size - 1);
	}

	registry->entries[idx].key = key;
	registry->entries[idx].prog = prog;
	registry->entries[idx].bind = bind;
	registry->entries[idx].occupied = 1;
	++registry->count;
}

int *shader_registry_get_keys(ShaderRegistry *registry, size_t *out_count) {
	*out_count = registry->count;
	if (registry->count == 0) return NULL;

	int *keys = malloc(registry->count * sizeof(int));
	size_t idx = 0;

	for (size_t i = 0; i < registry->size; ++i) {
		if (registry->entries[i].occupied) {
			keys[idx++] = registry->entries[i].key;
		}
	}

	return keys;
}

ShaderEntry shader_get(ShaderRegistry *registry, int key) {
	uint32_t idx = hash((uint32_t)key) & (registry->size - 1);

	while (registry->entries[idx].occupied) {
		if (registry->entries[idx].key == key)
			return registry->entries[idx];
		idx = (idx + 1) & (registry->size - 1);
	}

	return (ShaderEntry){0};
}

void shader_registry_destroy(ShaderRegistry *registry) {
	free(registry->entries);
	free(registry);
}

GLuint compile_shader(GLenum type, const char *src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
#ifdef DEBUG
		char log[1024];
		glGetShaderInfoLog(shader, sizeof(log), 0, log);
		LOG_ERROR("compile_shader():\n %s\n", log);
#endif
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint shader_create(const char *vert_src, const char *frag_src) {
	GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
	GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);
	if (!vert || !frag) {
		glDeleteShader(vert);
		glDeleteShader(frag);
		return 0;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
#ifdef DEBUG
		char log[1024];
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		LOG_ERROR("shader_create(): %s\n", log);
#endif

		glDeleteProgram(program);
		return 0;
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
	return program;
}
