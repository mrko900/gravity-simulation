#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;

namespace mrko900::gravity::app {

    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer) : m_Renderer(renderer),
        m_ViewportUpdateRequested(false), m_ViewportNewWidth(0), m_ViewportNewHeight(0) {
    }

    void ProgramLoop::init() {
        Circle* circle = new Circle(0.0f, 0.0f, 0.7f, Appearance(0.0f, 0.0f, 0.0f, 0.0f));
        Circle* circle2 = new Circle(0.8f, -0.4f, 0.15f, Appearance(0.0f, 0.0f, 0.0f, 0.0f));
        m_Renderer.addCircle(3, *circle);
        m_Renderer.addCircle(0, *circle2);
    }

    void ProgramLoop::run() {
        if (m_ViewportUpdateRequested) {
            m_Renderer.viewport(m_ViewportNewWidth, m_ViewportNewHeight);
            m_ViewportUpdateRequested = false;
        }

        m_Renderer.render();
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }

    void ProgramLoop::userInput(UserInput input, void* data) {

    }
}
