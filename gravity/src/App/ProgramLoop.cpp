#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer) : m_Renderer(renderer),
        m_ViewportUpdateRequested(false), m_ViewportNewWidth(0), m_ViewportNewHeight(0) {
    }

    void ProgramLoop::operator()() {
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

    void ProgramLoop::test_addObj(float x, float y, float radius, unsigned int id) {
        Circle circle { x, y, radius, { 1.0f, 0.0f, 0.5f, 1.0f } };
        m_Renderer.addCircle(id, std::move(circle));
    }

    void ProgramLoop::test_rmObj(unsigned int id) {
        m_Renderer.removeCircle(id);
    }
}
