#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) smooth out vec2 coord;

void main() {
    gl_Position = vec4(position, 0.5f, 1.0f);
    coord = vec2(position.x, position.y);
}
