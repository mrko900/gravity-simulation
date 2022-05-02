#pragma once

#include "MassPoint.h"
#include "VectorModel.h"

namespace mrko900::gravity::physics {
    struct DynamicPoint {
        MassPoint* massPoint;
        VectorModel* forceModel;
        DynamicCoordinates* velocity;
    };

    class ForceSimulation {
    public:
        ~ForceSimulation();
        virtual void addEntity(unsigned int id, DynamicPoint& entity) = 0;
        virtual void replaceEntity(unsigned int id, DynamicPoint& entity) = 0;
        virtual void removeEntity(unsigned int id) = 0;
        virtual void simulate(float timeElapsed) = 0;
    };
}
