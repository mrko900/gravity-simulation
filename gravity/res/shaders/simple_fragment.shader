#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 3) uniform uint u_AppearanceMode;
layout(location = 4) uniform vec4 u_PlainColor;
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
    if (u_AppearanceMode == 1)
        out_Color = u_PlainColor;
    else if (u_AppearanceMode == 2)
        out_Color = texture(u_Texture, gl_FragCoord.xy / textureSize(u_Texture, 0));
}
