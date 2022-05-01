#pragma once

#include "Appearance.h"

namespace mrko900::gravity::graphics {
    struct Circle {
        float x, y, radius;
        Appearance* appearance;
        int layer;
    };

    struct Rectangle {
        float x, y, width, height;
        Appearance* appearance;
        int layer;
    };

    enum class FigureType {
        CIRCLE, RECTANGLE
    };

    class Renderer {
    public:
        virtual ~Renderer();
        virtual void addCircle(unsigned int id, Circle& circle) = 0;
        virtual void addRectangle(unsigned int id, Rectangle& rectangle) = 0;
        virtual void removeFigure(unsigned int id) = 0;
        virtual void refreshFigure(unsigned int id) = 0;
        virtual void render() = 0;
        virtual void viewport(unsigned short viewportWidth, unsigned short viewportHeight) = 0;
        virtual void coordinateSystem(float xBegin, float xEnd, float yBegin, float yEnd) = 0;
    };
}
