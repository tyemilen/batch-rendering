#include "bglib.h"
#include "maler.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <stdbool.h>

#include "graphics/rect.h"

int main(void) {
	bg_init("cool window", 800, 600);

	float rect_data[5] = {1.0f, 0.6f, 0.6f, 1.0f, 0.0f};
	float rect_data_2[5] = {1.0f, 0.8f, 0.6f, 1.0f, 10.0f};
	float floor_data[5] = {0.8f, 0.7f, 0.4f, 0.5f, 0.0f};

	bg_register_shader(SHADER_RECT, (char *)fragment_shader_rect);

	bg_load_texture(0xCA7, "cats.jpg");
	bg_load_texture(0xF906, "frog.png");

	float bg_data[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	bg_create((MalerBox){0, 0, 800, 600}, (void*)bg_data, 4, SHADER_RECT, bg_get_texture(0xF906), 0);
	MalerElement *rect =
		bg_create((MalerBox){0, 0, 120, 120}, (void *)rect_data, 5, SHADER_RECT, NULL, 1);
	bg_create((MalerBox){29, 30, 60, 60}, (void *)rect_data_2, 5, SHADER_RECT, bg_get_texture(0xCA7), 1);
	MalerElement *floor =
		bg_create((MalerBox){0, 540, 800, 120}, (void *)floor_data, 5, SHADER_RECT, NULL, 2);

	float vel_x = 0.0f;
	float vel_y = 0.0f;

	const float accel = 1.5f;
	const float max_speed = 15.0f;
	const float friction = 0.3f;
	const float gravity = 0.5f;
	const float jump_strength = 12.0f;

	MalerContainer *rect_container = bg_get_container(1);

	while (bg_should_close()) {
		const bool *key_states = SDL_GetKeyboardState(NULL);

		if (key_states[SDL_SCANCODE_D])
			vel_x += accel;
		else if (key_states[SDL_SCANCODE_A])
			vel_x -= accel;
		else {
			if (vel_x > 0) {
				vel_x -= friction;
				if (vel_x < 0) vel_x = 0;
			}
			if (vel_x < 0) {
				vel_x += friction;
				if (vel_x > 0) vel_x = 0;
			}
		}

		if (vel_x > max_speed) vel_x = max_speed;
		if (vel_x < -max_speed) vel_x = -max_speed;

		rect_container->offset_x += vel_x;

		if (rect_container->offset_x < 0) {
			rect_container->offset_x = 0;
			vel_x = 0;
		}
		if (rect_container->offset_x + rect->box.width > 800) {
			rect_container->offset_x = 800 - rect->box.width;
			vel_x = 0;
		}

		bool on_ground =
			rect_container->offset_y + rect->box.height >= floor->box.y;

		if (key_states[SDL_SCANCODE_W] && on_ground) {
			vel_y = -jump_strength;
		}

		vel_y += gravity;
		rect_container->offset_y += vel_y;

		if (rect_container->offset_y + rect->box.height >= floor->box.y) {
			rect_container->offset_y = floor->box.y - rect->box.height;
			vel_y = 0;
		}

		MalerElement *inner_rect = rect_container->elements[1];

		inner_rect->box.x += vel_x * 0.3f;

		if (inner_rect->box.x > rect->box.width - inner_rect->box.width - 10)
			inner_rect->box.x = rect->box.width - inner_rect->box.width - 10;
		if (inner_rect->box.x <= 10)
			inner_rect->box.x = 10;
	}

	return 0;
}
