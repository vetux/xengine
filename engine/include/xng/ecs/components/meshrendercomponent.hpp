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

#ifndef XENGINE_MESHRENDERCOMPONENT_HPP
#define XENGINE_MESHRENDERCOMPONENT_HPP

#include "xng/asset/material.hpp"
#include "xng/asset/mesh.hpp"
#include "xng/resource/resourcehandle.hpp"
#include "xng/io/messageable.hpp"
#include "xng/ecs/component.hpp"

namespace xng {
    struct XENGINE_EXPORT MeshRenderComponent : public Component {
        bool castShadows{};
        bool receiveShadows{};

        ResourceHandle<Mesh> mesh;
        ResourceHandle<Material> material{};

        bool operator==(const MeshRenderComponent &other) const {
            return enabled == other.enabled
                   && castShadows == other.castShadows
                   && receiveShadows == other.receiveShadows
                   && mesh == other.mesh
                   && material == other.material;
        }

        Messageable &operator<<(const Message &message) override {
            message.value("castShadows", castShadows);
            message.value("receiveShadows", receiveShadows);
            mesh << message.getMessage("mesh");
            material << message.getMessage("material");
            return Component::operator<<(message);
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);
            message["castShadows"] = castShadows;
            message["receiveShadows"] = receiveShadows;
            mesh >> message["mesh"];
            material >> message["material"];
            return Component::operator>>(message);
        }

        std::type_index getType() const override {
            return typeid(MeshRenderComponent);
        }
    };
}

#endif //XENGINE_MESHRENDERCOMPONENT_HPP
