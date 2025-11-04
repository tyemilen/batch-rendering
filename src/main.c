#include "bglib.h"
#include "maler.h"

#include <stdbool.h>
#include <stddef.h>

#include "graphics/rect.h"

#include "core.h"

typedef struct {
	float box[4];
	float color[4];
} RectInstance;

void bind_rect(MalerContainer *container) {
	glBindVertexArray(container->vao);
	glBindBuffer(GL_ARRAY_BUFFER, container->instance_VBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance),
						  (void *)offsetof(RectInstance, box));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(RectInstance),
						  (void *)offsetof(RectInstance, color));
	glVertexAttribDivisor(2, 1);
}

int main(void) {
	bg_init("cool window", 800, 600);
	bg_register_shader(SHADER_RECT, vertex_shader_rect, fragment_shader_rect);

	bg_load_texture(1, "cats.jpg");
	bg_load_texture(2, "frog.png");

	RectInstance r1 = {{0, 0, 220, 220}, {1.0, 0.0, 1.0, 1.0}};

	MalerElement *rect_w_texture =
		bg_create(&r1, sizeof(RectInstance), SHADER_RECT, bg_get_texture(1));

	RectInstance r2 = {{50, 0, 20, 20}, {1.0, 0.0, 0.0, 1.0}};
	MalerElement *rect_plain =
		bg_create(&r2, sizeof(RectInstance), SHADER_RECT, 0);

	bg_create(&r2, sizeof(RectInstance), SHADER_RECT, 0);

	bind_rect(rect_w_texture->container);
	bind_rect(rect_plain->container);

	while (bg_should_close()) {
	}

	return 0;
}
