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
using namespace mrko900::gravity::physics; // todo remove

using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;
using enum mrko900::gravity::graphics::AppearanceAttribute;

static unsigned int objectOutline(unsigned short viewportWidth, float scale);

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
        m_PerspectiveChangeX(0.0f), m_PerspectiveChangeY(0.0f), m_PerspectiveXUpdateRequested(false),
        m_PerspectiveYUpdateRequested(false), m_PerspectiveX(0.0f), m_PerspectiveY(0.0f),
        m_GravitationalEnvironment(1e-3f, &m_CollisionDetector), m_LastPhysUpdate(high_resolution_clock::now()),
        m_PerformSimulation(true), m_SelectedObject(0), m_PrevSelectedObject(0), m_SelectedObjectValid(false),
        m_PrevSelectedObjectValid(false), m_NewObjectSelected(false), m_InputActive(false), m_Input(0.0f),
        m_InputFractional(false), m_InputDiv(1.0f), m_Menu(nullptr),
        m_CollisionDetector([this](std::pair<unsigned int, unsigned int> ids, float distance) {
            collisionTest(ids.first, ids.second, distance);
        }) { // todo m_LastPhysUpdate
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
                onClickCallback(*statePtr);
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
                m_PerformSimulation = !m_PerformSimulation;
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
        
        // object selection (important: should run before object spawning logic)
        m_LeftClickables.push_back([this](unsigned short clickX, unsigned short clickY) {
            for (const auto& i : m_Objects) {
                if (m_CanSpawnObj
                    && !testRectangleClick(clickX, clickY, *m_Menu)
                    && testCircleClick(clickX, clickY, i.second.circle)) {
                    if (m_SelectedObject != i.first || !m_SelectedObjectValid) {
                        m_SelectedObject = i.first;
                        m_SelectedObjectValid = true;
                        m_NewObjectSelected = true;
                    } else {
                        m_SelectedObjectValid = false;
                        m_NewObjectSelected = true;
                    }
                    m_CanSpawnObj = false;
                }
            }
        });

        m_MenuLayout.rect0state = false;
        m_MenuLayout.rect0appearance = initButton(3, m_MenuLayout.rect0, &m_MenuLayout.rect0state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.rect1state = false;
        m_MenuLayout.rect1appearance = initButton(4, m_MenuLayout.rect1, &m_MenuLayout.rect1state,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.massInputState = false;
        m_MenuLayout.massInputAppearance = initButton(5, m_MenuLayout.massInput, &m_MenuLayout.massInputState,
            std::function<void(bool)>([this](bool mode) {
                m_InputActive = mode;
                if (!mode) {
                    interpretInput();

                    printf("> Set mass for obj %d: %f\n", m_SelectedObject, m_Input);

                    Object& object = m_Objects.find(m_SelectedObject)->second;
                    PhysicalObject& physics = object.physics;
                    object.circle.radius *= sqrt(abs((float) m_Input / physics.massPoint.mass));
                    if (m_Input < 0.0f)
                        object.appearance.getFillColor() = RGBAColor { 1.0f, 0.0f, 0.0f, 1.0f };
                    else
                        object.appearance.getFillColor() = RGBAColor { 0.0f, 1.0f, 0.0f, 1.0f };
                    physics.massPoint.mass = (float) m_Input;
                    object.refresh = true;

                    updateSelectedObjectOutline();

                    resetInput();
                }
            })
        );
        m_MenuLayout.xvelInputState = false;
        m_MenuLayout.xvelInputAppearance = initButton(6, m_MenuLayout.xvelInput, &m_MenuLayout.xvelInputState,
            std::function<void(bool)>([this](bool mode) {
                m_InputActive = mode;
                if (!mode) {
                    interpretInput();

                    printf("> Set horizontal velocity for obj %d: %f\n", m_SelectedObject, m_Input);

                    Object& object = m_Objects.find(m_SelectedObject)->second;
                    PhysicalObject& physics = object.physics;
                    physics.velocity.setCoordinate(0, (float) m_Input);

                    resetInput();
                }
            })
        );
        m_MenuLayout.yvelInputState = false;
        m_MenuLayout.yvelInputAppearance = initButton(7, m_MenuLayout.yvelInput, &m_MenuLayout.yvelInputState,
            std::function<void(bool)>([this](bool mode) {
                m_InputActive = mode;
                if (!mode) {
                    interpretInput();

                    printf("> Set vertical velocity for obj %d: %f\n", m_SelectedObject, m_Input);

                    Object& object = m_Objects.find(m_SelectedObject)->second;
                    PhysicalObject& physics = object.physics;
                    physics.velocity.setCoordinate(1, (float) m_Input);

                    resetInput();
                }
            })
        );
        m_MenuLayout.radiusInputState = false;
        m_MenuLayout.radiusInputAppearance = initButton(8, m_MenuLayout.radiusInput, &m_MenuLayout.radiusInputState,
            std::function<void(bool)>([](bool) {}));
        m_MenuLayout.gInputState = false;
        m_MenuLayout.gInputAppearance = initButton(9, m_MenuLayout.gInput, &m_MenuLayout.gInputState,
            std::function<void(bool)>([](bool) {}));
        
        m_MenuLayout.xposInputState = false;
        m_MenuLayout.xposInputAppearance = initButton(10, m_MenuLayout.xposInput, &m_MenuLayout.xposInputState,
            std::function<void(bool)>([this](bool mode) {
                m_InputActive = mode;
                if (!mode) {
                    interpretInput();

                    printf("> Set pos x for obj %d: %f\n", m_SelectedObject, m_Input);

                    Object& object = m_Objects.find(m_SelectedObject)->second;
                    PhysicalObject& physics = object.physics;
                    physics.coordinates.setCoordinate(0, m_Input);

                    object.refresh = true;

                    resetInput();
                }
            })
        );

        m_MenuLayout.yposInputState = false;
        m_MenuLayout.yposInputAppearance = initButton(11, m_MenuLayout.yposInput, &m_MenuLayout.yposInputState,
            std::function<void(bool)>([this](bool mode) {
                m_InputActive = mode;
                if (!mode) {
                    interpretInput();

                    printf("> Set pos y for obj %d: %f\n", m_SelectedObject, m_Input);

                    Object& object = m_Objects.find(m_SelectedObject)->second;
                    PhysicalObject& physics = object.physics;
                    physics.coordinates.setCoordinate(1, m_Input);

                    object.refresh = true;

                    resetInput();
                }
            })
        );

        // object spawning
        m_LeftClickables.push_back([this] (unsigned short clickX, unsigned short clickY) {
            if (m_CanSpawnObj && !testRectangleClick(clickX, clickY, *m_Menu)) {
                static unsigned int idd = 12; // todo id system
                
                // screen coordinates of the new object
                float normalizedX = 2.0f * (float) clickX / (float) m_ViewportWidth - 1.0f;
                float normalizedY = 2.0f * (float) clickY / (float) m_ViewportHeight - 1.0f;
                float x = weightX(normalizedX);
                float y = weightY(normalizedY);

                AppearanceAttribute attributes[] { FILL_COLOR, OUTLINE }; // object appearance attribs

                // check if the map has resized itself and pointers became invalid
                bool revalidate = m_Objects.bucket_count() * m_Objects.max_load_factor() == m_Objects.size();

                // create and store a new object
                m_Objects.insert({ idd, Object {
                    AppearanceImpl(attributes, 2),
                    Circle { x, y, weightY(0.375f) * m_WorldScale * m_AspectRatio, nullptr, -2 },
                    normalizedX, normalizedY, false, m_AspectRatio,
                    PhysicalObject {
                        DynamicCoordinatesImpl(),
                        DynamicCoordinatesImpl(),
                        MassPoint { 1000.0f , nullptr },
                        VectorModelImpl(2),
                        DynamicCoordinatesImpl(),
                        DynamicPoint {},
                        GravityField { nullptr, true, nullptr }
                    }, true
                } });

                unsigned int myId = idd; // id of the new object

                if (revalidate) {
                    // update pointers to objects stored in the map if it has resized (since they now obsolete)
                    for (auto& entry : m_Objects) {
                        if (entry.first == myId)
                            continue;
                        Object& object = entry.second;
                        m_Renderer.replaceCircle(entry.first, object.circle);
                        m_ForceSimulation.replaceEntity(entry.first, object.physics.dynamicPoint);
                        object.circle.appearance = &entry.second.appearance;
                        object.physics.massPoint.coordinates = &object.physics.coordinates;
                        object.physics.dynamicPoint.massPoint = &object.physics.massPoint;
                        object.physics.dynamicPoint.forceModel = &object.physics.forceModel;
                        object.physics.dynamicPoint.velocity = &object.physics.velocity;
                        object.physics.gravityField.massPoint = &object.physics.massPoint;
                        object.physics.gravityField.netGravitationalForce = &object.physics.forces.at(0);
                        object.physics.forceModel.removeVector(0);
                        object.physics.forceModel.removeVector(1);
                        object.physics.forceModel.addVector(0, object.physics.forces[0]);
                        object.physics.forceModel.addVector(1, object.physics.forces[1]);
                    }
                }

                Object& me = m_Objects.at(myId); // newly spawned object

                // set appearance for the new object
                me.appearance.getFillColor() = RGBAColor { 0.0f, 1.0f, 0.0f, 1.0f };
                Outline& myOutline = me.appearance.getOutline();
                myOutline.mode = OutlineMode::INNER;
                myOutline.width = 0;
                myOutline.color = RGBAColor { 1.0f, 1.0f, 1.0f, 1.0f };
                me.circle.appearance = &me.appearance;

                // set up coordinates pointer for the new object
                me.physics.massPoint.coordinates = &me.physics.coordinates;

                // set up the dynamic point of the new object
                DynamicPoint& dynamicPoint = me.physics.dynamicPoint;
                dynamicPoint.massPoint = &me.physics.massPoint;
                dynamicPoint.forceModel = &me.physics.forceModel;
                dynamicPoint.velocity = &me.physics.velocity;

                // initialize gravitational force
                me.physics.forces.push_back(DynamicCoordinatesImpl()); // net gravitational force
                me.physics.forces.back().setCoordinate(0, 0.0f);
                me.physics.forces.back().setCoordinate(1, 0.0f);

                // initialize normal force
                me.physics.forces.push_back(DynamicCoordinatesImpl()); // normal force
                me.physics.forces.back().setCoordinate(0, 0.0f);
                me.physics.forces.back().setCoordinate(1, 0.0f);

                // add gravitational force to the force model associated with the new object
                me.physics.forceModel.addVector(0, me.physics.forces[0]);
                me.physics.forceModel.addVector(1, me.physics.forces[1]);

                // set up physical coordinates for the new object
                DynamicCoordinatesImpl& coordinates = me.physics.coordinates;
                coordinates.setCoordinate(0, worldX(me.normalizedX));
                coordinates.setCoordinate(1, worldY(me.normalizedY, me.aspectRatio));
                DynamicCoordinatesImpl& oldCoordinates = me.physics.oldCoordinates;
                oldCoordinates.setCoordinate(0, coordinates.getCoordinate(0));
                oldCoordinates.setCoordinate(1, coordinates.getCoordinate(1));

                // set up velocity for the new object
                DynamicCoordinatesImpl& velocity = me.physics.velocity;
                velocity.setCoordinate(0, 0.0f);
                velocity.setCoordinate(1, 0.0f);

                // set up gravity field pointers
                me.physics.gravityField.massPoint = &me.physics.massPoint;
                me.physics.gravityField.netGravitationalForce = &me.physics.forces.at(0);

                // add the new object to renderer
                m_Renderer.addCircle(idd, me.circle);

                // add the new object to simulation
                m_ForceSimulation.addEntity(idd, me.physics.dynamicPoint);
                m_GravitationalEnvironment.addEntity(idd, me.physics.gravityField);

                // select newly spawned object
                m_SelectedObject = myId;
                m_SelectedObjectValid = true;
                m_NewObjectSelected = true;

                ++idd;
            } else
                m_CanSpawnObj = true;
        });

        // todo: clickables order (able to remove object even when the menu button is pressed)

        // object removal
        m_MiddleClickables.push_back([this](unsigned short clickX, unsigned short clickY) {
            auto it = m_Objects.begin();
            while (it != m_Objects.end()) {
                bool erase;
                if (m_CanSpawnObj 
                    && !testRectangleClick(clickX, clickY, *m_Menu)
                    && testCircleClick(clickX, clickY, it->second.circle)) {
                    m_Renderer.removeFigure(it->first);
                    m_ForceSimulation.removeEntity(it->first);
                    m_GravitationalEnvironment.removeEntity(it->first);
                    if (it->first == m_PrevSelectedObject)
                        m_PrevSelectedObjectValid = false;
                    if (it->first == m_SelectedObject)
                        m_SelectedObjectValid = false;
                    // erase object
                    erase = true;
                } else {
                    erase = false;
                }
                if (erase)
                    it = m_Objects.erase(it);
                else
                    ++it;
            }
        });

        m_ViewportUpdateRequested = m_ViewportInitializationRequested = true;
    }

    void ProgramLoop::interpretInput() {
        m_Input /= m_InputDiv;
    }

    void ProgramLoop::resetInput() {
        m_Input = 0.0f;
        m_InputFractional = false;
        m_InputDiv = 1.0f;
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

            m_MenuLayout.xposInputAnimBeginX = m_MenuAnimBeginX - m_Menu->width / 2.0f + weightY(0.2f);
            m_MenuLayout.xposInput.x = m_MenuLayout.xposInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.xposInput.y = weightY(-0.7f);
            m_MenuLayout.xposInput.width = weightY(0.2f);
            m_MenuLayout.xposInput.height = weightY(0.2f);

            m_MenuLayout.yposInputAnimBeginX = m_MenuAnimBeginX + m_Menu->width / 2.0f - weightY(0.2f);
            m_MenuLayout.yposInput.x = m_MenuLayout.yposInputAnimBeginX - m_MenuAnimCompletion * m_Menu->width;
            m_MenuLayout.yposInput.y = weightY(-0.7f);
            m_MenuLayout.yposInput.width = weightY(0.2f);
            m_MenuLayout.yposInput.height = weightY(0.2f);

            if (!m_ViewportInitializationRequested) {
                m_Renderer.viewport(m_ViewportWidth, m_ViewportHeight);
            } else {
                m_ViewportInitializationRequested = false;
            }
            
            for (uint8_t id = 0; id <= 11; ++id)
                refresh.insert(id);

            for (const auto& entry : m_Objects)
                refresh.insert(entry.first);

            m_ViewportUpdateRequested = false;
        }

        if (m_MenuState == MenuState::OPENING || m_MenuState == MenuState::CLOSING) {
            auto now = std::chrono::high_resolution_clock::now();
            auto timeDiff = now - m_MenuAnimBeginTime;
            int microsTimeDiff = duration_cast<microseconds>(timeDiff).count();
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
            m_MenuLayout.xposInput.x = m_MenuLayout.xposInputAnimBeginX - dx;
            m_MenuLayout.yposInput.x = m_MenuLayout.yposInputAnimBeginX - dx;
            
            for (uint8_t id = 0; id <= 11; ++id)
                refresh.insert(id);

            if (normalizedTimeDiff >= 2.0f)
                m_MenuAnimCompletion = 0.0f;
            else
                m_MenuAnimCompletion = displacement;
        }

        if (m_PerspectiveXUpdateRequested) {
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                object.normalizedX -= m_PerspectiveChangeX;
                object.circle.x = weightX(object.normalizedX);
                refresh.insert(entry.first);
            }
            m_PerspectiveXUpdateRequested = false;
        }
        if (m_PerspectiveYUpdateRequested) {
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                object.normalizedY -= m_PerspectiveChangeY * object.aspectRatio / m_AspectRatio;
                object.circle.y = weightY(object.normalizedY) * m_AspectRatio / object.aspectRatio;
                refresh.insert(entry.first);
            }
            m_PerspectiveYUpdateRequested = false;
        }

        if (m_WorldScale != m_OldWorldScale) {
            float k = m_WorldScale / m_OldWorldScale;
            m_PerspectiveX *= k;
            m_PerspectiveY *= k;
            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                object.normalizedX *= k;
                object.normalizedY *= k;
                object.refresh = true;
                object.circle.radius *= k;
                refresh.insert(entry.first);
            }
            m_OldWorldScale = m_WorldScale;
        }

        constexpr float targetPhysUpdRate = 60.0f;

        // physics
        time_point<high_resolution_clock> currentFrame = high_resolution_clock::now();
        if (m_PerformSimulation) {
            microseconds physicsUpdDiff = duration_cast<microseconds>(currentFrame - m_LastPhysUpdate);
            if (physicsUpdDiff.count() > (int) (1.0e6f / targetPhysUpdRate)) {
                m_GravitationalEnvironment.calculate();

                for (auto& entry : m_Objects) {
                    Object& object = entry.second;
                    if (!object.canMove) {
                        object.physics.forces[0].setCoordinate(0, 0.0f);
                        object.physics.forces[0].setCoordinate(1, 0.0f);
                        object.physics.velocity.setCoordinate(0, 0.0f);
                        object.physics.velocity.setCoordinate(1, 0.0f);
                    }
                }

                m_ForceSimulation.simulate(1.0f / targetPhysUpdRate);

                for (auto& entry : m_Objects) {
                    Object& object = entry.second;
                    float newWorldX = object.physics.coordinates.getCoordinate(0);
                    float newWorldY = object.physics.coordinates.getCoordinate(1);
                    float oldWorldX = object.physics.oldCoordinates.getCoordinate(0);
                    float oldWorldY = object.physics.oldCoordinates.getCoordinate(1);
                    float worldDX = newWorldX - oldWorldX;
                    float worldDY = newWorldY - oldWorldY;
                    object.normalizedX += normalizedDX(worldDX);
                    object.normalizedY += normalizedDY(worldDY, object.aspectRatio);
                    object.physics.oldCoordinates.setCoordinate(0, newWorldX);
                    object.physics.oldCoordinates.setCoordinate(1, newWorldY);
                    object.refresh = true;
                    object.canMove = true;
                }

                m_LastPhysUpdate += microseconds((int) (1.0e6f / targetPhysUpdRate));
            }
        } else {
            m_LastPhysUpdate = currentFrame;

            for (auto& entry : m_Objects) {
                Object& object = entry.second;
                if (!object.refresh)
                    continue;
                float newWorldX = object.physics.coordinates.getCoordinate(0);
                float newWorldY = object.physics.coordinates.getCoordinate(1);
                float oldWorldX = object.physics.oldCoordinates.getCoordinate(0);
                float oldWorldY = object.physics.oldCoordinates.getCoordinate(1);
                float worldDX = newWorldX - oldWorldX;
                float worldDY = newWorldY - oldWorldY;
                object.normalizedX += normalizedDX(worldDX);
                object.normalizedY += normalizedDY(worldDY, object.aspectRatio);
                object.physics.oldCoordinates.setCoordinate(0, newWorldX);
                object.physics.oldCoordinates.setCoordinate(1, newWorldY);
            }
        }
        // end physics

        // object selection
        if (m_NewObjectSelected) {
            updateSelectedObjectOutline();
            if (m_PrevSelectedObjectValid)
                m_Objects.at(m_PrevSelectedObject).appearance.getOutline().width = 0;

            m_PrevSelectedObject = m_SelectedObject;
            m_PrevSelectedObjectValid = m_SelectedObjectValid;
            m_NewObjectSelected = false;
        }
        // end object selection

        for (auto& entry : m_Objects) {
            Object& object = entry.second;
            if (object.refresh) {
                refresh.insert(entry.first);
                object.circle.x = weightX(object.normalizedX);
                object.circle.y = weightY(object.normalizedY) * m_AspectRatio / object.aspectRatio;
                object.refresh = false;
            }
        }

        for (unsigned int id : refresh)
            m_Renderer.refreshFigure(id);

        m_Renderer.render();
    }

    static float vectorAngle(float x, float y) {
        float pi = 2 * asin(1);
        if (y == 0)
            return x > 0 ? 0 : pi;
        float baseAngle = atan(abs(x / y));
        if (x > 0 && y > 0)
            return baseAngle;
        else if (x < 0 && y > 0)
            return pi - baseAngle;
        else if (x < 0 && y < 0)
            return pi + baseAngle;
        else
            return 2 * pi - baseAngle;
    }

    void ProgramLoop::collisionTest(unsigned int obj1, unsigned int obj2, float distance) {
        Object& object1 = m_Objects.at(obj1);
        Object& object2 = m_Objects.at(obj2);
        PhysicalObject& physics1 = object1.physics;
        PhysicalObject& physics2 = object2.physics;
        DynamicCoordinates& netForce1 = physics1.forces[0];
        DynamicCoordinates& netForce2 = physics2.forces[0];

        float worldRadius1 = worldDX(object1.circle.radius);
        float worldRadius2 = worldDX(object2.circle.radius);

        if (distance < worldRadius1 + worldRadius2) {
            if (!m_Collisions.contains({ obj1, obj2 })) {
                m_Collisions.insert({ obj1, obj2 });
                handleCollision(true, obj1, obj2, distance, -3.14f);
            }
        } else if (m_Collisions.contains({ obj1, obj2 })) {
            m_Collisions.erase({ obj1, obj2 });
            handleCollision(false, obj1, obj2, distance, 3.14f);
        }
    }

    void ProgramLoop::handleCollision(bool collision, unsigned int obj1, unsigned int obj2,
                                      float distance, float gravitationalForce) {
        if (collision) {
            std::cout << "collision\n";

            Object& object1 = m_Objects.at(obj1);
            Object& object2 = m_Objects.at(obj2);
            PhysicalObject& physics1 = object1.physics;
            PhysicalObject& physics2 = object2.physics;

        } else {
            std::cout << "no collision\n";
        }
    }

    float ProgramLoop::worldX(float normalizedX) {
        return (normalizedX + m_PerspectiveX) / m_WorldScale;
    }

    float ProgramLoop::worldY(float normalizedY, float ownAspectRatio) {
        return (normalizedY + m_PerspectiveY) / ownAspectRatio / m_WorldScale;
    }

    float ProgramLoop::worldDX(float normalizedY) {
        return normalizedY / m_WorldScale;
    }

    float ProgramLoop::normalizedDX(float worldDX) {
        return worldDX * m_WorldScale;
    }

    float ProgramLoop::normalizedDY(float worldDY, float ownAspectRatio) {
        return worldDY * m_WorldScale * ownAspectRatio;
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportWidth = newWidth;
        m_ViewportHeight = newHeight;
        float oldAR = m_AspectRatio;
        m_AspectRatio = (float) newWidth / (float) newHeight;

        m_PerspectiveY *= m_AspectRatio / oldAR;

        updateSelectedObjectOutline();
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
            updateSelectedObjectOutline();
        } else if (input == MOUSE_MOVE) {
            if (m_ChangingPerspective) {
                const MouseMoveInputData& mouseMove = *((MouseMoveInputData*) data);
                short viewportDX = abs(mouseMove.toX - mouseMove.fromX);
                short viewportDY = abs(mouseMove.toY - mouseMove.fromY);
                float kx = mouseMove.toX > mouseMove.fromX ? -1.0f : 1.0f;
                float ky = mouseMove.toY > mouseMove.fromY ? -1.0f : 1.0f;
                float normalizedDX = abs((float) viewportDX / (float) m_ViewportWidth * 2.0f);
                float normalizedDY = abs((float) viewportDY / (float) m_ViewportHeight * 2.0f);
                m_PerspectiveChangeX = kx * normalizedDX;
                m_PerspectiveChangeY = ky * normalizedDY;
                m_PerspectiveX += m_PerspectiveChangeX;
                m_PerspectiveY += m_PerspectiveChangeY;
                m_PerspectiveXUpdateRequested = true;
                m_PerspectiveYUpdateRequested = true;
            }
        } else if (input == KEY_PRESSED) {
            if (m_InputActive) {
                const KeyboardInputData& keyboardInput = *((KeyboardInputData*) data);
                bool inputDivUpd = true;
                switch (keyboardInput) {
                    case KEY_0:
                        m_Input *= 10.0f;
                        break;
                    case KEY_1:
                        m_Input *= 10.0f;
                        m_Input += 1.0f;
                        break;
                    case KEY_2:
                        m_Input *= 10.0f;
                        m_Input += 2.0f;
                        break;
                    case KEY_3:
                        m_Input *= 10.0f;
                        m_Input += 3.0f;
                        break;
                    case KEY_4:
                        m_Input *= 10.0f;
                        m_Input += 4.0f;
                        break;
                    case KEY_5:
                        m_Input *= 10.0f;
                        m_Input += 5.0f;
                        break;
                    case KEY_6:
                        m_Input *= 10.0f;
                        m_Input += 6.0f;
                        break;
                    case KEY_7:
                        m_Input *= 10.0f;
                        m_Input += 7.0f;
                        break;
                    case KEY_8:
                        m_Input *= 10.0f;
                        m_Input += 8.0f;
                        break;
                    case KEY_9:
                        m_Input *= 10.0f;
                        m_Input += 9.0f;
                        break;
                    case KEY_PERIOD:
                        m_InputFractional = true;
                        inputDivUpd = false;
                        break;
                    case KEY_MINUS:
                        m_InputDiv *= -1.0f;
                        break;
                    default:
                        inputDivUpd = false;
                        break;
                }
                if (inputDivUpd && m_InputFractional) {
                    m_InputDiv *= 10.0f;
                }
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

    void ProgramLoop::updateSelectedObjectOutline() {
        if (m_SelectedObjectValid) {
            Object& o = m_Objects.at(m_SelectedObject);
            o.appearance.getOutline().width
                = objectOutline(m_ViewportWidth, 5 * o.circle.radius);
        }
    }
}

static unsigned int objectOutline(unsigned short viewportWidth, float scale) {
    return (float) viewportWidth / 30.0f * scale;
}
