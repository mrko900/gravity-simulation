#pragma once

#include "../GL/GLHelper.h"
#include "../Graphics/Renderer.h"
#include "UserInput.h"
#include "CoordinateSystemHelper.h"
#include <functional>
#include <vector>
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

        mrko900::gravity::graphics::Renderer& m_Renderer;
        CoordinateSystemHelper& m_CoordinateSystemHelper;

        mrko900::gravity::graphics::Circle* m_PlayButton;
        mrko900::gravity::graphics::PlainColor* m_PlayButtonColor;
        mrko900::gravity::graphics::Circle* m_MenuButton;
        mrko900::gravity::graphics::PlainColor* m_MenuButtonColor;
        mrko900::gravity::graphics::Rectangle* m_Menu;
        mrko900::gravity::graphics::PlainColor* m_MenuColor;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_MenuAnimBeginTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_MenuAnimPauseBeginTime;

        std::vector<std::function<void(unsigned short clickX, unsigned short clickY)>> m_Buttons;

        enum class MenuState {
            CLOSED, OPENING, OPEN, CLOSING
        };

        float m_MenuAnimBeginX;
        MenuState m_MenuState;
        std::function<float(float time)> m_AnimMenuDisplacementFunc;

        bool testCircleClick(unsigned short clickX, unsigned short clickY,
                             const mrko900::gravity::graphics::Circle* circle);
    };
}
