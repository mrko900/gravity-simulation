#include "MassPoint.h"

namespace mrko900::gravity::physics {
    MassPoint::MassPoint(DynamicCoordinates* coordinates, float mass) : 
        m_Mass(mass), m_Coordinates(coordinates) {
    }

    float MassPoint::getMass() {
        return m_Mass;
    }

    void MassPoint::setMass(float mass) {
        m_Mass = mass;
    }

    DynamicCoordinates& MassPoint::coordinates() {
        return *m_Coordinates;
    }
}
