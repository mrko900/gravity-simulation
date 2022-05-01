#include "AppearanceImpl.h"

#include <stdexcept>

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

    AppearanceImpl::AppearanceImpl(const AppearanceImpl& other) {
        for (const auto& entry : other.m_Attributes) {
            void* ptr;
            switch (entry.first) {
                case FILL_COLOR:
                    RGBAColor otherColor = *((RGBAColor*) entry.second);
                    ptr = new RGBAColor { otherColor.r, otherColor.g, otherColor.b, otherColor.a };
                    break;
                case OUTLINE:
                    Outline otherOutline = *((Outline*) entry.second);
                    ptr = new Outline { otherOutline.color, otherOutline.width, otherOutline.mode };
                    break;
                case TEXTURE:
                    ptr = entry.second;
                    break;
            }
            m_Attributes.insert({ entry.first, ptr });
        }
    }

    //AppearanceImpl::AppearanceImpl(AppearanceImpl&& other) noexcept {
    //    std::cout << "move" << '\n';
    //    std::cout << other.getFillColor().r << '\n';
    //    *this = std::move(other);
    //    std::cout << getFillColor().r << '\n';
    //    std::cout << "end move\n";
    //}

    AppearanceImpl& AppearanceImpl::operator=(const AppearanceImpl& other) {
        //for (const auto& entry : other.m_Attributes) {
        //    void* ptr;
        //    switch (entry.first) {
        //        case FILL_COLOR:
        //            RGBAColor otherColor = *((RGBAColor*) entry.second);
        //            ptr = new RGBAColor { otherColor.r, otherColor.g, otherColor.b, otherColor.a };
        //            break;
        //        case OUTLINE:
        //            Outline otherOutline = *((Outline*) entry.second);
        //            ptr = new Outline { otherOutline.color, otherOutline.width, otherOutline.mode };
        //            break;
        //        case TEXTURE:
        //            ptr = entry.second;
        //            break;
        //    }
        //    m_Attributes.insert({ entry.first, ptr });
        //}
        return *this;
    }

    //AppearanceImpl& AppearanceImpl::operator=(AppearanceImpl&& other) noexcept {
    //    for (const auto& entry : other.m_Attributes)
    //        m_Attributes.insert({ entry.first, entry.second });
    //    other.m_Attributes.clear();
    //    return *this;
    //}

    AppearanceImpl::~AppearanceImpl() {
        for (const auto& entry : m_Attributes) {
            if (entry.first == FILL_COLOR)
                delete (RGBAColor*) entry.second;
            else if (entry.first == OUTLINE)
                delete (Outline*) entry.second;
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
