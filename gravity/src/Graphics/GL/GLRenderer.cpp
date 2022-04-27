#include "GLRenderer.h"

#include <iostream>
#include "../../GL/GLMacros.h"
#include "../../GL/GLTypes.h"
#include "GLTextureBuffer.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;
using namespace mrko900::gravity::gl::types;

using enum mrko900::gravity::graphics::FigureType;

namespace mrko900::gravity::graphics::gl {
    GLRenderer::GLRenderer(GLHelper& glHelper, Shaders shaders) : m_GLHelper(glHelper), m_Shaders(std::move(shaders)),
        m_CoordXBegin(0.0f), m_CoordXEnd(0.0f), m_CoordYBegin(0.0f), m_CoordYEnd(0.0f), m_AutoGenLOD(false),
        m_AutoGenLODAfter(0) {
    }

    void GLRenderer::init() {
        GLuint vao;
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint program = glCreateProgram();
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexShaderSrc = m_Shaders.testVertexShader.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(vertexShader);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentShaderSrc = m_Shaders.testFragmentShader.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
        glCompileShader(fragmentShader);

        GLchar log[1024];
        GLsizei ptr;
        glGetShaderInfoLog(vertexShader, 1024, &ptr, log);
        std::cout << ptr << " <- size shader log (vertex shader): " << log << '\n';
        glGetShaderInfoLog(fragmentShader, 1024, &ptr, log);
        std::cout << ptr << " <- size shader log (fragment shader): " << log << '\n';

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glDeleteShader(vertexShader);
        glUseProgram(program);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        std::cout << "gl error: " << glGetError() << '\n';
    }

    void GLRenderer::render() {
        float bgColor[] = { 0.5f, 0.3f, 0.7f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, bgColor);
        for (const auto& entry : m_Figures) {
            unsigned int id = entry.first;

            if (entry.second.type == CIRCLE) {
#define circle entry.second.def.circleDef
#define circlePlainColor ((PlainColor*) circle.origin->appearance.ptr)
                glUniform1f(0, circle.xRadius);
                glUniform1f(1, circle.yRadius);
                glUniform2f(2, circle.x, circle.y);
                if (circle.origin->appearance.type == AppearanceType::PLAIN_COLOR) {
                    glUniform1ui(3, 1);
                    glUniform4f(4, circlePlainColor->r, circlePlainColor->g, circlePlainColor->b, circlePlainColor->a);
                } else {
                    glUniform1ui(3, 2);
                    glBindTextureUnit(0, circle.texture);
                }
                glBindVertexBuffer(0, circle.buffer, 0, 2 * sizeof(GLfloat));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#undef circle
#undef circlePlainColor
            }
        }
    }

    void GLRenderer::addCircle(unsigned int id, Circle& circle) {
        float x = (circle.x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
        float y = (circle.y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
        float xRadius = 2 * circle.radius / (m_CoordXEnd - m_CoordXBegin);
        float yRadius = 2 * circle.radius / (m_CoordYEnd - m_CoordYBegin);
        GLfloat positions[] {
            x - xRadius, y - yRadius,
            x - xRadius, y + yRadius,
            x + xRadius, y - yRadius,
            x + xRadius, y + yRadius
        };

        GLuint buf;
        glCreateBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferStorage(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), positions, GL_DYNAMIC_STORAGE_BIT);
        glVertexAttribBinding(0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, buf, 0, 2 * sizeof(GLfloat));

        GLuint texture;
        if (circle.appearance.type == AppearanceType::TEXTURE) {
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            GLTextureBuffer textureBuffer(m_GLHelper);
#define texture (*((Texture*) circle.appearance.ptr))
            for (unsigned int level = 0; level < texture.levels(); ++level) {
                texture.writeLevel(level, textureBuffer);
                if (m_AutoGenLOD && level == m_AutoGenLODAfter) {
                    glGenerateMipmap(GL_TEXTURE_2D);
                    break;
                }
            }
#undef texture
        }

        Def def;
        def.circleDef = CircleDef(x, y, xRadius, yRadius, &circle, buf, texture);
        m_Figures.insert(std::make_pair(id, Figure(CIRCLE, def)));
    }

    void GLRenderer::addRectangle(unsigned int id, Rectangle& rectangle) {

    }

    void GLRenderer::removeFigure(unsigned int id) {
        Figure& figure = m_Figures[id];
        switch (figure.type) {
            case CIRCLE:
                glDeleteBuffers(1, &figure.def.circleDef.buffer);
                break;
        }
        m_Figures.erase(id);
    }

    void GLRenderer::refreshFigure(unsigned int id) {
        Figure& figure = m_Figures[id];

        if (figure.type == CIRCLE)
            refreshCircle(figure.def.circleDef);
    }

    void GLRenderer::refreshCircle(CircleDef& circle) {
        circle.x = (circle.origin->x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
        circle.y = (circle.origin->y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
        circle.xRadius = 2 * circle.origin->radius / (m_CoordXEnd - m_CoordXBegin);
        circle.yRadius = 2 * circle.origin->radius / (m_CoordYEnd - m_CoordYBegin);
        GLfloat positions[] {
            circle.x - circle.xRadius, circle.y - circle.yRadius,
            circle.x - circle.xRadius, circle.y + circle.yRadius,
            circle.x + circle.xRadius, circle.y - circle.yRadius,
            circle.x + circle.xRadius, circle.y + circle.yRadius
        };
        glBindBuffer(GL_ARRAY_BUFFER, circle.buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), positions);
    }

    void GLRenderer::viewport(unsigned short viewportWidth, unsigned short viewportHeight) {
        glViewport(0, 0, viewportWidth, viewportHeight);
    }

    void GLRenderer::coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) {
        m_CoordXBegin = xBegin;
        m_CoordXEnd = xEnd;
        m_CoordYBegin = yBegin;
        m_CoordYEnd = yEnd;

        // update circles
        for (auto& entry : m_Figures) {
            if (entry.second.type != CIRCLE)
                continue;

            CircleDef& circle = entry.second.def.circleDef;

            circle.x = (circle.origin->x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
            circle.y = (circle.origin->y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
            float xRadius = 2 * circle.origin->radius / (m_CoordXEnd - m_CoordXBegin);
            float yRadius = 2 * circle.origin->radius / (m_CoordYEnd - m_CoordYBegin);
            GLfloat positions[] {
                circle.x - xRadius, circle.y - yRadius,
                circle.x - xRadius, circle.y + yRadius,
                circle.x + xRadius, circle.y - yRadius,
                circle.x + xRadius, circle.y + yRadius
            };

            glBindBuffer(GL_ARRAY_BUFFER, circle.buffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), positions);

            circle.xRadius = xRadius;
            circle.yRadius = yRadius;
        }
    }

    void GLRenderer::setAutoGenTextureLevels(bool autoGenTextureLevels) {
        m_AutoGenLOD = autoGenTextureLevels;
    }

    bool GLRenderer::isAutoGenTextureLevels() {
        return m_AutoGenLOD;
    }
}
