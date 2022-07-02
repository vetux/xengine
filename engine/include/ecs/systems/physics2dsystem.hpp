/**
 *  xEngine - C++ game engine library
 *  Copyright (C) 2021  Julian Zampiccoli
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

#ifndef XENGINE_PHYSICSSYSTEM3D_HPP
#define XENGINE_PHYSICSSYSTEM3D_HPP

#include "ecs/system.hpp"

#include "physics/2d/world2d.hpp"

namespace xng {
    class XENGINE_EXPORT Physics2DSystem : public System {
    public:
        explicit Physics2DSystem(World2D &world);

        ~Physics2DSystem() override = default;

        void start(EntityContainer &entityManager) override;

        void stop(EntityContainer &entityManager) override;

        void update(float deltaTime, EntityContainer &entityManager) override;

    private:
        World2D *world;
    };
}

#endif //XENGINE_PHYSICSSYSTEM3D_HPP
