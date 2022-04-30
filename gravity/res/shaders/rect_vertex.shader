#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) smooth out vec2 coord;

out gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

void main() {
    gl_Position = vec4(position, 0.0f, 1.0f);
    coord = vec2(position.x, position.y);
}
