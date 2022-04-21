#pragma once

#include "../Renderer.h"
#include "../../GL/GLHelper.h"
#include <string>
#include <vector>
#include "../../GL/GLTypes.h"

namespace mrko900::gravity::graphics::gl {
    class GLRenderer : public Renderer {
    public:
        struct Shaders {
            std::string testVertexShader;
            std::string testFragmentShader;
        };

        GLRenderer(mrko900::gravity::gl::GLHelper& glHelper, Shaders shaders);
        void init();
        void render() override;
        void addCircle(Circle circle) override;
        void removeCircle(Circle circle) override;
        void viewport(unsigned short viewportWidth, unsigned short viewportHeight) override;
        void coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) override;
    private:
        struct CircleDef {
            float x, y, xRadius, yRadius;
        };

        mrko900::gravity::gl::GLHelper& m_GLHelper;
        Shaders m_Shaders;
        std::vector<mrko900::gravity::gl::types::GLuint> m_CircleBuffers;
        std::vector<CircleDef> m_Circles;
        float m_CoordXBegin;
        float m_CoordXEnd;
        float m_CoordYBegin;
        float m_CoordYEnd;
    };
}
