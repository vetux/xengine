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

#ifndef XENGINE_COLLIDERDEF_HPP
#define XENGINE_COLLIDERDEF_HPP

#include "io/messageable.hpp"
#include "physics/collidershape.hpp"

namespace xng {
    struct ColliderDesc : public Messageable {
        ColliderShape shape;
        float friction{};
        float restitution{};
        float restitution_threshold{};
        float density{};
        bool isSensor{};

        bool operator==(const ColliderDesc &other) const {
            return shape == other.shape
                   && friction == other.friction
                   && restitution == other.restitution
                   && restitution_threshold == other.restitution_threshold
                   && density == other.density
                   && isSensor == other.isSensor;
        }

        Messageable &operator<<(const Message &message) override {
            shape << message.value("shape");
            friction = message.value("friction", 1.0f);
            restitution = message.value("restitution", 0.0f);
            restitution_threshold = message.value("restitution_threshold", 0.0f);
            density = message.value("density", 1.0f);
            isSensor = message.value("isSensor", false);
            return *this;
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);
            shape >> message["shape"];
            message["friction"] = friction;
            message["restitution"] = restitution;
            message["restitution_threshold"] = restitution_threshold;
            message["density"] = friction;
            message["isSensor"] = isSensor;
            return message;
        }
    };
}

#endif //XENGINE_COLLIDERDEF_HPP
