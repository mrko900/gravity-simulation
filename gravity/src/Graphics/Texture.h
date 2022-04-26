#pragma once

namespace mrko900::gravity::graphics {
    class TextureBuffer {
    public:
        virtual ~TextureBuffer();
        virtual void alloc(int size) = 0;
        virtual void write(int x, int y, int width, int height, const float* pixels) = 0;
    };

    class Texture {
    public:
        virtual ~Texture();
        virtual void writeLevel(int level, const TextureBuffer& buffer) = 0;
        virtual int levels() = 0;
    };
}
