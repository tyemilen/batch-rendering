#ifndef GRAPHIC_RECT_H_
#define GRAPHIC_RECT_H_

#define SHADER_RECT 0x1

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

uniform sampler2D uTexture;
uniform bool uUseTexture;

float sdbox(vec2 p, vec2 h, vec2 r) {
	r = min(r, h);
	vec2 q = abs(p) - h + r;
	return length(max(q, 0.0)) - min(r.x, r.y);
}

void main() {
	InstanceData inst = instances[vInstanceID];
	float a = 1.0;

	if (data[inst.data_offset + 4] > 0.0) {
		vec2 radius = vec2(data[inst.data_offset + 4]);
		vec2 h = 0.5 * uSize;
		vec2 p = vLocalPos;
		float d = sdbox(p, h, radius);
		float px = length(vec2(dFdx(d), dFdy(d)));
		a = 1.0 - smoothstep(-0.5, px, d);
	}

	vec4 color = vec4(
		data[inst.data_offset + 0],
		data[inst.data_offset + 1],
		data[inst.data_offset + 2],
		data[inst.data_offset + 3] * a
	);

	vec2 uv = (vLocalPos / uSize) + 0.5;
	
	if (uUseTexture) {
		vec4 texColor = texture(uTexture, uv);
		FragColor = texColor * a;
	} else {
		FragColor = color;
	}
}

)glsl";

#endif // GRAPHIC_RECT_H_
