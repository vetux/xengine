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

#include "ecs/ecs.hpp"

#include <utility>
#include <algorithm>

namespace xng {
    ECS::ECS(std::vector<std::reference_wrapper<System>> systems)
            : systems(std::move(systems)) {}

    ECS::~ECS() = default;

    ECS::ECS(ECS &&other) noexcept = default;

    ECS &ECS::operator=(ECS &&other) noexcept = default;

    void ECS::start() {
        for (auto &system: systems) {
            system.get().start(entityContainer);
        }
    }

    void ECS::update(float deltaTime) {
        for (auto &system: systems) {
            system.get().update(deltaTime, entityContainer);
        }
    }

    void ECS::stop() {
        for (auto &system: systems) {
            system.get().stop(entityContainer);
        }
    }

    Entity ECS::createEntity() {
        return Entity(entityContainer.create(), entityContainer);
    }

    Entity ECS::createEntity(const std::string &name) {
        return Entity(entityContainer.create(name), entityContainer);
    }

    void ECS::destroyEntity(const Entity &entity) {
        entityContainer.destroy(entity.getHandle());
    }

    Entity ECS::getEntity(const std::string &name) {
        return Entity(entityContainer.getByName(name), entityContainer);
    }

    const EntityContainer &ECS::getEntityContainer() const {
        return entityContainer;
    }

    void ECS::setEntityContainer(const EntityContainer &container) {
        entityContainer = container;
    }

    const std::vector<std::reference_wrapper<System>> &ECS::getSystems() const {
        return systems;
    }

    void ECS::setSystems(const std::vector<std::reference_wrapper<System>> &v) {
        stop();
        systems = v;
    }
}