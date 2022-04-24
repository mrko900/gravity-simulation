#pragma once

#include "VectorModel.h"
#include <unordered_map>

namespace mrko900::gravity::physics {
    class VectorModelImpl : public VectorModel {
    public:
        VectorModelImpl(unsigned int dimensions);
        void addVector(unsigned int id, DynamicCoordinates& vector) override;
        void removeVector(unsigned int id) override;
        unsigned int dimensions() override;
        void pack(DynamicCoordinates** ptr) override;
        void sum(float* result) override;

    private:
        std::unordered_map<unsigned int, DynamicCoordinates*> m_Vectors;
        unsigned int m_Dimensions;
    };
}
