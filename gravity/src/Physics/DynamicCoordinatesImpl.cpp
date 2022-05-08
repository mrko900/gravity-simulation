#include "DynamicCoordinatesImpl.h"

#include <stdexcept>
#include <iostream>
namespace mrko900::gravity::physics {
    float DynamicCoordinatesImpl::getCoordinate(unsigned int n) const {
        return m_Coordinates[n];
    }

    void DynamicCoordinatesImpl::setCoordinate(unsigned int n, float coordinate) {
        if (m_Coordinates.size() <= n) {
            m_Coordinates.resize(n + 1);
        }
        m_Coordinates[n] = coordinate;
    }

    void DynamicCoordinatesImpl::removeCoordinate(unsigned int n) {
        if (n >= m_Coordinates.size())
            throw std::invalid_argument("no such coordinate");
        m_Coordinates.erase(m_Coordinates.begin() + n, m_Coordinates.end());
    }

    unsigned int DynamicCoordinatesImpl::dimensions() const {
        return m_Coordinates.size();
    }

    void DynamicCoordinatesImpl::pack(float* ptr) const {
        for (int i = 0; i < m_Coordinates.size(); ++i)
            ptr[i] = m_Coordinates[i];
    }
}
