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
        for (int i = 0; i < m_CircleBuffers.size(); ++i) {
            glUniform1f(0, m_Circles[i].xRadius);
            glUniform1f(1, m_Circles[i].yRadius);
            glUniform2f(2, m_Circles[i].x, m_Circles[i].y);
            glBindVertexBuffer(0, m_CircleBuffers[i], 0, 2 * sizeof(GLfloat));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    void GLRenderer::addCircle(Circle circle) {
        float initialX = circle.x;
        float initialY = circle.y;
        circle.x = (circle.x - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
        circle.y = (circle.y - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
        float xRadius = circle.radius / (m_CoordXEnd - m_CoordXBegin) * 2;
        float yRadius = circle.radius / (m_CoordYEnd - m_CoordYBegin) * 2;
        GLfloat positions[] {
            circle.x - xRadius, circle.y - yRadius,
            circle.x - xRadius, circle.y + yRadius,
            circle.x + xRadius, circle.y - yRadius,
            circle.x + xRadius, circle.y + yRadius
        };

        GLuint index = m_CircleBuffers.size();
        m_CircleBuffers.push_back(0);
        GLuint* bufptr = &m_CircleBuffers[0] + index;
        glCreateBuffers(1, bufptr);
        glBindBuffer(GL_ARRAY_BUFFER, *bufptr);
        glBufferStorage(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), positions, 0);
        glVertexAttribBinding(0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, *bufptr, 0, 2 * sizeof(GLfloat));

        m_Circles.push_back(CircleDef(circle.x, circle.y, xRadius, yRadius, initialX, initialY, circle.radius));
    }

    void GLRenderer::removeCircle(Circle circle) {

    }

    void GLRenderer::viewport(unsigned short viewportWidth, unsigned short viewportHeight) {
        glViewport(0, 0, viewportWidth, viewportHeight);
    }

    void GLRenderer::coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) {
        m_CoordXBegin = xBegin;
        m_CoordXEnd = xEnd;
        m_CoordYBegin = yBegin;
        m_CoordYEnd = yEnd;

        int i = 0;
        for (GLuint buffer : m_CircleBuffers) {
            CircleDef& circle = m_Circles[i++];

            circle.x = (circle.initialX - m_CoordXBegin) / (m_CoordXEnd - m_CoordXBegin) * 2 - 1;
            circle.y = (circle.initialY - m_CoordYBegin) / (m_CoordYEnd - m_CoordYBegin) * 2 - 1;
            float xRadius = circle.initialRadius / (m_CoordXEnd - m_CoordXBegin) * 2;
            float yRadius = circle.initialRadius / (m_CoordYEnd - m_CoordYBegin) * 2;
            GLfloat positions[] {
                circle.x - xRadius, circle.y - yRadius,
                circle.x - xRadius, circle.y + yRadius,
                circle.x + xRadius, circle.y - yRadius,
                circle.x + xRadius, circle.y + yRadius
            };

            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(GLfloat), positions);

            circle.xRadius = xRadius;
            circle.yRadius = yRadius;
        }
    }
}
