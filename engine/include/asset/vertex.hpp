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

#ifndef XENGINE_VERTEX_HPP
#define XENGINE_VERTEX_HPP

#include "math/vector3.hpp"
#include "math/vector4.hpp"

namespace xng {
    struct XENGINE_EXPORT Vertex {
        float data[22];

        Vertex(Vec3f position, Vec3f normal, Vec2f uv, Vec3f tangent, Vec3f bitangent, Vec4i boneIds, Vec4f boneWeights)
                : data() {
            data[0] = position.x;
            data[1] = position.y;
            data[2] = position.z;
            data[3] = normal.x;
            data[4] = normal.y;
            data[5] = normal.z;
            data[6] = uv.x;
            data[7] = uv.y;
            data[8] = tangent.x;
            data[9] = tangent.y;
            data[10] = tangent.z;
            data[11] = bitangent.x;
            data[12] = bitangent.y;
            data[13] = bitangent.z;
            // Hack: Cast data values to int references for id assignment which should work as long as sizeof(float) == sizeof(int)
            static_assert(sizeof(float) == sizeof(int));
            reinterpret_cast<int &>(data[14]) = boneIds.x;
            reinterpret_cast<int &>(data[15]) = boneIds.y;
            reinterpret_cast<int &>(data[16]) = boneIds.z;
            reinterpret_cast<int &>(data[17]) = boneIds.w;
            data[18] = boneWeights.x;
            data[19] = boneWeights.y;
            data[20] = boneWeights.z;
            data[21] = boneWeights.w;
        }

        Vertex(Vec3f position, Vec3f normal, Vec2f uv, Vec3f tangent, Vec3f bitangent)
                : Vertex(position, normal, uv, tangent, bitangent, {}, {}) {}

        Vertex(Vec3f position, Vec2f uv) : Vertex(position, {}, uv, {}, {}, {}, {}) {}

        explicit Vertex(Vec3f position) : Vertex(position, {}, {}, {}, {}, {}, {}) {}

        Vertex() = default;

        Vec3f position() const {
            return {data[0], data[1], data[2]};
        }

        Vec3f normal() const {
            return {data[3], data[4], data[5]};
        }

        Vec2f uv() const {
            return {data[6], data[7]};
        }

        Vec3f tangent() const {
            return {data[8], data[9], data[10]};
        }

        Vec3f bitangent() const {
            return {data[11], data[12], data[13]};
        }

        Vec4i boneIds() const {
            return {reinterpret_cast<const int &>(data[14]),
                    reinterpret_cast<const int &>(data[15]),
                    reinterpret_cast<const int &>(data[16]),
                    reinterpret_cast<const int &>(data[17])};
        }

        Vec4f boneWeights() const {
            return {data[18], data[19], data[20], data[21]};
        }
    };
}

#endif //XENGINE_VERTEX_HPP
