#pragma once

#include "Appearance.h"
#include <unordered_map>

namespace mrko900::gravity::graphics {
    class AppearanceImpl : public Appearance {
    public:
        AppearanceImpl(AppearanceAttribute* attributes, unsigned int nAttributes);
        AppearanceImpl(const AppearanceImpl& other);
        AppearanceImpl(AppearanceImpl&& other) noexcept = delete;
        ~AppearanceImpl();
        AppearanceImpl& operator=(const AppearanceImpl& other);
        AppearanceImpl& operator=(AppearanceImpl&& other) noexcept = delete;
        bool hasAttribute(AppearanceAttribute attribute) const override;
        void* getAttribute(AppearanceAttribute attribute) override;
        void*& getAttributePointer(AppearanceAttribute attribute);

    private:
        std::unordered_map<AppearanceAttribute, void*> m_Attributes;
    };
}
