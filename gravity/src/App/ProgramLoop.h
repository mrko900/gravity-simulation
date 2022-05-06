#pragma once

#include "../GL/GLHelper.h"
#include "../Graphics/Renderer.h"
#include "UserInput.h"
#include "CoordinateSystemHelper.h"
#include <functional>
#include <vector>
#include <chrono>
#include "../Graphics/AppearanceImpl.h"
#include "../Physics/UniformForceSimulation.h"
#include "../Physics/VectorModelImpl.h"
#include "../Physics/DynamicCoordinatesImpl.h"
#include "../Physics/GravitationalEnvironment.h"
#include <chrono>

namespace mrko900::gravity::app {
    class ProgramLoop {
    public:
        ProgramLoop(mrko900::gravity::graphics::Renderer& renderer, CoordinateSystemHelper& coordinateSystemHelper,
                    unsigned short viewportWidth, unsigned short viewportHeight);
        ProgramLoop(const ProgramLoop& other) = delete;
        ProgramLoop(ProgramLoop&& other) = delete;
        ~ProgramLoop();
        ProgramLoop& operator=(const ProgramLoop& other) = delete;
        ProgramLoop& operator=(ProgramLoop&& other) = delete;
        void init();
        void run();
        void updateViewport(unsigned short newWidth, unsigned short newHeight);
        void userInput(UserInput input, void* data);

    private:
        bool m_ViewportUpdateRequested;
        bool m_ViewportInitializationRequested;
        unsigned short m_ViewportWidth;
        unsigned short m_ViewportHeight;
        float m_AspectRatio;

        mrko900::gravity::graphics::Renderer& m_Renderer;
        CoordinateSystemHelper& m_CoordinateSystemHelper;

        mrko900::gravity::graphics::Circle* m_PlayButton;
        mrko900::gravity::graphics::AppearanceImpl* m_PlayButtonAppearance;
        mrko900::gravity::graphics::Circle* m_MenuButton;
        mrko900::gravity::graphics::AppearanceImpl* m_MenuButtonAppearance;
        mrko900::gravity::graphics::Rectangle* m_Menu;
        mrko900::gravity::graphics::AppearanceImpl* m_MenuAppearance;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_MenuAnimBeginTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_MenuAnimPauseBeginTime;
        float m_MenuAnimCompletion;

        mrko900::gravity::graphics::AppearanceImpl* initButton(unsigned int id, 
                                                               mrko900::gravity::graphics::Rectangle& rect,
                                                               bool* statePtr, 
                                                               std::function<void(bool)> onClickCallback);

        bool m_CanSpawnObj;

        std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>> m_LeftClickables;
        std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>> m_RightClickables;
        std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>> m_MiddleClickables;

        enum class MenuState {
            CLOSED, OPENING, OPEN, CLOSING
        };

        struct MenuLayout {
            mrko900::gravity::graphics::Rectangle rect0;
            mrko900::gravity::graphics::AppearanceImpl* rect0appearance;
            float rect0animbeginx;
            bool rect0state;
            mrko900::gravity::graphics::Rectangle rect1;
            mrko900::gravity::graphics::AppearanceImpl* rect1appearance;
            float rect1animbeginx;
            bool rect1state;
            mrko900::gravity::graphics::Rectangle massInput;
            mrko900::gravity::graphics::AppearanceImpl* massInputAppearance;
            float massInputAnimBeginX;
            bool massInputState;
            mrko900::gravity::graphics::Rectangle xvelInput;
            mrko900::gravity::graphics::AppearanceImpl* xvelInputAppearance;
            float xvelInputAnimBeginX;
            bool xvelInputState;
            mrko900::gravity::graphics::Rectangle yvelInput;
            mrko900::gravity::graphics::AppearanceImpl* yvelInputAppearance;
            float yvelInputAnimBeginX;
            bool yvelInputState;
            mrko900::gravity::graphics::Rectangle radiusInput;
            mrko900::gravity::graphics::AppearanceImpl* radiusInputAppearance;
            float radiusInputAnimBeginX;
            bool radiusInputState;
            mrko900::gravity::graphics::Rectangle gInput;
            mrko900::gravity::graphics::AppearanceImpl* gInputAppearance;
            float gInputAnimBeginX;
            bool gInputState;
        };

        MenuLayout m_MenuLayout;

        float m_MenuAnimBeginX;
        float m_MenuButtonAnimBeginX;
        float m_PlayButtonAnimBeginX;
        MenuState m_MenuState;
        std::function<float(float time)> m_MenuAnimDisplacementFunc;

        struct PhysicalObject {
            physics::DynamicCoordinatesImpl coordinates;
            physics::DynamicCoordinatesImpl oldCoordinates;
            physics::MassPoint massPoint;
            physics::VectorModelImpl forceModel;
            physics::DynamicCoordinatesImpl velocity;
            physics::DynamicPoint dynamicPoint;
            physics::GravityField gravityField;
            std::vector<physics::DynamicCoordinatesImpl> forces;
        };

        struct Object {
            mrko900::gravity::graphics::AppearanceImpl appearance;
            mrko900::gravity::graphics::Circle circle;
            float normalizedX, normalizedY;
            bool refresh;
            float aspectRatio;
            PhysicalObject physics;
        };

        std::unordered_map<unsigned int, Object> m_Objects;

        bool testCircleClick(unsigned short clickX, unsigned short clickY,
                             const mrko900::gravity::graphics::Circle& circle);
        bool testRectangleClick(unsigned short clickX, unsigned short clickY,
                                const mrko900::gravity::graphics::Rectangle& rectangle);

        float worldX(float normalizedX);
        float worldY(float normalizedY, float ownAspectRatio);
        float normalizedX(float worldX);
        float normalizedY(float worldY, float ownAspectRatio);

        float m_WorldScale;
        float m_OldWorldScale;

        bool m_PerspectiveXUpdateRequested;
        bool m_PerspectiveYUpdateRequested;
        float m_PerspectiveChangeX;
        float m_PerspectiveChangeY;
        float m_PerspectiveX;
        float m_PerspectiveY;

        bool m_ChangingPerspective;

        physics::UniformForceSimulation m_ForceSimulation;
        physics::GravitationalEnvironment m_GravitationalEnvironment;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastPhysUpdate;

        bool m_PerformSimulation;
        unsigned int m_SelectedObject;
        unsigned int m_PrevSelectedObject;
        bool m_SelectedObjectValid;
        bool m_PrevSelectedObjectValid;
        bool m_NewObjectSelected;

        bool m_MassInputActive;
        int m_MassInput;
    };
}
