#pragma once

#include "MassPoint.h"
#include <unordered_map>

namespace mrko900::gravity::physics {
    class GravityField {
    public:
        MassPoint& massPoint;
        bool dynamic;
        DynamicCoordinates& netGravitationalForce;
    };

    class GravitationalEnvironment {
    public:
        void addEntity(unsigned int id, GravityField& entity);
        void removeEntity(unsigned int id);
        void calculate();

    private:
        std::unordered_map<unsigned int, GravityField*> m_Entities;
    };
}
