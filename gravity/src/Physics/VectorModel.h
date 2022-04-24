#pragma once

#include "DynamicCoordinates.h"

namespace mrko900::gravity::physics {
    class VectorModel {
    public:
        virtual ~VectorModel();
        virtual void addVector(unsigned int id, DynamicCoordinates& vector) = 0;
        virtual void removeVector(unsigned int id) = 0;
        virtual unsigned int dimensions() = 0;
        virtual void pack(DynamicCoordinates** ptr) = 0;
        virtual void sum(float* result) = 0;
    };
}
