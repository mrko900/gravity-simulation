#include "WinProgram.h"
#include <iostream>
#include "../../GL/Win/WinGLAccess.h"
#include "../../GL/GLHelper.h"
#include "../../GL/GLMacros.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS glHelper

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

using mrko900::gravity::gl::win::WinGLAccess;
using mrko900::gravity::gl::GLHelper;

using enum mrko900::gravity::gl::GLHelper::Function;

namespace mrko900::gravity::app::win {
    WinProgram::WinProgram(HINSTANCE hInstance, int nCmdShow, ProgramLoop& programLoop) : 
        m_HInstance(hInstance), m_NCmdShow(nCmdShow), m_ProgramLoop(programLoop) {
    }

    void WinProgram::run() {
        WNDCLASSEXW windowClass = wndClass(m_HInstance);
        if (!GetClassInfoExW(m_HInstance, WND_CLASS_NAME, &windowClass))
            RegisterClassExW(&windowClass);

        CreateWindowExW( // return value ignored - triggers WM_CREATE message which assigns m_CurrentWindow
            0,
            WND_CLASS_NAME,
            L"The Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            1600, 900,
            nullptr,
            nullptr,
            m_HInstance,
            this
        );

        HMODULE opengl32 = LoadLibraryA("opengl32.dll");
        WinGLAccess glAccess(opengl32);

        std::vector<const char*> glLoadFuncs {
            "glGetString",
            "wglSwapIntervalEXT",
            "wglGetSwapIntervalEXT"
        };
        std::vector<GLHelper::Function> glLoadFuncIds {
            IGL_GET_STRING,
            IWGL_SWAP_INTERVAL_EXT,
            IWGL_GET_SWAP_INTERVAL_EXT
        };

        int index;
        GLHelper glHelper = GLHelper([&glAccess, &glLoadFuncs, &index] (const GLHelper::Function funcId) {
            return glAccess.getFunction(glLoadFuncs[index]);
        });
        m_GLHelper = &glHelper;
        for (int i = 0; i < glLoadFuncs.size(); i++) {
            index = i;
            glAccess.loadFunction(glLoadFuncs[i]);
            m_GLHelper->initFunction(glLoadFuncIds[i]);
        }

        std::cout << glGetString(GL_VERSION) << '\n';

        wglSwapIntervalEXT(0);
        std::cout << "Swap interval: " << wglGetSwapIntervalEXT() << '\n';
        ShowWindow(m_CurrentWindow, m_NCmdShow);
        UpdateWindow(m_CurrentWindow);
        HDC hdc = GetDC(m_CurrentWindow);
        m_RunningGL = true;

        MSG msg;
        for (;;) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            if (m_ViewportUpdateRequested) {
                m_ProgramLoop.updateViewport(m_ViewportNewWidth, m_ViewportNewHeight);
                m_ViewportUpdateRequested = false;
            }

            m_ProgramLoop();
        }
    }

    WNDCLASSEXW WinProgram::wndClass(HINSTANCE hInstance) {
        WNDCLASSEXW wc;

        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &wndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = sizeof(LONG_PTR);
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = WND_CLASS_NAME;
        wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

        return wc;
    }

    void WinProgram::initGL() {
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        HDC hdc = GetDC(m_CurrentWindow);

        int letWindowsChooseThisPixelFormat;

        letWindowsChooseThisPixelFormat = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, letWindowsChooseThisPixelFormat, &pfd);

        HGLRC oldContext = wglCreateContext(hdc);

        wglMakeCurrent(hdc, oldContext);

        HGLRC (*wglCreateContextAttribsARB) (HDC, HGLRC, const int*) =
            (HGLRC (*) (HDC, HGLRC, const int*)) wglGetProcAddress("wglCreateContextAttribsARB");
        const int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        HGLRC context = wglCreateContextAttribsARB(hdc, NULL, attribs);

        BOOL (*wglMakeContextCurrentARB) (HDC, HDC, HGLRC) =
            (BOOL (*) (HDC, HDC, HGLRC)) wglGetProcAddress("wglMakeContextCurrentARB");

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(oldContext);
        wglMakeContextCurrentARB(hdc, hdc, context);
    }

    void WinProgram::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }

    void WinProgram::onWmCreate(HWND hWnd, LPARAM lParam) {
        WinProgram* obj = (WinProgram*) ((CREATESTRUCT*) lParam)->lpCreateParams;
        SetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX, (LONG_PTR) obj);
        obj->m_CurrentWindow = hWnd;
        obj->initGL();
    }

    void WinProgram::onWmSize(HWND hWnd, LPARAM lParam) {
        WinProgram& obj = *((WinProgram*) GetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX));
        if (obj.m_RunningGL)
            // requesting viewport update in a rendering loop instead of updating it here right away
            obj.updateViewport(LOWORD(lParam), HIWORD(lParam));
    }

    LRESULT WinProgram::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_PAINT:
                break;
            case WM_CREATE:
                onWmCreate(hWnd, lParam);
                break;
            case WM_SIZE:
                onWmSize(hWnd, lParam);
                break;
            case WM_DESTROY:
                PostQuitMessage(EXIT_SUCCESS);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }
}
