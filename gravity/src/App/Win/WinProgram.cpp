#include "WinProgram.h"

#include <iostream>
#include <fstream>
#include "../../GL/Win/WinGLAccess.h"
#include "../../Graphics/Renderer.h"
#include "../../Graphics/GL/GLRenderer.h"
#include "../../GL/GLMacros.h"
#include <chrono>

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS glHelper

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

using mrko900::gravity::gl::win::WinGLAccess;
using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Renderer;
using mrko900::gravity::graphics::gl::GLRenderer;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using enum mrko900::gravity::gl::GLHelper::Function;

namespace mrko900::gravity::app::win {
    static std::string readFile(const std::string& path) {
        std::ifstream inputStream;
        inputStream.open(path);
        std::string s;
        while (true) {
            const char i = inputStream.get();
            if (i == -1)
                break;
            s += i;
        }
        inputStream.close();
        return s;
    }

    WinProgram::WinProgram(HINSTANCE hInstance, int nCmdShow) : m_HInstance(hInstance), m_NCmdShow(nCmdShow),
        m_CurrentWindow(NULL), m_RunningGL(false), m_ViewportUpdateRequested(false), m_ViewportNewWidth(0),
        m_ViewportNewHeight(0), m_GLHelper(nullptr) {
    }

    void WinProgram::run() {
        WNDCLASSEXW windowClass = wndClass(m_HInstance);
        if (!GetClassInfoExW(m_HInstance, WND_CLASS_NAME, &windowClass))
            RegisterClassExW(&windowClass);

        float width = 1600.0f;
        float height = 900.0f;

        CreateWindowExW( // return value ignored - triggers WM_CREATE message which assigns m_CurrentWindow
            0,
            WND_CLASS_NAME,
            L"The Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
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
            "wglGetSwapIntervalEXT",
            "glCreateVertexArrays",
            "glBindVertexArray",
            "glCreateProgram",
            "glCreateShader",
            "glShaderSource",
            "glCompileShader",
            "glAttachShader",
            "glLinkProgram",
            "glDeleteShader",
            "glUseProgram",
            "glGetError",
            "glGetShaderInfoLog",
            "glClearBufferfv",
            "glDrawArrays",
            "glViewport",
            "glUniform1f",
            "glCreateBuffers",
            "glBindBuffer",
            "glBufferStorage",
            "glVertexAttribBinding",
            "glEnableVertexAttribArray",
            "glVertexAttribFormat",
            "glBindVertexBuffer",
            "glBlendFunc",
            "glEnable",
            "glUniform2f",
            "glBufferSubData",
            "glGetIntegerv",
            "glDisable",
            "glDeleteBuffers"
        };
        std::vector<GLHelper::Function> glLoadFuncIds {
            IGL_GET_STRING,
            IWGL_SWAP_INTERVAL_EXT,
            IWGL_GET_SWAP_INTERVAL_EXT,
            IGL_CREATE_VERTEX_ARRAYS,
            IGL_BIND_VERTEX_ARRAY,
            IGL_CREATE_PROGRAM,
            IGL_CREATE_SHADER,
            IGL_SHADER_SOURCE,
            IGL_COMPILE_SHADER,
            IGL_ATTACH_SHADER,
            IGL_LINK_PROGRAM,
            IGL_DELETE_SHADER,
            IGL_USE_PROGRAM,
            IGL_GET_ERROR,
            IGL_GET_SHADER_INFO_LOG,
            IGL_CLEAR_BUFFERFV,
            IGL_DRAW_ARRAYS,
            IGL_VIEWPORT,
            IGL_UNIFORM1F,
            IGL_CREATE_BUFFERS,
            IGL_BIND_BUFFER,
            IGL_BUFFER_STORAGE,
            IGL_VERTEX_ATTRIB_BINDING,
            IGL_ENABLE_VERTEX_ATTRIB_ARRAY,
            IGL_VERTEX_ATTRIB_FORMAT,
            IGL_BIND_VERTEX_BUFFER,
            IGL_BLEND_FUNC,
            IGL_ENABLE,
            IGL_UNIFORM2F,
            IGL_BUFFER_SUB_DATA,
            IGL_GET_INTEGERV,
            IGL_DISABLE,
            IGL_DELETE_BUFFERS
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

        ShowWindow(m_CurrentWindow, m_NCmdShow);
        UpdateWindow(m_CurrentWindow);
        HDC hdc = GetDC(m_CurrentWindow);
        m_RunningGL = true;

        const std::string vertexShaderStr = readFile("shaders/test_vertex.shader");
        const std::string fragmentShaderStr = readFile("shaders/test_fragment.shader");
        GLRenderer glRenderer = GLRenderer(glHelper, { vertexShaderStr, fragmentShaderStr });
        glRenderer.init();
        Renderer& renderer = glRenderer;
        RECT dimensions;
        GetClientRect(m_CurrentWindow, &dimensions);
        float ratio = (float) dimensions.bottom / (float) dimensions.right;
        renderer.coordinateSystem(-1.0f, 1.0f, -ratio, ratio);

        ProgramLoop programLoop = ProgramLoop(renderer);

        wglSwapIntervalEXT(0);

        std::chrono::steady_clock::time_point secondStart = std::chrono::high_resolution_clock::now();

        unsigned int frames = 0;

        MSG msg;
        float loop = 0.0f;
        int i = 0;
        float pi = 2 * asinf(1);
        for (;;) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            if (m_ViewportUpdateRequested) {
                ratio = (float) m_ViewportNewHeight / (float) m_ViewportNewWidth;
                programLoop.updateViewport(m_ViewportNewWidth, m_ViewportNewHeight);
                renderer.coordinateSystem(-1.0f, 1.0f, -ratio, ratio);
                m_ViewportUpdateRequested = false;
            }

            programLoop();
            
            ++frames;

            std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            if (currentTime - secondStart > std::chrono::seconds(1)) {
                secondStart = currentTime;
                std::cout << "fps: " << frames << '\n';
                frames = 0;
            }

            SwapBuffers(hdc);
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
