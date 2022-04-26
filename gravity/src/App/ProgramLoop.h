#pragma once

#include "../GL/GLHelper.h"
#include "../Graphics/Renderer.h"
#include "UserInput.h"

namespace mrko900::gravity::app {
    class ProgramLoop {
    public:
        ProgramLoop(mrko900::gravity::graphics::Renderer& renderer);
        ProgramLoop(const ProgramLoop& other) = delete;
        ProgramLoop(ProgramLoop&& other) = delete;
        ProgramLoop& operator=(const ProgramLoop& other) = delete;
        ProgramLoop& operator=(ProgramLoop&& other) = delete;
        void init();
        void run();
        void updateViewport(unsigned short newWidth, unsigned short newHeight);
        void userInput(UserInput input, void* data);

    private:
        bool m_ViewportUpdateRequested;
        unsigned short m_ViewportNewWidth;
        unsigned short m_ViewportNewHeight;
        mrko900::gravity::graphics::Renderer& m_Renderer;
    };
}
