#pragma once

#include "../GLAccess.h";
#include <Windows.h>
#include <unordered_map>
#include <string>

namespace mrko900::gravity::gl::win {
    class WinGLAccess : public GLAccess {
    public:
        // public constructors
        WinGLAccess();
        WinGLAccess(const HMODULE legacyLoadModule);
        WinGLAccess(const WinGLAccess& other) = delete;
        WinGLAccess(WinGLAccess&& other) = delete;

        // public operators
        WinGLAccess& operator=(const WinGLAccess&) = delete;
        WinGLAccess& operator=(WinGLAccess&&) = delete;

        // public functions
        bool loadFunction(const LPCSTR name) override;
        bool legacyLoadFunction(const LPCSTR name);
        bool wglLoadFunction(const LPCSTR name);
        bool unloadFunction(const LPCSTR name) override;
        bool isFunctionLoaded(const LPCSTR name) const override;
        FuncPtr getFunction(const LPCSTR name) const override;

    private:
        // private member variables
        const HMODULE m_LegacyLoadModule;
        std::unordered_map<std::string, FARPROC> m_FunctionMap {};
    };
}
