#include "VectorModelImpl.h"

#include <stdexcept>

namespace mrko900::gravity::physics {
    VectorModelImpl::VectorModelImpl(unsigned int dimensions) : m_Dimensions(dimensions) {
    }

    void VectorModelImpl::addVector(unsigned int id, DynamicCoordinates& vector) {
        if (vector.dimensions() != m_Dimensions)
            throw std::invalid_argument("vector dimensionality does not match the dimensionality of the vector model");
        m_Vectors.insert(std::make_pair(id, &vector));
    }

    void VectorModelImpl::removeVector(unsigned int id) {
        m_Vectors.erase(id);
    }

    unsigned int VectorModelImpl::dimensions() {
        return m_Dimensions;
    }

    void VectorModelImpl::pack(DynamicCoordinates** ptr) {
        int i = 0;
        for (auto& entry : m_Vectors) {
            ptr[i] = entry.second;
            ++i;
        }
    }

    void VectorModelImpl::sum(float* result) {
        for (int i = 0; i < m_Dimensions; ++i) {
            for (auto& entry : m_Vectors) {
                result[i] += entry.second->getCoordinate(i);
            }
        }
    }
}   
