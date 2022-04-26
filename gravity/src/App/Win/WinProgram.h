#pragma once

#include <Windows.h>
#include "../../GL/GLHelper.h"
#include "../ProgramLoop.h"

namespace mrko900::gravity::app::win {
    class WinProgram {
    public:
        WinProgram(HINSTANCE hInstance, int nCmdShow);
        WinProgram(const WinProgram& other) = delete;
        WinProgram(WinProgram&& other) = delete;
        WinProgram& operator=(const WinProgram& other) = delete;
        WinProgram& operator=(WinProgram&& other) = delete;
        void run();

        static constexpr LPCWSTR WND_CLASS_NAME = L"mrko900::gravity::ProgramLoop";

    private:
        bool m_RunningGL;
        HWND m_CurrentWindow;
        int m_NCmdShow;
        HINSTANCE m_HInstance;
        bool m_ViewportUpdateRequested;
        unsigned short m_ViewportNewWidth;
        unsigned short m_ViewportNewHeight;
        mrko900::gravity::gl::GLHelper* m_GLHelper;
        bool m_ProgramLoopRunning;
        mrko900::gravity::app::ProgramLoop* m_ProgramLoop;

        void initGL();
        void updateViewport(unsigned short newWidth, unsigned short newHeight);

        static constexpr int OBJ_WINDOW_LONG_PTR_INDEX = 0;
        static WNDCLASSEXW wndClass(HINSTANCE hInstance);
        static LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        static void onWmCreate(HWND hWnd, LPARAM lParam);
        static void onWmSize(HWND hWnd, LPARAM lParam);
        static void onWmKey(HWND hWnd, WPARAM wParam, UserInput ui);
        static void onWmKeyDown(HWND hWnd, WPARAM wParam);
        static void onWmKeyUp(HWND hWnd, WPARAM wParam);
    };
}
