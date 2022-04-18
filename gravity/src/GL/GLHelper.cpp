#include "GLHelper.h"

#include <stdexcept>
#include <iostream>

namespace mrko900::gravity::gl {
    GLHelper::GLHelper(std::function<FuncPtr(Function)> funcLoader) : m_FuncLoader(funcLoader) {
    }

    void GLHelper::initFunction(Function function) {
        if (function >= LEN || function < 0)
            throw std::invalid_argument("unknown function: " + function);
        funcPtrs[function] = m_FuncLoader(function);
    }
}
