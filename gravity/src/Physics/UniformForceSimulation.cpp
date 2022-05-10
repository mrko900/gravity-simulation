#include "UniformForceSimulation.h"

#include <vector>

namespace mrko900::gravity::physics {
    void UniformForceSimulation::addEntity(unsigned int id, DynamicPoint& entity) {
        m_Entities.insert(std::make_pair(id, &entity));
    }

    void UniformForceSimulation::replaceEntity(unsigned int id, DynamicPoint& entity) {
        m_Entities.at(id) = &entity;
    }

    void UniformForceSimulation::removeEntity(unsigned int id) {
        m_Entities.erase(id);
    }

    void UniformForceSimulation::simulateDisplacement(float timeElapsed) {
        for (auto& entry : m_Entities) {
            DynamicPoint& entity = *entry.second;
            unsigned int dimensions = entity.forceModel->dimensions();
            if (dimensions == 0)
                break;
            std::vector<float> netForce(dimensions);
            entity.forceModel->sum(&netForce.front());
            float k = (timeElapsed * timeElapsed) / (2 * entity.massPoint->mass);
            DynamicCoordinates& coordinates = *entity.massPoint->coordinates;
            for (int i = 0; i < dimensions; ++i)
                coordinates.setCoordinate(i, coordinates.getCoordinate(i)
                    + k * netForce[i] + entity.velocity->getCoordinate(i) * timeElapsed);
        }
    }

    void UniformForceSimulation::simulate(float timeElapsed) {
        for (auto& entry : m_Entities) {
            DynamicPoint& entity = *entry.second;
            unsigned int dimensions = entity.forceModel->dimensions();
            if (dimensions == 0)
                break;
            std::vector<float> netForce(dimensions);
            entity.forceModel->sum(&netForce.front());
            float k = (timeElapsed * timeElapsed) / (2 * entity.massPoint->mass);
            DynamicCoordinates& coordinates = *entity.massPoint->coordinates;
            for (int i = 0; i < dimensions; ++i)
                coordinates.setCoordinate(i, coordinates.getCoordinate(i) 
                    + k * netForce[i] + entity.velocity->getCoordinate(i) * timeElapsed);
            for (int i = 0; i < dimensions; ++i)
                entity.velocity->setCoordinate(i, entity.velocity->getCoordinate(i) 
                    + netForce[i] * timeElapsed / entity.massPoint->mass);
        }
    }
}
