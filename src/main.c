#include "bglib.h"
#include "maler.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <stdbool.h>

const char *fragment_shader_rect = R"glsl(
#version 430 core

flat in uint vInstanceID;
in vec2 vLocalPos;
in vec2 uSize;
out vec4 FragColor;

struct InstanceData {
	vec4 box;
	uint data_offset;
	uint data_count;
	uint padding0;
	uint padding1;
};

layout(std430, binding = 0) buffer Instances { InstanceData instances[]; };
layout(std430, binding = 1) buffer ElementData { float data[]; };

float sdbox(vec2 p, vec2 h, vec2 r) {
	r = min(r, h);
	vec2 q = abs(p) - h + r;
	return length(max(q, 0.0)) - min(r.x, r.y);
}

void main() {
	InstanceData inst = instances[vInstanceID];
	float a = 1.0f;

	if (data[inst.data_offset + 4] > 0.0f) {
		vec2 radius = vec2(data[inst.data_offset + 4]);
		vec2 h = 0.5 * uSize;
		vec2 p = vLocalPos;

		float d = sdbox(p, h, radius);

		float px = length(vec2(dFdx(d), dFdy(d)));
		a = 1.0 - smoothstep(-0.5, px, d);
	}

	vec4 color = vec4(
		data[inst.data_offset],
		data[inst.data_offset + 1],
		data[inst.data_offset + 2],
		data[inst.data_offset + 3] * a
	);

	FragColor = color;
}

)glsl";

int main(void) {
	bg_init("cool window", 800, 600);

	float rect_data[5] = {1.0f, 0.6f, 0.6f, 1.0f, 0.0f};
	float rect_data_2[5] = {1.0f, 0.8f, 0.6f, 1.0f, 10.0f};
	float floor_data[5] = {0.8f, 0.7f, 1.0f, 1.0f, 0.0f};

	bg_registry_shader(0, (char *)fragment_shader_rect);

	MalerElement *rect =
		bg_create((MalerBox){0, 0, 120, 120}, (void *)rect_data, 5, 0, 0);
	bg_create((MalerBox){29, 30, 60, 60}, (void *)rect_data_2, 5, 0, 0);
	MalerElement *floor =
		bg_create((MalerBox){0, 540, 800, 120}, (void *)floor_data, 5, 0, 1);

	float vel_x = 0.0f;
	float vel_y = 0.0f;

	const float accel = 1.5f;
	const float max_speed = 15.0f;
	const float friction = 0.3f;
	const float gravity = 0.5f;
	const float jump_strength = 12.0f;

	MalerLayer *rect_layer = bg_get_layer(0);

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

		rect_layer->offset_x += vel_x;

		if (rect_layer->offset_x < 0) {
			rect_layer->offset_x = 0;
			vel_x = 0;
		}
		if (rect_layer->offset_x + rect->box.width > 800) {
			rect_layer->offset_x = 800 - rect->box.width;
			vel_x = 0;
		}

		bool on_ground =
			rect_layer->offset_y + rect->box.height >= floor->box.y;

		if (key_states[SDL_SCANCODE_W] && on_ground) {
			vel_y = -jump_strength;
		}

		vel_y += gravity;
		rect_layer->offset_y += vel_y;

		if (rect_layer->offset_y + rect->box.height >= floor->box.y) {
			rect_layer->offset_y = floor->box.y - rect->box.height;
			vel_y = 0;
		}

		MalerElement *inner_rect = rect_layer->elements[1];

		inner_rect->box.x += vel_x * 0.3f;

		if (inner_rect->box.x > rect->box.width - inner_rect->box.width - 10)
			inner_rect->box.x = rect->box.width - inner_rect->box.width - 10;
		if (inner_rect->box.x <= 10)
			inner_rect->box.x = 10;
	}

	return 0;
}
