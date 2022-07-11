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

#ifndef XENGINE_RIGANIMATION_HPP
#define XENGINE_RIGANIMATION_HPP

#include <string>
#include <map>

#include "animation/skeletal/boneanimation.hpp"

namespace xng {
    struct XENGINE_EXPORT RigAnimation {
        std::string name; // The name of this animation
        double duration; // Total duration in ticks
        double ticksPerSecond; // The number of ticks to show per second
        std::map<std::string, BoneAnimation> channels; // The animation objects for each bone identified by the name of the bone.
    };
}

#endif //XENGINE_RIGANIMATION_HPP