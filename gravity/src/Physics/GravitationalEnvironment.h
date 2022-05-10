#pragma once

#include "MassPoint.h"
#include <map>
#include <set>

namespace mrko900::gravity::physics {
    struct GravityField {
        MassPoint* massPoint;
        bool dynamic;
        DynamicCoordinates* netGravitationalForce;
    };

    class GravitationalEnvironment {
    public:
        GravitationalEnvironment(float k);
        GravitationalEnvironment(float k,
            const std::function<void(std::pair<unsigned int, unsigned int> ids, 
                                     float distance, float gravitationalForce)>* distanceConsumer);
        void addEntity(unsigned int id, GravityField& entity);
        void removeEntity(unsigned int id);
        void addException(unsigned int first, unsigned int second);
        void removeException(unsigned int first, unsigned int second);
        void calculate();

    private:
        const std::function<void(std::pair<unsigned int, unsigned int> ids, 
                                 float distance, float gravitationalForce)>* m_DistanceConsumer;
        std::map<unsigned int, GravityField*> m_Entities;
        float m_Coef;

        // todo unordered_set
        std::set<std::pair<unsigned int, unsigned int>> m_Exceptions;
    };
}
