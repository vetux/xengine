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

#ifndef XENGINE_VERTEXARRAYBUFFERDESC_HPP
#define XENGINE_VERTEXARRAYBUFFERDESC_HPP

#include <vector>

#include "xng/gpu/vertexattribute.hpp"
#include "xng/asset/primitive.hpp"

namespace xng {
    struct VertexArrayBufferDesc {
        Primitive primitive = TRI;
        std::vector<VertexAttribute> vertexLayout{}; // The layout of one vertex in the vertex buffer
        std::vector<VertexAttribute> instanceArrayLayout{}; // The layout of one entry in the instance buffer

        bool operator==(const VertexArrayBufferDesc &other) const {
            return primitive == other.primitive
                   && vertexLayout == other.vertexLayout
                   && instanceArrayLayout == other.instanceArrayLayout;
        }
    };
}

#endif //XENGINE_VERTEXARRAYBUFFERDESC_HPP
