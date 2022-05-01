#include "AppearanceImpl.h"

#include <stdexcept>s

using enum mrko900::gravity::graphics::AppearanceAttribute;
#include <iostream>
namespace mrko900::gravity::graphics {
    AppearanceImpl::AppearanceImpl(AppearanceAttribute* attributes, unsigned int nAttributes) {
        for (int i = 0; i < nAttributes; ++i) {
            void* ptr;
            switch (attributes[i]) {
                case FILL_COLOR:
                    ptr = new RGBAColor {};
                    break;
                case OUTLINE:
                    ptr = new Outline {};
                    break;
                case TEXTURE:
                    ptr = nullptr;
                    break;
                default:
                    throw std::invalid_argument("unsupported appearance attribute");
            }
            m_Attributes.insert({ attributes[i], ptr });
        }
    }

    AppearanceImpl::~AppearanceImpl() {
        for (const auto& entry : m_Attributes) {
            if (entry.first != TEXTURE)
                delete entry.second;
        }
    }

    bool AppearanceImpl::hasAttribute(AppearanceAttribute attribute) const {
        return m_Attributes.contains(attribute);;
    }

    void* AppearanceImpl::getAttribute(AppearanceAttribute attribute) {
        return m_Attributes.at(attribute);
    }

    void*& AppearanceImpl::getAttributePointer(AppearanceAttribute attribute) {
        return m_Attributes.at(attribute);
    }
}
