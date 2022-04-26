#pragma once

namespace mrko900::gravity::app {
    enum class UserInput {
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        KEY_PRESSED,
        KEY_RELEASED
    };

    enum class MouseButton {
        RIGHT, LEFT, MIDDLE
    };

    struct MouseClickInputData {
        // x, y from (0, 0) at the lower left corner of the viewport
        unsigned short x, y;
        MouseButton button;
    };

    enum class KeyboardInputData {
        SPACE
    };
}
