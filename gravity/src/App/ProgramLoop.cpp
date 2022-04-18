#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop(GLHelper& glHelper) : m_GLHelper(glHelper), 
        m_ViewportUpdateRequested(false), m_ViewportNewWidth(0), m_ViewportNewHeight(0) {
    }

    void ProgramLoop::operator()() {
        if (m_ViewportUpdateRequested) {
            std::cout << "gl version: " << glGetString(GL_VERSION) << '\n';
            std::cout << "viewport update requested: w " << m_ViewportNewWidth << " h " << m_ViewportNewHeight << '\n';
            m_ViewportUpdateRequested = false;
        }
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }
}
