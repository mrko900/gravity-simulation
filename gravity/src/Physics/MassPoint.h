#pragma once

#include "DynamicCoordinates.h"
#include <functional>

namespace mrko900::gravity::physics {
    struct MassPoint {
        float mass;
        DynamicCoordinates* coordinates;
    };
}
