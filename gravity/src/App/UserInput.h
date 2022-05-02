#pragma once

namespace mrko900::gravity::app {
    enum class UserInput {
        MOUSE_PRESSED, // - MouseClickInputData
        MOUSE_RELEASED, // - MouseClickInputData
        KEY_PRESSED, // - KeyboardInputData
        KEY_RELEASED, // - KeyboardInputData
        MOUSE_WHEEL // - float
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
