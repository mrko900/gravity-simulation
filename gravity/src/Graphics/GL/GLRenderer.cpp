#include "GLRenderer.h"

#include <iostream>
#include "../../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;

namespace mrko900::gravity::graphics::gl {
    GLRenderer::GLRenderer(GLHelper& glHelper) : m_GLHelper(glHelper) {
    }

    void GLRenderer::render() {
        std::cout << "Rendering length " << lenCircles() << '\n';
        std::cout << "gl version: " << glGetString(GL_VERSION) << '\n';
    }
}
