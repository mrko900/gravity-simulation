#include "RendererBase.h"

namespace mrko900::gravity::graphics {
    RendererBase::~RendererBase() = default;

    void RendererBase::addCircle(Circle& circle) {
        m_Circles.push_back(&circle);
    }

    void RendererBase::removeCircle(Circle& circle) {
        for (int i = 0; i < m_Circles.size(); ++i) {
            if (*m_Circles[i] == circle) {
                m_Circles.erase(m_Circles.begin() + i);
                break;
            }
        }
    }

    int RendererBase::lenCircles() {
        return m_Circles.size();
    }

    Circle* RendererBase::circles() {
        if (m_Circles.size() == 1)
            return nullptr;
        return m_Circles[0];
    }
}
