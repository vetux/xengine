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

#ifndef XENGINE_JOINTBOX2D_HPP
#define XENGINE_JOINTBOX2D_HPP

#include <Box2D/Dynamics/b2Body.h>

#include "physics/2d/joint2d.hpp"

namespace xng {
    class JointBox2D : public Joint2D {
    public:
        b2Joint *joint;

        ~JointBox2D() override = default;
    };
}

#endif //XENGINE_JOINTBOX2D_HPP
