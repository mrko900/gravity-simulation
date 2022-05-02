#include "ProgramLoop.h"

#include <iostream>
#include "../GL/GLMacros.h"
#include "../Graphics/Texture.h"
#include <chrono>
#include <stdexcept>
#include <unordered_set>

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
using enum mrko900::gravity::graphics::AppearanceAttribute;

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
                   : ((t - 1.0f) * (t - 1.0f) * (2.0f * t - 5.0f) + 1.0f);
        }), m_MenuAnimBeginTime(time_point<std::chrono::high_resolution_clock>()),
        m_MenuAnimPauseBeginTime(time_point<std::chrono::high_resolution_clock>()), m_MenuAnimCompletion(0.0f),
        m_CanSpawnObj(true), m_WorldScale(1.0f), m_OldWorldScale(m_WorldScale), 
        m_AspectRatio((float) m_ViewportWidth / (float) m_ViewportHeight), m_ChangingPerspective(false),
        m_PerspectiveChangeX(0.0f), m_PerspectiveChangeY(0.0f) {
    }

    ProgramLoop::~ProgramLoop() {
        if (m_PlayButton != nullptr) {
            delete m_PlayButton;
            delete m_PlayButtonAppearance;
        }

        if (m_MenuButton != nullptr) {
            delete m_MenuButton;
            delete m_MenuButtonAppearance;
        }

        if (m_Menu != nullptr) {
            delete m_Menu;
            delete m_MenuAppearance;
        }

        if (m_MenuLayout.rect0appearance != nullptr)
            delete m_MenuLayout.rect0appearance;

        if (m_MenuLayout.rect1appearance != nullptr)
            delete m_MenuLayout.rect1appearance;

        if (m_MenuLayout.massInputAppearance != nullptr)
            delete m_MenuLayout.massInputAppearance;

        if (m_MenuLayout.xvelInputAppearance != nullptr)
            delete m_MenuLayout.xvelInputAppearance;

        if (m_MenuLayout.yvelInputAppearance != nullptr)
            delete m_MenuLayout.yvelInputAppearance;

        if (m_MenuLayout.radiusInputAppearance != nullptr)
            delete m_MenuLayout.radiusInputAppearance;

        if (m_MenuLayout.gInputAppearance != nullptr)
            delete m_MenuLayout.gInputAppearance;
    }

    AppearanceImpl* ProgramLoop::initButton(unsigned int id, Rectangle& rect, bool* statePtr,
                                            std::function<void(bool)> onClickCallback) {
        AppearanceAttribute attribs[1] { FILL_COLOR };
        AppearanceImpl* appearance = new AppearanceImpl(attribs, 1);
        appearance->getFillColor().r = 1.0f;
        appearance->getFillColor().g = 0.0f;
        appearance->getFillColor().b = 0.0f;
        appearance->getFillColor().a = 1.0f;
        m_LeftClickables.push_back([this, &rect, onClickCallback, statePtr]
                            (unsigned short clickX, unsigned short clickY) {
            if (testRectangleClick(clickX, clickY, rect)) {
                rect.appearance->getFillColor() = *statePtr
                    ? RGBAColor { 1.0f, 0.0f, 0.0f, 1.0f }
                    : RGBAColor { 0.0f, 1.0f, 0.0f, 1.0f };
                *statePtr = !*statePtr;
                onClickCallback(true);
                m_CanSpawnObj = false;
            }
        });
        rect.width = rect.height = rect.x = rect.y = 0.0f;
        rect.layer = 0;
        rect.appearance = appearance;
        m_Renderer.addRectangle(id, rect);
        return appearance;
    }

    void ProgramLoop::init() {
        AppearanceAttribute attribs[1] = { FILL_COLOR };
        m_PlayButtonAppearance = new AppearanceImpl(attribs, 1);
        m_PlayButtonAppearance->getFillColor() = RGBAColor { 0.7f, 0.5f, 0.4f, 1.0f };
        m_PlayButton = new Circle { 
            0.0f, 0.0f, 0.0f, m_PlayButtonAppearance, 0
        };
        m_Renderer.addCircle(0, *m_PlayButton);
        
        m_MenuButtonAppearance = new AppearanceImpl(attribs, 1);
        m_MenuButtonAppearance->getFillColor() = RGBAColor { 0.3f, 0.8f, 0.95f, 1.0f };
        m_MenuButton = new Circle {
            0.0f, 0.0f, 0.0f, m_MenuButtonAppearance, 0
        };
        m_Renderer.addCircle(1, *m_MenuButton);
        
        m_MenuAppearance = new AppearanceImpl(attribs, 1);
        m_MenuAppearance->getFillColor() = RGBAColor { 0.3f, 0.3f, 0.4f, 1.0f };
        m_Menu = new Rectangle {
            0.0f, 0.0f, 0.0f, 0.0f, m_MenuAppearance, -1
        };
        m_Renderer.addRectangle(2, *m_Menu);

        m_LeftClickables.push_back([this](unsigned short clickX, unsigned short clickY) {
            if (testCircleClick(clickX, clickY, *m_PlayButton)) {
                m_PlayButton->appearance->getFillColor().g += 0.1f;
                m_PlayButton->appearance->getFillColor().r -= 0.03f;
                m_PlayButton->appearance->getFillColor().b += 0.15f;
                if (m_PlayButton->appearance->getFillColor().g >= 1.0f)
                    m_PlayButton->appearance->getFillColor().g -= 1.0f;
                if (m_PlayButton->appearance->getFillColor().r <= 0.0f)
                    m_PlayButton->appearance->getFillColor().r += 1.0f;
                if (m_PlayButton->appearance->getFillColor().b >= 1.0f)
                    m_PlayButton->appearance->getFillColor().b -= 1.0f;
                m_CanSpawnObj = false;
            }
        });

        m_LeftClickables.push_back([this] (unsigned short clickX, unsigned short clickY) {
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
        m_LeftClickables.push_back([this] (unsigned short clickX, unsigned short clickY) {
            if (m_CanSpawnObj && !testRectangleClick(clickX, clickY, *m_Menu)) {
                static unsigned int idd = 10; // todo
                float normalizedX = 2.0f * (float) clickX / (float) m_ViewportWidth - 1.0f;
                float normalizedY = 2.0f * (float) clickY / (float) m_ViewportHeight - 1.0f;
                float x = weightX(normalizedX);
                float y = weightY(normalizedY);
                AppearanceAttribute attributes[] { FILL_COLOR };
                bool revalidate = m_Objects.bucket_count() * m_Objects.max_load_factor() == m_Objects.size();
                m_Objects.insert({ idd, Object {
                    AppearanceImpl(attributes, 1), 
                    Circle { x, y, weightY(0.375f) * m_WorldScale * m_AspectRatio, nullptr, -2 }, 
                    normalizedX, normalizedY, false, m_AspectRatio
                } });
                unsigned int myId = idd;
                if (revalidate) {
                    int i = 0;
                    for (auto& entry : m_Objects) {
                        if (entry.first == myId)
                            continue;
                        m_Renderer.replaceCircle(entry.first, entry.second.circle);
                        entry.second.circle.appearance = &entry.second.appearance;
                        ++i;
                    }
                }
                Object& me = m_Objects.at(myId);
                me.appearance.getFillColor() = RGBAColor { 0.0f, 1.0f, 0.0f, 1.0f };
                me.circle.appearance = &me.appearance;
                m_Renderer.addCircle(idd, me.circle);
                ++idd;
            } else
                m_CanSpawnObj = true;
        });

        m_MenuLayout.rect0state = false;
        m_MenuLayout.rect0appearance = initButton(3, m_MenuLayout.rect0, &m_MenuLayout.rect0state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.rect1state = false;
        m_MenuLayout.rect1appearance = initButton(4, m_MenuLayout.rect1, &m_MenuLayout.rect1state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.massInputState = false;
        m_MenuLayout.massInputAppearance = initButton(5, m_MenuLayout.massInput, &m_MenuLayout.massInputState,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.xvelInputState = false;
        m_MenuLayout.xvelInputAppearance = initButton(6, m_MenuLayout.xvelInput, &m_MenuLayout.xvelInputState,
            std::function<void(bool)> ([](bool) {}));
        m_MenuLayout.yvelInputState = false;
        m_MenuLayout.yvelInputAppearance = initButton(7, m_MenuLayout.yvelInput, &m_MenuLayout.yvelInputState,
            std::function<void(bool)> ([](bool) {}));
        m_MenuLayout.radiusInputState = false;
        m_MenuLayout.radiusInputAppearance = initButton(8, m_MenuLayout.radiusInput, &m_MenuLayout.radiusInputState,
            std::function<void(bool)> ([](bool) {}));
        m_MenuLayout.gInputState = false;
        m_MenuLayout.gInputAppearance = initButton(9, m_MenuLayout.gInput, &m_MenuLayout.gInputState,
            std::function<void(bool)> ([](bool) {}));

        m_MiddleClickables.push_back([this](unsigned short clickX, unsigned short clickY) {
            auto it = m_Objects.begin();
            while (it != m_Objects.end()) {
                bool erase;
                if (m_CanSpawnObj 
                    && !testRectangleClick(clickX, clickY, *m_Menu)
                    && testCircleClick(clickX, clickY, it->second.circle)) {
                    m_Renderer.removeFigure(it->first);
                    erase = true;
                } else {
                    erase = false;
                }
                if (erase) {
                    it = m_Objects.erase(it);
                }
                else
                    ++it;
            }
        });

        m_ViewportUpdateRequested = m_ViewportInitializationRequested = true;
    }

    void ProgramLoop::run() {
        std::unordered_set<unsigned int> refresh;

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

            m_MenuLayout.radiusInputAnimBeginX = m_MenuAnimBeginX;
            m_MenuLayout.radiusInput.x = m_MenuLayout.radiusInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.radiusInput.y = weightY(-0.1f);
            m_MenuLayout.radiusInput.width = m_Menu->width - weightY(0.2f);
            m_MenuLayout.radiusInput.height = weightY(0.2f);

            m_MenuLayout.gInputAnimBeginX = m_MenuAnimBeginX;
            m_MenuLayout.gInput.x = m_MenuLayout.gInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.gInput.y = weightY(-0.4f);
            m_MenuLayout.gInput.width = m_Menu->width - weightY(0.2f);
            m_MenuLayout.gInput.height = weightY(0.2f);

            if (!m_ViewportInitializationRequested) {
                m_Renderer.viewport(m_ViewportWidth, m_ViewportHeight);
            } else {
                m_ViewportInitializationRequested = false;
            }

            for (int id = 0; id <= 9; ++id)
                refresh.insert(id);

            for (const auto& entry : m_Objects)
                refresh.insert(entry.first);

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
            m_MenuLayout.radiusInput.x = m_MenuLayout.radiusInputAnimBeginX - dx;
            m_MenuLayout.gInput.x = m_MenuLayout.gInputAnimBeginX - dx;
            
            for (int id = 0; id <= 9; ++id)
                refresh.insert(id);

            if (normalizedTimeDiff >= 2.0f)
                m_MenuAnimCompletion = 0.0f;
            else
                m_MenuAnimCompletion = displacement;
        }

        for (auto& entry : m_Objects) {
            if (entry.second.refresh) {
                refresh.insert(entry.first);
                entry.second.refresh = false;
            }
        }

        if (m_PerspectiveChangeX != 0.0f) {
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                object.normalizedX += m_PerspectiveChangeX;
                object.circle.x = weightX(object.normalizedX);
                refresh.insert(entry.first);
            }
            m_PerspectiveChangeX = 0.0f;
        }
        if (m_PerspectiveChangeY != 0.0f) {
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                object.normalizedY += m_PerspectiveChangeY;
                object.circle.y = weightY(object.normalizedY) * m_AspectRatio / object.aspectRatio;
                refresh.insert(entry.first);
            }
            m_PerspectiveChangeY = 0.0f;
        }

        if (m_WorldScale != m_OldWorldScale) {
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                float k = m_WorldScale / m_OldWorldScale;
                object.normalizedX *= k;
                object.normalizedY *= k;
                object.circle.x = weightX(object.normalizedX);
                object.circle.y = weightY(object.normalizedY) * m_AspectRatio / object.aspectRatio;
                object.circle.radius *= k;
                refresh.insert(entry.first);
            }
            m_OldWorldScale = m_WorldScale;
        }

        for (unsigned int id : refresh)
            m_Renderer.refreshFigure(id);

        m_Renderer.render();
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportWidth = newWidth;
        m_ViewportHeight = newHeight;
        m_AspectRatio = (float) newWidth / (float) newHeight;
    }

    void ProgramLoop::userInput(UserInput input, void* data) {
        if (input == MOUSE_PRESSED) {
            const MouseClickInputData& mouseClick = *((MouseClickInputData*) data);
            const std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>>* clickables;
            switch (mouseClick.button) {
                case MouseButton::LEFT:
                    clickables = &m_LeftClickables;
                    break;
                case MouseButton::RIGHT:
                    clickables = &m_RightClickables;
                    m_ChangingPerspective = true;
                    break;
                case MouseButton::MIDDLE:
                    clickables = &m_MiddleClickables;
                    break;
                default:
                    throw std::invalid_argument("unsupported mouse button type");
            }
            for (auto& button : *clickables)
                button(mouseClick.x, mouseClick.y);
        } else if (input == MOUSE_RELEASED) {
            const MouseClickInputData& mouseClick = *((MouseClickInputData*) data);
            if (mouseClick.button == MouseButton::RIGHT)
                m_ChangingPerspective = false;
        } else if (input == MOUSE_WHEEL) {
            float mouseWheelEvent = *((float*) data);
            m_WorldScale *= 1.0f + 0.1f * mouseWheelEvent;
        } else if (input == MOUSE_MOVE) {
            if (m_ChangingPerspective) {
                const MouseMoveInputData& mouseMove = *((MouseMoveInputData*) data);
                short viewportDX = abs(mouseMove.toX - mouseMove.fromX);
                short viewportDY = abs(mouseMove.toY - mouseMove.fromY);
                float kx = mouseMove.toX > mouseMove.fromX ? 1.0f : -1.0f;
                float ky = mouseMove.toY > mouseMove.fromY ? 1.0f : -1.0f;
                float normalizedDX = abs((float) viewportDX / (float) m_ViewportWidth * 2.0f);
                float normalizedDY = abs((float) viewportDY / (float) m_ViewportHeight * 2.0f);
                m_PerspectiveChangeX = kx * normalizedDX;
                m_PerspectiveChangeY = ky * normalizedDY;
            }
        }
    }

    bool ProgramLoop::testCircleClick(unsigned short clickX, unsigned short clickY, const Circle& circle) {
        float normalizedX = (float) clickX / (float) m_ViewportWidth * 2.0f - 1.0f;
        float normalizedY = (float) clickY / (float) m_ViewportHeight * 2.0f - 1.0f;
        float weightedX = weightX(normalizedX);
        float weightedY = weightY(normalizedY);
        float x = weightedX - circle.x;
        float y = weightedY - circle.y;
        float r = circle.radius;
        return x * x + y * y <= r * r;
    }

    bool ProgramLoop::testRectangleClick(unsigned short clickX, unsigned short clickY, const Rectangle& rectangle) {
        float normalizedX = (float) clickX / (float) m_ViewportWidth * 2.0f - 1.0f;
        float normalizedY = (float) clickY / (float) m_ViewportHeight * 2.0f - 1.0f;
        float weightedX = weightX(normalizedX);
        float weightedY = weightY(normalizedY);
        float halfWidth = rectangle.width / 2.0f;
        float halfHeight = rectangle.height / 2.0f;
        return weightedX > rectangle.x - halfWidth
            && weightedX < rectangle.x + halfWidth
            && weightedY > rectangle.y - halfHeight
            && weightedY < rectangle.y + halfHeight;
    }
}
