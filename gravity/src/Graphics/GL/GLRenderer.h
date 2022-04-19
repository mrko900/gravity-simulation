#pragma once

#include "../RendererBase.h"
#include "../../GL/GLHelper.h"

namespace mrko900::gravity::graphics::gl {
    class GLRenderer : public RendererBase {
    public:
        GLRenderer(mrko900::gravity::gl::GLHelper& glHelper);
        void render() override;

    private:
        mrko900::gravity::gl::GLHelper& m_GLHelper;
    };
}
