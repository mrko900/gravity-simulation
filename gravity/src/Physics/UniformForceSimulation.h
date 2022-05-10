#pragma once

#include "ForceSimulation.h"
#include <unordered_map>

namespace mrko900::gravity::physics {
    class UniformForceSimulation : public ForceSimulation {
    public:
        void addEntity(unsigned int id, DynamicPoint& entity) override;
        void replaceEntity(unsigned int id, DynamicPoint& entity) override;
        void removeEntity(unsigned int id) override;
        void simulateDisplacement(float timeElapsed);
        void simulate(float timeElapsed) override;

    private:
        std::unordered_map<unsigned int, DynamicPoint*> m_Entities;
    };
}
