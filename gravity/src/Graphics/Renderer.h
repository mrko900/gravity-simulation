#pragma once

namespace mrko900::gravity::graphics {
    struct Appearance {
        float r, g, b, a;
        bool operator==(const Appearance& other) {
            return r == other.r
                && g == other.g
                && b == other.b
                && a == other.a;
        }
    };

    struct Circle {
        float x, y, radius;
        Appearance appearance;
        bool operator==(const Circle& other) {
            return appearance == other.appearance
                && x == other.x
                && y == other.y
                && radius == other.radius;
        }
    };

    class Renderer {
    public:
        virtual ~Renderer() noexcept;
        virtual void addCircle(Circle circle) = 0;
        virtual void removeCircle(Circle circle) = 0;
        virtual void render() = 0;
        virtual void viewport(unsigned short viewportWidth, unsigned short viewportHeight) = 0;
    };
}
