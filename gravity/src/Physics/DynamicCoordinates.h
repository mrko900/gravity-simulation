#pragma once

namespace mrko900::gravity::physics {
    class DynamicCoordinates {
    public:
        virtual ~DynamicCoordinates();
        virtual float getCoordinate(unsigned int n) const = 0;
        virtual void setCoordinate(unsigned int n, float coordinate) = 0;
        virtual void removeCoordinate(unsigned int n) = 0;
        virtual unsigned int dimensions() const = 0;
        virtual void pack(float* ptr) const = 0;
    };
}
