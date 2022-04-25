#include "GravitationalEnvironment.h"

#include <cmath>
#include <algorithm>

#include <iostream>

namespace mrko900::gravity::physics {
    GravitationalEnvironment::GravitationalEnvironment(float k) : m_Coef(k) {
    }

    void GravitationalEnvironment::addEntity(unsigned int id, GravityField& entity) {
        m_Entities.insert(std::make_pair(id, &entity));
    }

    void GravitationalEnvironment::removeEntity(unsigned int id) {
        m_Entities.erase(id);
    }

    static float calcdist(MassPoint& m1, MassPoint m2) {
        float s = 0.0f;
        int dim = std::max(m1.coordinates().dimensions(), m2.coordinates().dimensions());
        for (int i = 0; i < dim; ++i) {
            float d = m1.coordinates().dimensions() > i ? m1.coordinates().getCoordinate(i) : 0.0f
                      - m2.coordinates().dimensions() > i ? m2.coordinates().getCoordinate(i) : 0.0f;
            s += d * d;
        }
        return sqrt(s);
    }

    void GravitationalEnvironment::calculate() {
        if (m_Entities.size() == 0)
            return;
        auto iEnd = m_Entities.end();
        --iEnd;
        for (auto& entry : m_Entities) {
            for (int i = 0; i < entry.second->netGravitationalForce.dimensions(); ++i)
                entry.second->netGravitationalForce.setCoordinate(i, 0.0f);
        }
        for (auto iIt = m_Entities.begin(); iIt != iEnd; ++iIt) {
            for (auto jIt = iIt; jIt != m_Entities.end(); ++jIt) {
                if (jIt == iIt)
                    continue;
                for (int d = 0; d < iIt->second->massPoint.coordinates().dimensions(); ++d) {
                    if (!iIt->second->dynamic && !jIt->second->dynamic)
                        continue;
                    float dist = abs(iIt->second->massPoint.coordinates().getCoordinate(d) 
                                     - jIt->second->massPoint.coordinates().getCoordinate(d));
                    float force = m_Coef * iIt->second->massPoint.getMass() * jIt->second->massPoint.getMass()
                                  / (dist * dist);
                    if (iIt->second->dynamic)
                        iIt->second->netGravitationalForce.setCoordinate(d, 
                            iIt->second->netGravitationalForce.getCoordinate(d) + force);
                    if (jIt->second->dynamic)
                        jIt->second->netGravitationalForce.setCoordinate(d, 
                            jIt->second->netGravitationalForce.getCoordinate(d) + force);
                }
            }
        }
    }
}
