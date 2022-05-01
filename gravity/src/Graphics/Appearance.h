#pragma once

#include "Texture.h"

namespace mrko900::gravity::graphics {
    enum class AppearanceAttribute {
        FILL_COLOR,
        OUTLINE,
        TEXTURE
    };

    struct RGBAColor {
        float r, g, b, a;
    };

    struct Outline {
        RGBAColor color;
        float width;
    };

    class Appearance {
    public:
        virtual ~Appearance();
        virtual bool hasAttribute(AppearanceAttribute attribute) const = 0;
        virtual void* getAttribute(AppearanceAttribute attribute) = 0;
        virtual RGBAColor& getFillColor();
        virtual Outline& getOutline();
        virtual Texture& getTexture();
    };
}
