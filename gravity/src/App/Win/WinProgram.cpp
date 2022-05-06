#include "WinProgram.h"

#include <iostream>
#include <fstream>
#include "../../GL/Win/WinGLAccess.h"
#include "../../Graphics/Renderer.h"
#include "../../Graphics/GL/GLRenderer.h"
#include "../../GL/GLMacros.h"
#include <chrono>
#include "../UserInput.h"
#include <stdexcept>
#include <WindowsX.h>

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
using enum mrko900::gravity::app::UserInput;

static std::unordered_map<unsigned int, mrko900::gravity::app::KeyboardInputData> newKeyMap();

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
        m_ViewportNewHeight(0), m_GLHelper(nullptr), m_ProgramLoopRunning(false), m_ProgramLoop(nullptr) {
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
            "glDeleteBuffers",
            "glUniform4f",
            "glUniform1ui",
            "glBindTextureUnit",
            "glCreateTextures",
            "glBindTexture",
            "glTexStorage2D",
            "glTexSubImage2D",
            "glGenerateMipmap",
            "glCreateProgramPipelines",
            "glBindProgramPipeline",
            "glProgramParameteri",
            "glUseProgramStages",
            "glGetProgramInfoLog",
            "glProgramUniform1ui",
            "glProgramUniform4f",
            "glProgramUniform2f",
            "glProgramUniform1f"
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
            IGL_DELETE_BUFFERS,
            IGL_UNIFORM4F,
            IGL_UNIFORM1UI,
            IGL_BIND_TEXTURE_UNIT,
            IGL_CREATE_TEXTURES,
            IGL_BIND_TEXTURE,
            IGL_TEX_STORAGE2D,
            IGL_TEX_SUB_IMAGE2D,
            IGL_GENERATE_MIPMAP,
            IGL_CREATE_PROGRAM_PIPELINES,
            IGL_BIND_PROGRAM_PIPELINE,
            IGL_PROGRAM_PARAMETERI,
            IGL_USE_PROGRAM_STAGES,
            IGL_GET_PROGRAM_INFO_LOG,
            IGL_PROGRAM_UNIFORM1UI,
            IGL_PROGRAM_UNIFORM4F,
            IGL_PROGRAM_UNIFORM2F,
            IGL_PROGRAM_UNIFORM1F
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

        std::string rectVertexShaderStr = readFile("shaders/rect_vertex.shader");
        std::string simpleFragmentShaderStr = readFile("shaders/simple_fragment.shader");
        std::string circleFragmentShaderStr = readFile("shaders/circle_fragment.shader");
        GLRenderer::Shaders shaders;
        shaders.rectVertexShader = std::move(rectVertexShaderStr);
        shaders.simpleFragmentShader = std::move(simpleFragmentShaderStr);
        shaders.circleFragmentShader = std::move(circleFragmentShaderStr);
        GLRenderer glRenderer = GLRenderer(glHelper, std::move(shaders));
        glRenderer.init();
        glRenderer.setAutoGenTextureLevels(true);
        Renderer& renderer = glRenderer;
        RECT dimensions;
        GetClientRect(m_CurrentWindow, &dimensions);
        float ratio = (float) dimensions.bottom / (float) dimensions.right;
        renderer.coordinateSystem(-1.0f, 1.0f, -ratio, ratio);

        class A : public CoordinateSystemHelper {
        public:
            float k = 0.0f;
            float weighted(unsigned int coordinate, float normalized) {
                if (coordinate == 0) // x
                    return normalized;
                else if (coordinate == 1) // y
                    return normalized * k;
                else
                    throw std::invalid_argument("unknown coordinate: not 0 (x) or 1 (y)");
            }
        } coordinateSystemHelper;

        coordinateSystemHelper.k = ratio;

        ProgramLoop programLoop = ProgramLoop(renderer, coordinateSystemHelper, dimensions.right, dimensions.bottom);
        programLoop.init();
        m_ProgramLoop = &programLoop;
        m_ProgramLoopRunning = true;

        wglSwapIntervalEXT(0);

        std::chrono::steady_clock::time_point secondStart = std::chrono::high_resolution_clock::now();

        POINT p;
        GetCursorPos(&p);
        ScreenToClient(m_CurrentWindow, &p);
        m_CursorX = p.x;
        m_CursorY = p.y;

        unsigned int frames = 0;

        MSG msg;
        for (;;) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    break;
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            if (m_ViewportUpdateRequested) {
                ratio = (float) m_ViewportNewHeight / (float) m_ViewportNewWidth;
                coordinateSystemHelper.k = ratio;
                programLoop.updateViewport(m_ViewportNewWidth, m_ViewportNewHeight);
                renderer.coordinateSystem(-1.0f, 1.0f, -ratio, ratio);
                m_ViewportUpdateRequested = false;
            }

            programLoop.run();
            
            ++frames;

            std::chrono::steady_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            if (currentTime - secondStart > std::chrono::seconds(1)) {
                secondStart = currentTime;
                //std::cout << "fps: " << frames << '\n';
                frames = 0;
            }

            SwapBuffers(hdc);
        }

        m_ProgramLoopRunning = false;
        m_ProgramLoop = nullptr;
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
            obj.updateViewport(LOWORD(lParam), HIWORD(lParam));
    }

    void WinProgram::onWmKey(HWND hWnd, WPARAM wParam, UserInput ui) {
        WinProgram& obj = *((WinProgram*) GetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX));

        if (!obj.m_ProgramLoopRunning)
            return;

        std::unordered_map<unsigned int, KeyboardInputData> keyMap = newKeyMap();

        if (keyMap.contains(wParam)) {
            KeyboardInputData data = keyMap[wParam];
            obj.m_ProgramLoop->userInput(ui, &data);
        }
    }

    void WinProgram::onWmKeyDown(HWND hWnd, WPARAM wParam) {
        onWmKey(hWnd, wParam, KEY_PRESSED);
    }

    void WinProgram::onWmKeyUp(HWND hWnd, WPARAM wParam) {
        onWmKey(hWnd, wParam, KEY_RELEASED);
    }

    void WinProgram::onWmMouseButton(HWND hWnd, LPARAM lParam, MouseButton mouseButton, bool down) {
        WinProgram& obj = *((WinProgram*) GetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX));
        if (!obj.m_ProgramLoopRunning)
            return;
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        MouseClickInputData data {
            static_cast<unsigned short>(GET_X_LPARAM(lParam)),
            static_cast<unsigned short>(clientRect.bottom - GET_Y_LPARAM(lParam)),
            mouseButton
        };
        obj.m_ProgramLoop->userInput(down ? MOUSE_PRESSED : MOUSE_RELEASED, &data);
    }

    void WinProgram::onWmLButtonDown(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::LEFT, true);
    }

    void WinProgram::onWmRButtonDown(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::RIGHT, true);
    }

    void WinProgram::onWmMButtonDown(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::MIDDLE, true);
    }

    void WinProgram::onWmLButtonUp(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::LEFT, false);
    }

    void WinProgram::onWmRButtonUp(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::RIGHT, false);
    }

    void WinProgram::onWmMButtonUp(HWND hWnd, LPARAM lParam) {
        onWmMouseButton(hWnd, lParam, MouseButton::MIDDLE, false);
    }

    void WinProgram::onWmMouseWheel(HWND hWnd, WPARAM wParam) {
        WinProgram& obj = *((WinProgram*) GetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX));
        if (!obj.m_ProgramLoopRunning)
            return;
        short param = GET_WHEEL_DELTA_WPARAM(wParam);
        param /= abs(param);
        float data = param;
        obj.m_ProgramLoop->userInput(MOUSE_WHEEL, &data);
    }

    void WinProgram::onWmMouseMove(HWND hWnd, LPARAM lParam) {
        WinProgram& obj = *((WinProgram*) GetWindowLongPtrW(hWnd, OBJ_WINDOW_LONG_PTR_INDEX));
        if (!obj.m_ProgramLoopRunning)
            return;
        WORD x = GET_X_LPARAM(lParam);
        WORD y = GET_Y_LPARAM(lParam);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        y = clientRect.bottom - y;
        MouseMoveInputData data {
            obj.m_CursorX, obj.m_CursorY,
            static_cast<unsigned short>(x), static_cast<unsigned short>(y) 
        };
        obj.m_ProgramLoop->userInput(MOUSE_MOVE, &data);

        obj.m_CursorX = x;
        obj.m_CursorY = y;
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
            case WM_KEYDOWN:
                onWmKeyDown(hWnd, wParam);
                break;
            case WM_KEYUP:
                onWmKeyUp(hWnd, wParam);
                break;
            case WM_LBUTTONDOWN:
                onWmLButtonDown(hWnd, lParam);
                break;
            case WM_RBUTTONDOWN:
                onWmRButtonDown(hWnd, lParam);
                break;
            case WM_MBUTTONDOWN:
                onWmMButtonDown(hWnd, lParam);
                break;
            case WM_LBUTTONUP:
                onWmLButtonUp(hWnd, lParam);
                break;
            case WM_RBUTTONUP:
                onWmRButtonUp(hWnd, lParam);
                break;
            case WM_MBUTTONUP:
                onWmMButtonUp(hWnd, lParam);
                break;
            case WM_MOUSEWHEEL:
                onWmMouseWheel(hWnd, wParam);
                break;
            case WM_MOUSEMOVE:
                onWmMouseMove(hWnd, lParam);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
}

static std::unordered_map<unsigned int, mrko900::gravity::app::KeyboardInputData> newKeyMap() {
    using enum mrko900::gravity::app::KeyboardInputData;
    return {
        { VK_SPACE, KEY_SPACE },
        { 0x30, KEY_0 },
        { 0x31, KEY_1 },
        { 0x32, KEY_2 },
        { 0x33, KEY_3 },
        { 0x34, KEY_4 },
        { 0x35, KEY_5 },
        { 0x36, KEY_6 },
        { 0x37, KEY_7 },
        { 0x38, KEY_8 },
        { 0x39, KEY_9 },
        { VK_OEM_PERIOD, KEY_PERIOD },
        { VK_OEM_MINUS, KEY_MINUS }
    };
}
