#ifndef GRAPHICS_HELPERS_H_
#define GRAPHICS_HELPERS_H_
#define BIND_ATTRIB_F(index, comps, field, STRUCT)                             \
	glEnableVertexAttribArray(index);                                          \
	glVertexAttribPointer(index, comps, GL_FLOAT, GL_FALSE, sizeof(STRUCT),    \
						  (void *)offsetof(STRUCT, field));                    \
	glVertexAttribDivisor(index, 1);

#define BIND_FLOAT(index, field, STRUCT) BIND_ATTRIB_F(index, 1, field, STRUCT)
#define BIND_VEC2(index, field, STRUCT) BIND_ATTRIB_F(index, 2, field, STRUCT)
#define BIND_VEC3(index, field, STRUCT) BIND_ATTRIB_F(index, 3, field, STRUCT)
#define BIND_VEC4(index, field, STRUCT) BIND_ATTRIB_F(index, 4, field, STRUCT)

#define __include_shader(path) "included_shader_" #path

#endif // GRAPHICS_HELPERS_H_
