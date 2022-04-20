#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 0) uniform float radius;
layout(location = 1) uniform vec2 center;

void main() {
    float dist = distance(center, coord);
    if (dist <= radius)
        out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        out_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
