#pragma once

namespace mrko900::gravity::app {
    enum class UserInput {
        LEFT_CLICK,
        RIGHT_CLICK,
        KEY_PRESSED,
        KEY_RELEASED
    };

    struct MouseClickInputData {
        unsigned short x, y;
    };

    enum class KeyboardInputData {
        SPACE
    };
}
