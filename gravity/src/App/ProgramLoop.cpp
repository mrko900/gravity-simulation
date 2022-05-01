#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"
#include "../Graphics/Texture.h"
#include <chrono>
#include <stdexcept>

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

#define weightX(x) (m_CoordinateSystemHelper.weighted(0, x))
#define weightY(y) (m_CoordinateSystemHelper.weighted(1, y))

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using namespace mrko900::gravity::graphics; // todo remove

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer, 
                             CoordinateSystemHelper& coordinateSystemHelper,
                             unsigned short viewportWidth, unsigned short viewportHeight) : m_Renderer(renderer),
        m_CoordinateSystemHelper(coordinateSystemHelper), m_ViewportUpdateRequested(false),
        m_ViewportInitializationRequested(false), m_ViewportWidth(viewportWidth), m_ViewportHeight(viewportHeight), 
        m_PlayButton(nullptr), m_MenuState(MenuState::CLOSED), m_MenuAnimBeginX(0.0f),
        m_MenuAnimDisplacementFunc([](float t) -> float {
            if (t < 0.0f || t > 2.0f)
                throw std::invalid_argument("function domain is [0, 2]");
            return t <= 1.0f 
                   ? (t * t * (3.0f - 2.0f * t)) 
                   : ((t - 1.0f) * (t - 1.0f) * (2.0f * t - 5.0f) + 1);
        }), m_MenuAnimBeginTime(time_point<std::chrono::high_resolution_clock>()),
        m_MenuAnimPauseBeginTime(time_point<std::chrono::high_resolution_clock>()), m_MenuAnimCompletion(0.0f),
        m_CanSpawnObj(true) {
    }

    ProgramLoop::~ProgramLoop() {
        if (m_PlayButton != nullptr) {
            delete m_PlayButton;
            delete m_PlayButtonColor;
        }

        if (m_MenuButton != nullptr) {
            delete m_MenuButton;
            delete m_MenuButtonColor;
        }

        if (m_Menu != nullptr) {
            delete m_Menu;
            delete m_MenuColor;
        }
    }

    void ProgramLoop::initButton(unsigned int id, Rectangle& rect, PlainColor& color, bool* statePtr,
                                 std::function<void(bool)> onClickCallback) {
        color.r = 1.0f;
        color.g = 0.0f;
        color.b = 0.0f;
        color.a = 1.0f;
        rect.appearance.type = AppearanceType::PLAIN_COLOR;
        rect.appearance.ptr = &color;
        m_Buttons.push_back([this, &rect, onClickCallback, statePtr]
                            (unsigned short clickX, unsigned short clickY) {
            if (testRectangleClick(clickX, clickY, rect)) {
                rect.appearance.plainColor() = *statePtr
                    ? PlainColor { 1.0f, 0.0f, 0.0f, 1.0f }
                    : PlainColor { 0.0f, 1.0f, 0.0f, 1.0f };
                *statePtr = !*statePtr;
                onClickCallback(true);
                m_CanSpawnObj = false;
            }
        });
        rect.width = rect.height = rect.x = rect.y = 0.0f;
        rect.layer = 0;
        m_Renderer.addRectangle(id, rect);
    }

    void ProgramLoop::init() {
        m_PlayButtonColor = new PlainColor { 0.3f, 0.5f, 0.3f, 1.0f };
        m_PlayButton = new Circle { 
            0.0f, 0.0f, 0.0f, Appearance { AppearanceType::PLAIN_COLOR, m_PlayButtonColor }, 0
        };
        m_Renderer.addCircle(0, *m_PlayButton);

        m_MenuButtonColor = new PlainColor { 0.8f, 0.1f, 0.35f, 1.0f };
        m_MenuButton = new Circle {
            0.0f, 0.0f, 0.0f, Appearance { AppearanceType::PLAIN_COLOR, m_MenuButtonColor }, 0
        };
        m_Renderer.addCircle(1, *m_MenuButton);

        m_Menu = new Rectangle {
            0.0f, 0.0f, 0.0f, 0.0f, 
            Appearance { AppearanceType::PLAIN_COLOR, m_MenuColor = new PlainColor { 0.8f, 0.3f, 0.8f, 1.0f } }, -1
        };
        m_Renderer.addRectangle(2, *m_Menu);

        m_Buttons.push_back([this](unsigned short clickX, unsigned short clickY) {
            if (testCircleClick(clickX, clickY, *m_PlayButton)) {
                m_PlayButton->appearance.plainColor().g += 0.1f;
                m_PlayButton->appearance.plainColor().r -= 0.03f;
                m_PlayButton->appearance.plainColor().b += 0.15f;
                if (m_PlayButton->appearance.plainColor().g >= 1.0f)
                    m_PlayButton->appearance.plainColor().g -= 1.0f;
                if (m_PlayButton->appearance.plainColor().r <= 0.0f)
                    m_PlayButton->appearance.plainColor().r += 1.0f;
                if (m_PlayButton->appearance.plainColor().b >= 1.0f)
                    m_PlayButton->appearance.plainColor().b -= 1.0f;
                m_CanSpawnObj = false;
            }
        });

        m_Buttons.push_back([this] (unsigned short clickX, unsigned short clickY) {
            if (testCircleClick(clickX, clickY, *m_MenuButton)) {
                if (m_MenuState == MenuState::CLOSED) {
                    m_MenuState = MenuState::OPENING;
                    m_MenuAnimBeginTime = high_resolution_clock::now();
                }
                else if (m_MenuState == MenuState::OPEN) {
                    m_MenuAnimBeginTime += (high_resolution_clock::now() - m_MenuAnimPauseBeginTime);
                    m_MenuState = MenuState::CLOSING;
                }
                m_CanSpawnObj = false;
            }
        });

        // object spawning
        m_Buttons.push_back([this] (unsigned short clickX, unsigned short clickY) {
            if (m_CanSpawnObj && !testRectangleClick(clickX, clickY, *m_Menu)) {
                // todo spawn an obj
            } else
                m_CanSpawnObj = true;
        });

        m_MenuLayout.rect0state = false;
        initButton(3, m_MenuLayout.rect0, m_MenuLayout.rect0color, &m_MenuLayout.rect0state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.rect1state = false;
        initButton(4, m_MenuLayout.rect1, m_MenuLayout.rect1color, &m_MenuLayout.rect1state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.massInputState = false;
        initButton(5, m_MenuLayout.massInput, m_MenuLayout.massInputColor, &m_MenuLayout.massInputState,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.xvelInputState = false;
        initButton(6, m_MenuLayout.xvelInput, m_MenuLayout.xvelInputColor, &m_MenuLayout.xvelInputState,
            std::function<void(bool)> ([](bool) {}));
        m_MenuLayout.yvelInputState = false;
        initButton(7, m_MenuLayout.yvelInput, m_MenuLayout.yvelInputColor, &m_MenuLayout.yvelInputState,
            std::function<void(bool)> ([](bool) {}));
        m_MenuLayout.gInputState = false;
        initButton(8, m_MenuLayout.gInput, m_MenuLayout.gInputColor, &m_MenuLayout.gInputState,
            std::function<void(bool)> ([](bool) {}));

        m_ViewportUpdateRequested = m_ViewportInitializationRequested = true;
    }

    void ProgramLoop::run() {
        if (m_ViewportUpdateRequested) {
            m_Menu->width = weightX(0.5f);
            m_Menu->height = weightY(2.0f);
            m_MenuAnimBeginX = weightX(1.0f) + m_Menu->width / 2;
            m_Menu->x = m_MenuAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_Menu->y = weightY(0.0f);

            m_PlayButtonAnimBeginX = weightX(1.0f - weightY(0.1f));
            m_PlayButton->x = m_PlayButtonAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_PlayButton->y = weightY(-0.9f);
            m_PlayButton->radius = weightY(0.08f);

            m_MenuButtonAnimBeginX = weightX(1.0f - weightY(0.1f));
            m_MenuButton->x = m_MenuButtonAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuButton->y = weightY(0.9f);
            m_MenuButton->radius = weightY(0.08f);

            m_MenuLayout.rect0animbeginx = m_MenuAnimBeginX - m_Menu->width / 2.0f + weightY(0.2f);
            m_MenuLayout.rect0.x = m_MenuLayout.rect0animbeginx - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.rect0.y = weightY(0.8f);
            m_MenuLayout.rect0.width = weightY(0.2f);
            m_MenuLayout.rect0.height = weightY(0.2f);

            m_MenuLayout.rect1animbeginx = m_MenuAnimBeginX + m_Menu->width / 2.0f - weightY(0.2f);
            m_MenuLayout.rect1.x = m_MenuLayout.rect1animbeginx - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.rect1.y = weightY(0.8f);
            m_MenuLayout.rect1.width = weightY(0.2f);
            m_MenuLayout.rect1.height = weightY(0.2f);

            m_MenuLayout.massInputAnimBeginX = m_MenuAnimBeginX;
            m_MenuLayout.massInput.x = m_MenuLayout.massInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.massInput.y = weightY(0.5f);
            m_MenuLayout.massInput.width = m_Menu->width - weightY(0.2f);
            m_MenuLayout.massInput.height = weightY(0.2f);

            m_MenuLayout.xvelInputAnimBeginX = m_MenuAnimBeginX - m_Menu->width / 2.0f + weightY(0.2f);
            m_MenuLayout.xvelInput.x = m_MenuLayout.xvelInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.xvelInput.y = weightY(0.2f);
            m_MenuLayout.xvelInput.width = weightY(0.2f);
            m_MenuLayout.xvelInput.height = weightY(0.2f);

            m_MenuLayout.yvelInputAnimBeginX = m_MenuAnimBeginX + m_Menu->width / 2.0f - weightY(0.2f);
            m_MenuLayout.yvelInput.x = m_MenuLayout.yvelInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.yvelInput.y = weightY(0.2f);
            m_MenuLayout.yvelInput.width = weightY(0.2f);
            m_MenuLayout.yvelInput.height = weightY(0.2f);

            m_MenuLayout.gInputAnimBeginX = m_MenuAnimBeginX;
            m_MenuLayout.gInput.x = m_MenuLayout.gInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.gInput.y = weightY(-0.1f);
            m_MenuLayout.gInput.width = m_Menu->width - weightY(0.2f);
            m_MenuLayout.gInput.height = weightY(0.2f);

            if (!m_ViewportInitializationRequested) {
                m_Renderer.viewport(m_ViewportWidth, m_ViewportHeight);
            } else {
                m_ViewportInitializationRequested = false;
            }

            m_Renderer.refreshFigure(0);
            m_Renderer.refreshFigure(1);
            m_Renderer.refreshFigure(2);
            m_Renderer.refreshFigure(3);
            m_Renderer.refreshFigure(4);
            m_Renderer.refreshFigure(5);
            m_Renderer.refreshFigure(6);
            m_Renderer.refreshFigure(7);
            m_Renderer.refreshFigure(8);

            m_ViewportUpdateRequested = false;
        }

        if (m_MenuState == MenuState::OPENING || m_MenuState == MenuState::CLOSING) {
            auto now = std::chrono::high_resolution_clock::now();
            auto timeDiff = now - m_MenuAnimBeginTime;
            long microsTimeDiff = duration_cast<microseconds>(timeDiff).count();
            float normalizedTimeDiff = (float) microsTimeDiff / 1'000'000.0f;

            if (m_MenuState == MenuState::OPENING && normalizedTimeDiff >= 1.0f) {
                m_MenuState = MenuState::OPEN;
                m_MenuAnimPauseBeginTime = now;
                normalizedTimeDiff = 1.0f;
            } else if (normalizedTimeDiff >= 2.0f) {
                m_MenuState = MenuState::CLOSED;
                normalizedTimeDiff = 2.0f;
            }

            float displacement = m_MenuAnimDisplacementFunc(normalizedTimeDiff);
            float dx = m_Menu->width * displacement;
            m_Menu->x = m_MenuAnimBeginX - dx;
            m_MenuButton->x = m_MenuButtonAnimBeginX - dx;
            m_PlayButton->x = m_PlayButtonAnimBeginX - dx;
            m_MenuLayout.rect0.x = m_MenuLayout.rect0animbeginx - dx;
            m_MenuLayout.rect1.x = m_MenuLayout.rect1animbeginx - dx;
            m_MenuLayout.massInput.x = m_MenuLayout.massInputAnimBeginX - dx;
            m_MenuLayout.xvelInput.x = m_MenuLayout.xvelInputAnimBeginX - dx;
            m_MenuLayout.yvelInput.x = m_MenuLayout.yvelInputAnimBeginX - dx;
            m_MenuLayout.gInput.x = m_MenuLayout.gInputAnimBeginX - dx;
            
            m_Renderer.refreshFigure(0);
            m_Renderer.refreshFigure(1);
            m_Renderer.refreshFigure(2);
            m_Renderer.refreshFigure(3);
            m_Renderer.refreshFigure(4);
            m_Renderer.refreshFigure(5);
            m_Renderer.refreshFigure(6);
            m_Renderer.refreshFigure(7);
            m_Renderer.refreshFigure(8);

            if (normalizedTimeDiff >= 2.0f)
                m_MenuAnimCompletion = 0.0f;
            else
                m_MenuAnimCompletion = displacement;
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

    bool ProgramLoop::testCircleClick(unsigned short clickX, unsigned short clickY, const Circle& circle) {
        float normalizedX = (float) clickX / (float) m_ViewportWidth * 2 - 1;
        float normalizedY = (float) clickY / (float) m_ViewportHeight * 2 - 1;
        float weightedX = weightX(normalizedX);
        float weightedY = weightY(normalizedY);
        float x = weightedX - circle.x;
        float y = weightedY - circle.y;
        float r = circle.radius;
        return x * x + y * y <= r * r;
    }

    bool ProgramLoop::testRectangleClick(unsigned short clickX, unsigned short clickY, const Rectangle& rectangle) {
        float normalizedX = (float) clickX / (float) m_ViewportWidth * 2 - 1;
        float normalizedY = (float) clickY / (float) m_ViewportHeight * 2 - 1;
        float weightedX = weightX(normalizedX);
        float weightedY = weightY(normalizedY);
        float halfWidth = rectangle.width / 2;
        float halfHeight = rectangle.height / 2;
        return weightedX > rectangle.x - halfWidth
            && weightedX < rectangle.x + halfWidth
            && weightedY > rectangle.y - halfHeight
            && weightedY < rectangle.y + halfHeight;
    }
}
