#pragma once

#include "DynamicCoordinates.h"
#include <unordered_map>

namespace mrko900::gravity::physics {
    class DynamicCoordinatesImpl : public DynamicCoordinates {
    public:
        float getCoordinate(unsigned int n) const override;
        void setCoordinate(unsigned int n, float coordinate) override;
        void removeCoordinate(unsigned int n) override;
        unsigned int dimensions() const;
        void pack(float* ptr) const;

    private:
        std::vector<float> m_Coordinates;
    };
}
