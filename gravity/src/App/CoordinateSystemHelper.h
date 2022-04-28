#pragma once

namespace mrko900::gravity::app {
    class CoordinateSystemHelper {
    public:
        virtual ~CoordinateSystemHelper();
        virtual float weighted(unsigned int coordinate, float normalized) = 0;
        //virtual float normalized(unsigned int coordinate, float weighted) = 0; // todo ?
    };
}
