#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 0) uniform float u_XRadius;
layout(location = 1) uniform float u_YRadius;
layout(location = 2) uniform vec2 u_Center;
layout(location = 3) uniform uint u_AppearanceMode;
layout(location = 4) uniform vec4 u_PlainColor;
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
    float x = coord.x - u_Center.x;
    float y = coord.y - u_Center.y;
    if ((x * x) / (u_XRadius * u_XRadius) + (y * y) / (u_YRadius * u_YRadius) <= 1) {
        if (u_AppearanceMode == 1)
            out_Color = u_PlainColor;
        else if (u_AppearanceMode == 2)
            out_Color = texture(u_Texture, gl_FragCoord.xy / textureSize(u_Texture, 0));
    } else
        out_Color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
