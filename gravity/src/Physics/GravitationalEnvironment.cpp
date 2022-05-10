#include "GravitationalEnvironment.h"

#include <cmath>
#include <algorithm>

#include <iostream> // todo remove
namespace mrko900::gravity::physics {
    GravitationalEnvironment::GravitationalEnvironment(float k) : GravitationalEnvironment(k, nullptr) {
    }

    GravitationalEnvironment::GravitationalEnvironment(float k,
        const std::function<void(std::pair<unsigned int, unsigned int> ids, 
                                 float distance, float gravitationalForce)>* distanceConsumer) :
        m_Coef(k), m_DistanceConsumer(distanceConsumer) {
    }

    void GravitationalEnvironment::addEntity(unsigned int id, GravityField& entity) {
        m_Entities.insert(std::make_pair(id, &entity));
    }

    void GravitationalEnvironment::removeEntity(unsigned int id) {
        m_Entities.erase(id);
    }

    static float calcdist(MassPoint& m1, MassPoint m2) {
        float s = 0.0f;
        int dim = std::max(m1.coordinates->dimensions(), m2.coordinates->dimensions());
        for (int i = 0; i < dim; ++i) {
            float d = m1.coordinates->dimensions() > i ? m1.coordinates->getCoordinate(i) : 0.0f
                      - m2.coordinates->dimensions() > i ? m2.coordinates->getCoordinate(i) : 0.0f;
            s += d * d;
        }
        return sqrt(s);
    }

    void GravitationalEnvironment::addException(unsigned int first, unsigned int second) {
        m_Exceptions.insert({ first, second });
    }

    void GravitationalEnvironment::removeException(unsigned int first, unsigned int second) {
        m_Exceptions.erase({ first, second });
        m_Exceptions.erase({ second, first });
    }

    void GravitationalEnvironment::calculate() {
        if (m_Entities.size() == 0)
            return;
        auto iEnd = m_Entities.end();
        --iEnd;
        for (auto& entry : m_Entities) {
            for (int i = 0; i < entry.second->netGravitationalForce->dimensions(); ++i)
                entry.second->netGravitationalForce->setCoordinate(i, 0.0f);
        }

        struct CallbackData {
            unsigned int obj1, obj2;
            float distance, gravitationalForce;
        };

        std::vector<CallbackData> distances;

        for (auto iIt = m_Entities.begin(); iIt != iEnd; ++iIt) {
            for (auto jIt = iIt; jIt != m_Entities.end(); ++jIt) {
                unsigned int iId = iIt->first, jId = jIt->first;

                if (iId == jId)
                    continue;

                if (m_Exceptions.contains({ iId, jId }) || m_Exceptions.contains({ jId, iId }))
                    continue;

                DynamicCoordinates& iCoords = *iIt->second->massPoint->coordinates;
                DynamicCoordinates& jCoords = *jIt->second->massPoint->coordinates;
                float sqdist = 0.0f;
                for (unsigned int d = 0; d < iCoords.dimensions(); ++d) {
                    sqdist += pow(iCoords.getCoordinate(d) - jCoords.getCoordinate(d), 2);
                }
                float iMass = iIt->second->massPoint->mass;
                float jMass = jIt->second->massPoint->mass;
                float forceMagn = m_Coef * iMass * jMass / sqdist;

                float dist = sqrt(sqdist);
                
                distances.push_back(CallbackData { iId, jId, dist, forceMagn });

                float k = forceMagn / dist;

                for (unsigned int d = 0; d < iCoords.dimensions(); ++d) {
                    for (char c = 0; c < 2; ++c) {
                        if (!(c == 0 ? iIt : jIt)->second->dynamic)
                            continue;

                        DynamicCoordinates& netGravForce = *(c == 0 ? iIt : jIt)->second->netGravitationalForce;
                        DynamicCoordinates& myCoords = c == 0 ? iCoords : jCoords;
                        DynamicCoordinates& theirCoords = c == 0 ? jCoords : iCoords;
                        float signedDist = theirCoords.getCoordinate(d) - myCoords.getCoordinate(d);
                        netGravForce.setCoordinate(d, netGravForce.getCoordinate(d) + signedDist * k);
                    }
                }
            }
        }

        if (m_DistanceConsumer != nullptr) {
            for (const CallbackData& callbackData : distances)
                (*m_DistanceConsumer)({ callbackData.obj1, callbackData.obj2 }, 
                    callbackData.distance, callbackData.gravitationalForce);
        }
    }
}
