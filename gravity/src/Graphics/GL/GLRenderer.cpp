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

        GLuint rectVertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* rectVertexShaderSrc = m_Shaders.rectVertexShader.c_str();
        glShaderSource(rectVertexShader, 1, &rectVertexShaderSrc, nullptr);
        glCompileShader(rectVertexShader);
        m_RectVertexShaderProgram = glCreateProgram();
        glAttachShader(m_RectVertexShaderProgram, rectVertexShader);
        glProgramParameteri(m_RectVertexShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glLinkProgram(m_RectVertexShaderProgram);

        //GLuint simpleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        //const char* simpleFragmentShaderSrc = m_Shaders.simpleFragmentShader.c_str();
        //glShaderSource(simpleFragmentShader, 1, &simpleFragmentShaderSrc, nullptr);
        //glCompileShader(simpleFragmentShader);
        //GLuint m_SimpleFragmentShaderProgram = glCreateProgram();
        //glAttachShader(m_SimpleFragmentShaderProgram, simpleFragmentShader);
        //glProgramParameteri(m_SimpleFragmentShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        //glLinkProgram(m_SimpleFragmentShaderProgram);

        GLuint circleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* circleFragmentShaderSrc = m_Shaders.circleFragmentShader.c_str();
        glShaderSource(circleFragmentShader, 1, &circleFragmentShaderSrc, nullptr);
        glCompileShader(circleFragmentShader);
        m_CircleFragmentShaderProgram = glCreateProgram();
        glAttachShader(m_CircleFragmentShaderProgram, circleFragmentShader);
        glProgramParameteri(m_CircleFragmentShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glLinkProgram(m_CircleFragmentShaderProgram);

        glCreateProgramPipelines(1, &m_ProgramPipeline);
        glBindProgramPipeline(m_ProgramPipeline);

        GLchar log[1024];
        GLsizei ptr;
        glGetShaderInfoLog(rectVertexShader, 1024, &ptr, log);
        std::cout << ptr << " <- size shader log (vertex shader): " << log << '\n';
        glGetShaderInfoLog(circleFragmentShader, 1024, &ptr, log);
        std::cout << ptr << " <- size shader log (fragment shader): " << log << '\n';
        glGetProgramInfoLog(m_CircleFragmentShaderProgram, 1024, &ptr, log);
        std::cout << ptr << " <- size program log (fragment shader program): " << log << '\n';
        glGetProgramInfoLog(m_RectVertexShaderProgram, 1024, &ptr, log);
        std::cout << ptr << " <- size program log (vertex shader program): " << log << '\n';

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        std::cout << "gl error: " << glGetError() << '\n';
    }
    static bool first = true;
    void GLRenderer::render() {
        float bgColor[] = { 0.5f, 0.3f, 0.7f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, bgColor);
        
        for (const auto& entry : m_Figures) {
            unsigned int id = entry.first;

            if (entry.second.type == CIRCLE) {
                glUseProgramStages(m_ProgramPipeline, GL_VERTEX_SHADER_BIT, m_RectVertexShaderProgram);
                glUseProgramStages(m_ProgramPipeline, GL_FRAGMENT_SHADER_BIT, m_CircleFragmentShaderProgram);

#define circle entry.second.def.circleDef
#define circlePlainColor ((PlainColor*) circle.origin->appearance.ptr)
                glProgramUniform1f(m_CircleFragmentShaderProgram, 0, circle.xRadius);
                glProgramUniform1f(m_CircleFragmentShaderProgram, 1, circle.yRadius);
                glProgramUniform2f(m_CircleFragmentShaderProgram, 2, circle.x, circle.y);
                if (circle.origin->appearance.type == AppearanceType::PLAIN_COLOR) {
                    glProgramUniform1ui(m_CircleFragmentShaderProgram, 3, 1);
                    glProgramUniform4f(m_CircleFragmentShaderProgram, 4, circlePlainColor->r, 
                                       circlePlainColor->g, circlePlainColor->b, circlePlainColor->a);
                } else {
                    glProgramUniform1ui(m_CircleFragmentShaderProgram, 3, 2);
                    glBindTextureUnit(0, circle.texture);
                }
                
                glBindVertexBuffer(0, circle.buffer, 0, 2 * sizeof(GLfloat));
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                //if (first) std::cout << "fu" << glGetError() << '\n';
#undef circle
#undef circlePlainColor
            } else if (entry.second.type == RECTANGLE) {
#define circle entry.second.def.rectangleDef
#define circlePlainColor ((PlainColor*) circle.origin->appearance.ptr)
                glUniform1f(0, circle.xHalf);
                glUniform1f(1, circle.yHalf);
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

        first = false;
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

        GLuint texture;
        if (circle.appearance.type == AppearanceType::TEXTURE) {
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            GLTextureBuffer textureBuffer(m_GLHelper);
#define txt (*((Texture*) circle.appearance.ptr))
            for (unsigned int level = 0; level < txt.levels(); ++level) {
                txt.writeLevel(level, textureBuffer);
                if (m_AutoGenLOD && level == m_AutoGenLODAfter) {
                    glGenerateMipmap(GL_TEXTURE_2D);
                    break;
                }
            }
#undef txt
        } else
            texture = 0;

        Figure figure = Figure(CIRCLE, Def());
        figure.def.circleDef = CircleDef { x, y, xRadius, yRadius, &circle, buf, texture };
        m_Figures.insert(std::make_pair(id, figure));
    }

    void GLRenderer::addRectangle(unsigned int id, Rectangle& rectangle) {
        float x = (rectangle.x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
        float y = (rectangle.y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
        float xHalf = rectangle.width / 2;
        float yHalf = rectangle.height / 2;
        GLfloat positions[] {
            x - xHalf, y - yHalf,
            x - xHalf, y + yHalf,
            x + xHalf, y - yHalf,
            x + xHalf, y + yHalf
        };

        GLuint buf;
        glCreateBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferStorage(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), positions, GL_DYNAMIC_STORAGE_BIT);
        glVertexAttribBinding(0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);

        GLuint texture;
        if (rectangle.appearance.type == AppearanceType::TEXTURE) {
            glCreateTextures(GL_TEXTURE_2D, 1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            GLTextureBuffer textureBuffer(m_GLHelper);
#define txt (*((Texture*) rectangle.appearance.ptr))
            for (unsigned int level = 0; level < txt.levels(); ++level) {
                txt.writeLevel(level, textureBuffer);
                if (m_AutoGenLOD && level == m_AutoGenLODAfter) {
                    glGenerateMipmap(GL_TEXTURE_2D);
                    break;
                }
            }
#undef txt
        } else
            texture = 0;

        Figure figure = Figure(RECTANGLE, Def());
        figure.def.rectangleDef = RectangleDef { x, y, xHalf, yHalf, &rectangle, buf, texture };
        m_Figures.insert(std::make_pair(id, figure));
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
        else if (figure.type == RECTANGLE)
            refreshRectangle(figure.def.rectangleDef);
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

    void GLRenderer::refreshRectangle(RectangleDef& rectangle) {
    }

    void GLRenderer::viewport(unsigned short viewportWidth, unsigned short viewportHeight) {
        glViewport(0, 0, viewportWidth, viewportHeight);
    }

    void GLRenderer::coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) {
        m_CoordXBegin = xBegin;
        m_CoordXEnd = xEnd;
        m_CoordYBegin = yBegin;
        m_CoordYEnd = yEnd;
    }

    void GLRenderer::setAutoGenTextureLevels(bool autoGenTextureLevels) {
        m_AutoGenLOD = autoGenTextureLevels;
    }

    bool GLRenderer::isAutoGenTextureLevels() {
        return m_AutoGenLOD;
    }
}
