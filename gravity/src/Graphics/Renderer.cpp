#include "Renderer.h"

namespace mrko900::gravity::graphics {
    Renderer::~Renderer() = default;

    PlainColor& Appearance::plainColor() const {
        return *((PlainColor*) ptr);
    }

    Texture& Appearance::texture() const {
        return *((Texture*) ptr);
    }
}
