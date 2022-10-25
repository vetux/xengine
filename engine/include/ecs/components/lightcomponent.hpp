/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2022  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef XENGINE_LIGHTCOMPONENT_HPP
#define XENGINE_LIGHTCOMPONENT_HPP

#include "asset/light.hpp"
#include "io/messageable.hpp"

namespace xng {
    struct XENGINE_EXPORT LightComponent : public Messageable {
        bool enabled = true;

        Light light;

        Messageable &operator<<(const Message &message) override {
            enabled = message.value("enabled", true);
            light << message.value("light");
            return *this;
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);
            message["enabled"] = enabled;
            light >> message["light"];
            return message;
        }
    };
}

#endif //XENGINE_LIGHTCOMPONENT_HPP
