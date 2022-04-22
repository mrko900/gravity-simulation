#include "GLRenderer.h"

#include <iostream>
#include "../../GL/GLMacros.h"
#include "../../GL/GLTypes.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;
using namespace mrko900::gravity::gl::types;

namespace mrko900::gravity::graphics::gl {
    GLRenderer::GLRenderer(GLHelper& glHelper, Shaders shaders) : m_GLHelper(glHelper), m_Shaders(std::move(shaders)) {
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
        for (const auto& entry : m_Circles) {
            unsigned int id = entry.first;
            glUniform1f(0, m_Circles[id].xRadius);
            glUniform1f(1, m_Circles[id].yRadius);
            glUniform2f(2, m_Circles[id].x, m_Circles[id].y);
            glBindVertexBuffer(0, m_Circles[id].buffer, 0, 2 * sizeof(GLfloat));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    void GLRenderer::addCircle(unsigned int id, Circle circle) {
        float initialX = circle.x;
        float initialY = circle.y;
        circle.x = (circle.x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
        circle.y = (circle.y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
        float xRadius = 2 * circle.radius / (m_CoordXEnd - m_CoordXBegin);
        float yRadius = 2 * circle.radius / (m_CoordYEnd - m_CoordYBegin);
        GLfloat positions[] {
            circle.x - xRadius, circle.y - yRadius,
            circle.x - xRadius, circle.y + yRadius,
            circle.x + xRadius, circle.y - yRadius,
            circle.x + xRadius, circle.y + yRadius
        };

        GLuint buf;
        glCreateBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);
        glBufferStorage(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), positions, GL_DYNAMIC_STORAGE_BIT);
        glVertexAttribBinding(0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, buf, 0, 2 * sizeof(GLfloat));

        m_Circles.insert(std::make_pair(id, CircleDef(circle.x, circle.y, 
            xRadius, yRadius, initialX, initialY, circle.radius, buf)));
    }

    void GLRenderer::removeCircle(unsigned int id) {
        glDeleteBuffers(1, &m_Circles[id].buffer);
        m_Circles.erase(id);
    }

    void GLRenderer::viewport(unsigned short viewportWidth, unsigned short viewportHeight) {
        glViewport(0, 0, viewportWidth, viewportHeight);
    }

    void GLRenderer::coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) {
        m_CoordXBegin = xBegin;
        m_CoordXEnd = xEnd;
        m_CoordYBegin = yBegin;
        m_CoordYEnd = yEnd;

        for (auto& entry : m_Circles) {
            CircleDef& circle = entry.second;

            circle.x = (circle.initialX - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
            circle.y = (circle.initialY - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
            float xRadius = 2 * circle.initialRadius / (m_CoordXEnd - m_CoordXBegin);
            float yRadius = 2 * circle.initialRadius / (m_CoordYEnd - m_CoordYBegin);
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
}
