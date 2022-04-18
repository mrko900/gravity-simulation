#include "WinGLAccess.h"

namespace mrko900::gravity::gl::win {
    WinGLAccess::WinGLAccess() : WinGLAccess(nullptr) {
    }

    WinGLAccess::WinGLAccess(const HMODULE legacyLoadModule) :
        m_LegacyLoadModule(legacyLoadModule) {
    }

    bool WinGLAccess::loadFunction(const LPCSTR name) {
        if (!wglLoadFunction(name) && !legacyLoadFunction(name))
            return false;
        return true;
    }

    bool WinGLAccess::legacyLoadFunction(const LPCSTR name) {
        if (m_LegacyLoadModule == nullptr)
            return false;

        if (m_FunctionMap.contains(name))
            return false;

        FARPROC funcPtr = GetProcAddress(m_LegacyLoadModule, name);
        if (!funcPtr)
            return false;
        m_FunctionMap[name] = funcPtr;

        return true;
    }

    bool WinGLAccess::wglLoadFunction(const LPCSTR name) {
        if (m_FunctionMap.contains(name))
            return false;
        FARPROC funcPtr = wglGetProcAddress(name);
        if (!funcPtr)
            return false;
        m_FunctionMap[name] = funcPtr;
        return true;
    }

    bool WinGLAccess::unloadFunction(const LPCSTR name) {
        if (!m_FunctionMap.contains(name))
            return false;
        m_FunctionMap.erase(name);
    }

    bool WinGLAccess::isFunctionLoaded(const LPCSTR pointer) const {
        return m_FunctionMap.contains(pointer);
    }

    GLAccess::FuncPtr WinGLAccess::getFunction(const LPCSTR name) const {
        if (!m_FunctionMap.contains(name))
            return nullptr;
        return (FuncPtr) m_FunctionMap.at(name);
    }
};
