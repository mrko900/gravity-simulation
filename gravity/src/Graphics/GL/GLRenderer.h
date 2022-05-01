#pragma once

#include "../Renderer.h"
#include "../../GL/GLHelper.h"
#include <string>
#include <unordered_map>
#include <set>
#include "../../GL/GLTypes.h"

namespace mrko900::gravity::graphics::gl {
    class GLRenderer : public Renderer {
    public:
        struct Shaders {
            std::string rectVertexShader;
            std::string simpleFragmentShader;
            std::string circleFragmentShader;
        };

        GLRenderer(mrko900::gravity::gl::GLHelper& glHelper, Shaders shaders);
        void init();
        void render() override;
        void addCircle(unsigned int id, Circle& circle) override;
        void replaceCircle(unsigned int id, Circle& circle) override;
        void addRectangle(unsigned int id, Rectangle& rectangle) override;
        void replaceRectangle(unsigned int id, Rectangle& rectangle) override;
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
            float x, y, xHalf, yHalf;
            Rectangle* origin;
            mrko900::gravity::gl::types::GLuint buffer;
            mrko900::gravity::gl::types::GLuint texture;
        };

        union Def {
            CircleDef circleDef;
            RectangleDef rectangleDef;
        };

        struct Figure {
            FigureType type;
            Def def;
        };

        struct LayerCmp {
            bool operator()(std::pair<unsigned int, int> a, std::pair<unsigned int, int> b) const {
                return a.second < b.second;
            }
        };

        void refreshCircle(CircleDef& circle);
        void refreshRectangle(RectangleDef& rectangle);

        mrko900::gravity::gl::GLHelper& m_GLHelper;
        Shaders m_Shaders;
        std::unordered_map<unsigned int, Figure> m_Figures;
        std::multiset<std::pair<unsigned int, int>, LayerCmp> m_Layers;

        float m_CoordXBegin;
        float m_CoordXEnd;
        float m_CoordYBegin;
        float m_CoordYEnd;

        bool m_AutoGenLOD;
        unsigned int m_AutoGenLODAfter;

        mrko900::gravity::gl::types::GLuint m_ProgramPipeline;
        mrko900::gravity::gl::types::GLuint m_RectVertexShaderProgram;
        mrko900::gravity::gl::types::GLuint m_SimpleFragmentShaderProgram;
        mrko900::gravity::gl::types::GLuint m_CircleFragmentShaderProgram;
    };
}
