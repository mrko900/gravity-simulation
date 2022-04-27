#include "ProgramLoop.h"
#include <iostream>
#include "../GL/GLMacros.h"
#include "../Graphics/Texture.h"

#undef VARNAME_GL_FUNCTIONS
#define VARNAME_GL_FUNCTIONS m_GLHelper

#define weightX(x) (m_CoordinateSystemHelper.weighted(0, x))
#define weightY(y) (m_CoordinateSystemHelper.weighted(1, y))

using mrko900::gravity::gl::GLHelper;
using mrko900::gravity::graphics::Circle;
using mrko900::gravity::graphics::Appearance;

using namespace mrko900::gravity::graphics; // todo remove

using enum mrko900::gravity::app::UserInput;
using enum mrko900::gravity::app::KeyboardInputData;

namespace mrko900::gravity::app {
    ProgramLoop::ProgramLoop(mrko900::gravity::graphics::Renderer& renderer, 
                             CoordinateSystemHelper& coordinateSystemHelper) : m_Renderer(renderer),
        m_CoordinateSystemHelper(coordinateSystemHelper), m_ViewportUpdateRequested(false),
        m_ViewportNewWidth(0), m_ViewportNewHeight(0), m_PlayButton(nullptr) {
    }

    ProgramLoop::~ProgramLoop() {
        if (m_PlayButton != nullptr)
            delete m_PlayButton;
    }

    void ProgramLoop::init() {
        constexpr unsigned int size = 64;
        float* textureData = new float[sizeof(float) * size * size];
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                textureData[(y * size + x) * sizeof(float)] = (float) ((x & y) & 0xFF) / (float) (size - 1);
                textureData[(y * size + x) * sizeof(float) + 1] = (float) ((x | y) & 0xFF) / (float) (size - 1);
                textureData[(y * size + x) * sizeof(float) + 2] = (float) ((x ^ y) & 0xFF) / (float) (size - 1);
                textureData[(y * size + x) * sizeof(float) + 3] = 1.0f;
            }
        }

        class TextureClass : public Texture {
        public:
            float* textureData;
            unsigned int size;
            TextureClass(float* textureData, unsigned int size) : textureData(textureData), size(size) {
            }
            void writeLevel(unsigned int level, TextureBuffer& buffer) override {
                buffer.alloc(1, size, size);
                buffer.write(0, 0, 0, size, size, textureData);
                delete[] textureData;
            }
            unsigned int levels() override {
                return 8;
            }
        };
        Texture* texture = new TextureClass(textureData, size);

        m_PlayButton = new Circle { weightX(1.0f - weightY(0.1f)), weightY(-0.9f), weightY(0.08f),
            Appearance(AppearanceType::TEXTURE, texture), 3 };

        m_Renderer.addCircle(3, *m_PlayButton);

        PlainColor* plainColor = new PlainColor { 0.0f, 0.0f, 1.0f, 1.0f }; // mem leak
        Circle* circle = new Circle { 0.0f, 0.0f, 0.5f, // mem leak
            Appearance(AppearanceType::PLAIN_COLOR, plainColor), 0 };
        m_Renderer.addCircle(-1, *circle);
    }

    void ProgramLoop::run() {
        if (m_ViewportUpdateRequested) {
            m_Renderer.viewport(m_ViewportNewWidth, m_ViewportNewHeight);
            m_ViewportUpdateRequested = false;

            if (m_PlayButton != nullptr) {
                m_PlayButton->x = weightX(1.0f - weightY(0.1f));
                m_PlayButton->y = weightY(-0.9f);
                m_PlayButton->radius = weightY(0.08f);
            }

            // refresh figures manually
            m_Renderer.refreshFigure(3);
            m_Renderer.refreshFigure(-1);
        }

        m_Renderer.render();
    }

    void ProgramLoop::updateViewport(unsigned short newWidth, unsigned short newHeight) {
        m_ViewportUpdateRequested = true;
        m_ViewportNewWidth = newWidth;
        m_ViewportNewHeight = newHeight;
    }

    void ProgramLoop::userInput(UserInput input, void* data) {
    }
}
