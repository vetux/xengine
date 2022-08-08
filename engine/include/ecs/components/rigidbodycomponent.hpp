/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2021  Julian Zampiccoli
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

#ifndef XENGINE_RIGIDBODYCOMPONENT_HPP
#define XENGINE_RIGIDBODYCOMPONENT_HPP

#include "physics/rigidbody.hpp"
#include "io/messageable.hpp"

namespace xng {
    struct XENGINE_EXPORT RigidBodyComponent : public Messageable {
        bool syncTransform = true; // If true the simulated position and rotation values are applied to the TransformComponent
        RigidBody::RigidBodyType type;
        std::vector<ColliderDesc> colliders;

        Vec3f force = Vec3f();
        Vec3f forcePoint = Vec3f();
        Vec3f torque = Vec3f();

        RigidBody::RigidBodyType convert(const std::string &text) const {
            if (text == "static")
                return RigidBody::STATIC;
            else if (text == "kinematic")
                return RigidBody::KINEMATIC;
            else
                return RigidBody::DYNAMIC;
        }

        std::string convert(RigidBody::RigidBodyType type) const {
            switch (type) {
                case RigidBody::STATIC:
                    return "static";
                case RigidBody::KINEMATIC:
                    return "kinematic";
                case RigidBody::DYNAMIC:
                default:
                    return "dynamic";
            }
        }

        Messageable &operator<<(const Message &message) override {
            syncTransform = message.value("syncTransform", true);
            type = convert(message.value("type", std::string("static")));
            auto list = message.value("colliders");
            if (list.getType() == Message::LIST) {
                for (auto &col: list.asList()) {
                    ColliderDesc desc;
                    desc << col;
                    colliders.emplace_back(desc);
                }
            }
            return *this;
        }

        Message &operator>>(Message &message) const override {
            message["syncTransform"] = syncTransform;
            message["type"] = convert(type);
            auto vec = std::vector<Message>();
            for (auto &col: colliders) {
                Message msg;
                col >> msg;
                vec.emplace_back(msg);
            }
            message["colliders"] = vec;
            return message;
        }
    };
}

#endif //XENGINE_RIGIDBODYCOMPONENT_HPP