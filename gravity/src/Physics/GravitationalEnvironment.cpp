#include "GravitationalEnvironment.h"

namespace mrko900::gravity::physics {
    GravitationalEnvironment::~GravitationalEnvironment() = default;

    void GravitationalEnvironment::addEntity(unsigned int id, GravityField& entity) {
        m_Entities.insert(std::make_pair(id, &entity));
    }

    void GravitationalEnvironment::removeEntity(unsigned int id) {
        m_Entities.erase(id);
    }

    void GravitationalEnvironment::calculate() {
        for (auto& entry : m_Entities) {
            for ()
        }
    }
}
