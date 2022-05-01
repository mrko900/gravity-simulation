#pragma once

#include "../GL/GLHelper.h"
#include "../Graphics/Renderer.h"
#include "UserInput.h"
#include "CoordinateSystemHelper.h"
#include <functional>
#include <vector>
#include <chrono>
#include "../Graphics/AppearanceImpl.h"

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
        std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>> m_Buttons;

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

        bool testCircleClick(unsigned short clickX, unsigned short clickY,
                             const mrko900::gravity::graphics::Circle& circle);
        bool testRectangleClick(unsigned short clickX, unsigned short clickY,
                                const mrko900::gravity::graphics::Rectangle& rectangle);
    };
}

