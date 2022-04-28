#include "Renderer.h"

namespace mrko900::gravity::graphics {
    Renderer::~Renderer() = default;

    PlainColor& Appearance::plainColor() {
        return *((PlainColor*) ptr);
    }

    Texture& Appearance::texture() {
        return *((Texture*) ptr);
    }
}
