#pragma once

#include "MassPoint.h"
#include <map>

namespace mrko900::gravity::physics {
    class GravityField {
    public:
        MassPoint& massPoint;
        bool dynamic;
        DynamicCoordinates& netGravitationalForce;
    };

    class GravitationalEnvironment {
    public:
        GravitationalEnvironment(float k);
        void addEntity(unsigned int id, GravityField& entity);
        void removeEntity(unsigned int id);
        void calculate();

    private:
        std::map<unsigned int, GravityField*> m_Entities;
        float m_Coef;
    };
}
