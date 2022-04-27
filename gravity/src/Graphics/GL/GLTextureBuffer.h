#pragma once

#include "../Texture.h"
#include "../../GL/GLHelper.h"
#include "../../GL/GLTypes.h"

namespace mrko900::gravity::graphics::gl {
    class GLTextureBuffer : public TextureBuffer {
    public:
        GLTextureBuffer(mrko900::gravity::gl::GLHelper& glHelper);
        void alloc(unsigned int levels, unsigned int width, unsigned int height) override;
        void write(unsigned int level, unsigned int x, unsigned int y, unsigned int width, unsigned int height, 
                   const float* pixels) override;

    private:
        mrko900::gravity::gl::GLHelper& m_GLHelper;
    };
}
