#version 460 core

layout(location = 1) in vec2 coord;
layout(location = 0) out vec4 out_Color;
layout(location = 0) uniform float u_XRadius;
layout(location = 1) uniform float u_YRadius;
layout(location = 2) uniform vec2 u_Center;
layout(location = 3) uniform uint u_AppearanceMode;
layout(location = 4) uniform vec4 u_PlainColor;
layout(location = 5) uniform uint u_OutlineMode;
layout(location = 6) uniform float u_OutlineWidth;
layout(location = 7) uniform vec4 u_OutlineColor;
layout(binding = 0) uniform sampler2D u_Texture;

void main() {
    float x = coord.x - u_Center.x;
    float y = coord.y - u_Center.y;
    float dist = (x * x) / (u_XRadius * u_XRadius) + (y * y) / (u_YRadius * u_YRadius);
    if (dist <= 1) {
        if (u_AppearanceMode == 1) {
            out_Color = u_PlainColor;
            if (u_OutlineMode == 1) {
                // inner outline
                float xoutline = u_OutlineWidth;
                float youtline = xoutline * (u_YRadius / u_XRadius);
                float outlineDist = (x * x) / ((u_XRadius - xoutline) * (u_XRadius - xoutline))
                    + (y * y) / ((u_YRadius - youtline) * (u_YRadius - youtline));
                if (outlineDist > 1.0f)
                    out_Color = u_OutlineColor;
            }
        } else if (u_AppearanceMode == 2) {
            out_Color = texture(u_Texture, gl_FragCoord.xy / textureSize(u_Texture, 0));
        }
    } else {
        discard;
    }
}
