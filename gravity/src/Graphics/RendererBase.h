#pragma once

#include "Renderer.h"
#include <vector>

namespace mrko900::gravity::graphics {
    class RendererBase : public Renderer {
    public:
        virtual ~RendererBase() noexcept;
        void addCircle(Circle& circle) override;
        void removeCircle(Circle& circle) override;

    protected:
        int lenCircles();
        Circle* circles();

    private:
        std::vector<Circle*> m_Circles;
    };
}
