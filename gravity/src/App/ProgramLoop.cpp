#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;

namespace mrko900::gravity::app {

    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer) : m_Renderer(renderer),
        m_ViewportUpdateRequested(false), m_ViewportNewWidth(0), m_ViewportNewHeight(0) {
    }

    void ProgramLoop::operator()() {
        if (m_ViewportUpdateRequested) {
            m_Renderer.viewport(m_ViewportNewWidth, m_ViewportNewHeight);
            m_ViewportUpdateRequested = false;
        }

        m_Renderer.render();
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }

    void ProgramLoop::userInput(UserInput input, void* data) {
        if (input == KEY_PRESSED || input == KEY_RELEASED) {
            KeyboardInputData dat = *((KeyboardInputData*) data);
            if (dat == SPACE) {
                std::cout << "Space pressed or released\n";
                if (input == KEY_RELEASED)
                    std::cout << " Actually released\n";
            }
        } else if (input == MOUSE_PRESSED) {
            MouseClickInputData dat = *((MouseClickInputData*) data);
            if (dat.button == MouseButton::LEFT) {
                std::cout << "\" lbutton pressed at " << dat.x << " " << dat.y << '\n';
            }
        }
    }
}
