#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 0) uniform float u_Width;
layout(location = 1) uniform float u_Height;
layout(location = 3) uniform uint u_AppearanceMode;
layout(location = 4) uniform vec4 u_PlainColor;
layout(location = 5) uniform uint u_OutlineMode;
layout(location = 6) uniform float u_OutlineWidth;
layout(location = 7) uniform vec4 u_OutlineColor;
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
    if (u_AppearanceMode == 1) {
        out_Color = u_PlainColor;
        if (u_OutlineMode == 1) {
            // inner outline
            if (u_Width - abs(coord.x) <= u_OutlineWidth || u_Height - abs(coord.y) <= u_OutlineWidth)
                out_Color = u_OutlineColor;
        }
    } else if (u_AppearanceMode == 2)
        out_Color = texture(u_Texture, gl_FragCoord.xy / textureSize(u_Texture, 0));
}
