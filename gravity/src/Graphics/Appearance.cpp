#include "Appearance.h"

namespace mrko900::gravity::graphics {
    Appearance::~Appearance() = default;
    
    RGBAColor& Appearance::getFillColor() {
        return *((RGBAColor*) getAttribute(AppearanceAttribute::FILL_COLOR));
    }

    Outline& Appearance::getOutline() {
        return *((Outline*) getAttribute(AppearanceAttribute::OUTLINE));
    }

    Texture& Appearance::getTexture() {
        return *((Texture*) getAttribute(AppearanceAttribute::TEXTURE));
    }
}
