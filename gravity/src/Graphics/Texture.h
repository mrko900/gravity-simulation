#pragma once

namespace mrko900::gravity::graphics {
    class TextureBuffer {
    public:
        virtual ~TextureBuffer();
        virtual void alloc(unsigned int levels, unsigned int width, unsigned int height) = 0;
        virtual void write(unsigned int level, unsigned int x, unsigned int y, unsigned int width, unsigned int height, 
                           const float* pixels) = 0;
    };

    class Texture {
    public:
        virtual ~Texture();
        virtual void writeLevel(unsigned int level, TextureBuffer& buffer) = 0;
        virtual unsigned int levels() = 0;
    };
}
