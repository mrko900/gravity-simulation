#pragma once

#include "DynamicCoordinates.h"
#include <functional>

namespace mrko900::gravity::physics {
    class MassPoint {
    public:
        MassPoint(DynamicCoordinates* coordinates, float mass = 0.0f);
        float getMass();
        void setMass(float mass);
        DynamicCoordinates& coordinates();
        
    private:
        float m_Mass;
        DynamicCoordinates* m_Coordinates;
    };
}
