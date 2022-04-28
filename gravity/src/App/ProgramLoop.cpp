#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"
#include "../Graphics/Texture.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

#define weightX(x) (m_CoordinateSystemHelper.weighted(0, x))
#define weightY(y) (m_CoordinateSystemHelper.weighted(1, y))
#define distance(x1, y1, x2, y2) (sqrt(pow(x1 - x2, 2) + pow(y1 - y1, 2)))

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using namespace mrko900::gravity::graphics; // todo remove

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer, 
                             CoordinateSystemHelper& coordinateSystemHelper,
                             unsigned short viewportWidth, unsigned short viewportHeight) : m_Renderer(renderer),
        m_CoordinateSystemHelper(coordinateSystemHelper), m_ViewportUpdateRequested(false),
        m_ViewportInitializationRequested(false), m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight), 
        m_PlayButton(nullptr) {
    }

    ProgramLoop::~ProgramLoop() {
        if (m_PlayButton != nullptr) {
            delete m_PlayButton;
            delete m_PlayButtonColor;
        }
    }

    void ProgramLoop::init() {
        m_PlayButtonColor = new PlainColor { 0.3f, 0.5f, 0.3f, 1.0f };
        m_PlayButton = new Circle { 
            0.0f, 0.0f, 0.0f, Appearance { AppearanceType::PLAIN_COLOR, m_PlayButtonColor }, 0
        };
        m_Renderer.addCircle(0, *m_PlayButton);

        m_Buttons.push_back([this] (unsigned short clickX, unsigned short clickY) {
            float normalizedX = (float) clickX / (float) m_ViewportWidth * 2 - 1;
            float normalizedY = (float) clickY / (float) m_ViewportHeight * 2 - 1;
            //float weightedX = weightY(normalizedX);
            //float weightedY = weightY(normalizedY);
            //float dist = distance(weightedX, weightedY, m_PlayButton->x, m_PlayButton->y);
            float rx = 2 * m_PlayButton->radius / weightX(2.0f);
            float ry = 2 * m_PlayButton->radius / weightY(2.0f);
            if (((normalizedX - 0.4f) * (normalizedX - 0.4f)) / (rx * rx) 
                + ((normalizedY + 0.1f) * (normalizedY + 0.1f)) / (ry * ry) <= 1) {
                // click
                m_PlayButton->appearance.plainColor().g += 0.1f;
                m_PlayButton->appearance.plainColor().r -= 0.03f;
                m_PlayButton->appearance.plainColor().b += 0.15f;
                if (m_PlayButton->appearance.plainColor().g >= 1.0f)
                    m_PlayButton->appearance.plainColor().g -= 1.0f;
                if (m_PlayButton->appearance.plainColor().r <= 0.0f)
                    m_PlayButton->appearance.plainColor().r += 1.0f;
                if (m_PlayButton->appearance.plainColor().b >= 1.0f)
                    m_PlayButton->appearance.plainColor().b -= 1.0f;
            }
        });

        m_ViewportUpdateRequested = true;
    }

    void ProgramLoop::run() {
        if (m_ViewportUpdateRequested) {
            if (!m_ViewportInitializationRequested)
                m_Renderer.viewport(m_ViewportWidth, m_ViewportHeight);
            else
                m_ViewportInitializationRequested = false;
            m_ViewportUpdateRequested = false;

            if (m_PlayButton != nullptr) {
                //m_PlayButton->x = weightX(1.0f - weightY(0.1f));
                //m_PlayButton->y = weightY(-0.9f);
                //m_PlayButton->radius = weightY(0.08f);
                m_PlayButton->x = weightX(0.4f);
                m_PlayButton->y = weightY(-0.1f);
                m_PlayButton->radius = weightX(0.5f);
            }

            m_Renderer.refreshFigure(0);
        }

        m_Renderer.render();
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportWidth = newWidth;
        m_ViewportHeight = newHeight;
    }

    void ProgramLoop::userInput(UserInput input, void* data) {
        if (input == MOUSE_PRESSED) {
            MouseClickInputData mouseClick = *((MouseClickInputData*) data);
            for (auto& button : m_Buttons)
                button(mouseClick.x, mouseClick.y);
        }
    }
}
