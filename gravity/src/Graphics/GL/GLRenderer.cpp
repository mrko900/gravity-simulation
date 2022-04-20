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
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void GLRenderer::addCircle(Circle& circle) {
        glUniform1f(0, circle.radius);
        glUniform2f(1, circle.x, circle.y);

        GLfloat positions[] {
            circle.x - circle.radius, circle.y - circle.radius,
            circle.x - circle.radius, circle.y + circle.radius,
            circle.x + circle.radius, circle.y - circle.radius,
            circle.x + circle.radius, circle.y + circle.radius
        };

        GLuint posBuffer;
        glCreateBuffers(1, &posBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glBufferStorage(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), positions, 0);
        glVertexAttribBinding(0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, posBuffer, 0, 2 * sizeof(GLfloat));
    }

    void GLRenderer::removeCircle(Circle& circle) {

    }

    void GLRenderer::viewport(unsigned short viewportWidth, unsigned short viewportHeight) {
        glViewport(0, 0, viewportWidth, viewportHeight);
    }
}
