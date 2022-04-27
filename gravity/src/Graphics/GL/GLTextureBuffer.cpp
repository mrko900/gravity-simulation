#include "GLTextureBuffer.h"

#include "../../GL/GLMacros.h"
#include <stdexcept>

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;

using namespace mrko900::gravity::gl::types;

namespace mrko900::gravity::graphics::gl {
    GLTextureBuffer::GLTextureBuffer(GLHelper& glHelper) : m_GLHelper(glHelper) {
    }

    void GLTextureBuffer::alloc(unsigned int levels, unsigned int width, unsigned int height) {
        glTexStorage2D(GL_TEXTURE_2D, levels, GL_RGBA32F, width, height);
    }

    void GLTextureBuffer::write(unsigned int level, unsigned int x, unsigned int y, 
                                unsigned int width, unsigned int height, const float* pixels) {
        glTexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, GL_RGBA, GL_FLOAT, pixels);
    }
}
