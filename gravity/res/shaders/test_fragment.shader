#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 0) uniform float xRadius;
layout(location = 1) uniform float yRadius;
layout(location = 2) uniform vec2 center;

void main() {
    float x = coord.x - center.x;
    float y = coord.y - center.y;
    if (x * x / xRadius / xRadius + y * y / yRadius / yRadius <= 1)
        out_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else
        out_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
