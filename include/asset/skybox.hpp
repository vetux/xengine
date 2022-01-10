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

#ifndef XENGINE_SKYBOX_HPP
#define XENGINE_SKYBOX_HPP

#include <array>

#include "asset/assetpath.hpp"
#include "color.hpp"

namespace xengine {
    struct XENGINE_EXPORT Skybox {
        ColorRGBA color = {12, 123, 123, 255}; // If texture is unassigned skybox color is drawn
        AssetPath texture; // The cube map texture
    };
}

#endif //XENGINE_SKYBOX_HPP
