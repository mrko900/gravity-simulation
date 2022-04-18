#include "ProgramLoop.h"
#include <iostream>

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop() = default;

    void ProgramLoop::operator()() {
        if (m_ViewportUpdateRequested) {
            std::cout << "viewport update requested: w " << m_ViewportNewWidth << " h " << m_ViewportNewHeight << '\n';
            m_ViewportUpdateRequested = false;
        }
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }
}
