#pragma once

#include "Appearance.h"
#include <unordered_map>

namespace mrko900::gravity::graphics {
    class AppearanceImpl : public Appearance {
    public:
        AppearanceImpl(AppearanceAttribute* attributes, unsigned int nAttributes);
        ~AppearanceImpl();
        bool hasAttribute(AppearanceAttribute attribute) const override;
        void* getAttribute(AppearanceAttribute attribute) override;
        void*& getAttributePointer(AppearanceAttribute attribute);

    private:
        std::unordered_map<AppearanceAttribute, void*> m_Attributes;
    };
}
