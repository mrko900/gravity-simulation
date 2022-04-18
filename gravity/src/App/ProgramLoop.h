#pragma once

namespace mrko900::gravity::app {
    class ProgramLoop {
    public:
        ProgramLoop();
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
    };
}
