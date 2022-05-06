#pragma once

namespace mrko900::gravity::app {
    enum class UserInput {
        MOUSE_PRESSED, // - MouseClickInputData
        MOUSE_RELEASED, // - MouseClickInputData
        KEY_PRESSED, // - KeyboardInputData
        KEY_RELEASED, // - KeyboardInputData
        MOUSE_WHEEL, // - float,
        MOUSE_MOVE // - MouseMoveInputData
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
        KEY_SPACE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0
    };

    struct MouseMoveInputData {
        unsigned short fromX, fromY, toX, toY;
    };
}
