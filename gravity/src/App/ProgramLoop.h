#pragma once

#include "../GL/GLHelper.h"

namespace mrko900::gravity::app {
    class ProgramLoop {
    public:
        ProgramLoop(mrko900::gravity::gl::GLHelper& glHelper);
        ProgramLoop(const ProgramLoop& other) = delete;
        ProgramLoop(ProgramLoop&& other) = delete;
        ProgramLoop& operator=(const ProgramLoop& other) = delete;
        ProgramLoop& operator=(ProgramLoop&& other) = delete;
        void operator()();
        void updateViewport(unsigned short newWidth, unsigned short newHeight);

    private:
        bool m_ViewportUpdateRequested;
        unsigned short m_ViewportNewWidth;
        unsigned short m_ViewportNewHeight;
        mrko900::gravity::gl::GLHelper& m_GLHelper;
    };
}
