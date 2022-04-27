#pragma once

#include "../Renderer.h"
#include "../../GL/GLHelper.h"
#include <string>
#include <unordered_map>
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
        void addCircle(unsigned int id, Circle& circle) override;
        void addRectangle(unsigned int id, Rectangle& rectangle) override;
        void removeFigure(unsigned int id) override;
        void refreshFigure(unsigned int id) override;
        void viewport(unsigned short viewportWidth, unsigned short viewportHeight) override;
        void coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) override;
        void setAutoGenTextureLevels(bool autoGenTextureLevels);
        bool isAutoGenTextureLevels();
        void setAutoGenTextureLevelsAfter(unsigned int autoGenTextureLevelsAfter);
        unsigned int getAutoGenTextureLevelsAfter();

    private:
        struct CircleDef {
            float x, y, xRadius, yRadius;
            Circle* origin;
            mrko900::gravity::gl::types::GLuint buffer;
            mrko900::gravity::gl::types::GLuint texture;
        };

        struct RectangleDef {
            Rectangle* origin;
        };

        union Def {
            CircleDef circleDef;
            RectangleDef rectangleDef;
        };

        struct Figure {
            FigureType type;
            Def def;
        };

        void refreshCircle(CircleDef& circle);

        mrko900::gravity::gl::GLHelper& m_GLHelper;
        Shaders m_Shaders;
        std::unordered_map<unsigned int, Figure> m_Figures;
        float m_CoordXBegin;
        float m_CoordXEnd;
        float m_CoordYBegin;
        float m_CoordYEnd;
        bool m_AutoGenLOD;
        unsigned int m_AutoGenLODAfter;
    };
}
