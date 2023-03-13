/**
 *  xEngine - C++ game engine library
 *  Copyright (C) 2023  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef XENGINE_RECTTRANSFORM_HPP
#define XENGINE_RECTTRANSFORM_HPP

#include <map>

#include "xng/math/vector2.hpp"

#include "xng/io/messageable.hpp"

namespace xng {
    struct RectTransform : public Messageable {
        enum Alignment : int {
            RECT_ALIGN_LEFT_TOP,
            RECT_ALIGN_CENTER_TOP,
            RECT_ALIGN_RIGHT_TOP,
            RECT_ALIGN_LEFT_CENTER,
            RECT_ALIGN_CENTER_CENTER,
            RECT_ALIGN_RIGHT_CENTER,
            RECT_ALIGN_LEFT_BOTTOM,
            RECT_ALIGN_CENTER_BOTTOM,
            RECT_ALIGN_RIGHT_BOTTOM
        } alignment = RECT_ALIGN_LEFT_TOP;

        Vec2f position{};
        Vec2f size{};
        Vec2f center{};
        float rotation{};

        Messageable &operator<<(const Message &message) override {
            message.value("alignment", reinterpret_cast<int&>(alignment), static_cast<int>(RECT_ALIGN_LEFT_TOP));
            message.value("position", position);
            message.value("size", size);
            message.value("center", center);
            message.value("rotation", rotation);
            return *this;
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);
            alignment >> message["alignment"];
            position >> message["position"];
            size >> message["size"];
            center >> message["center"];
            rotation >> message["rotation"];
            return message;
        }
    };
}
#endif //XENGINE_RECTTRANSFORM_HPP
