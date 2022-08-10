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

#ifndef XENGINE_VERTEXBUFFER_HPP
#define XENGINE_VERTEXBUFFER_HPP

#include "gpu/renderobject.hpp"
#include "gpu/vertexbufferdesc.hpp"
#include "gpu/fence.hpp"

#include "asset/mesh.hpp"

#include "math/transform.hpp"

namespace xng {
    class XENGINE_EXPORT VertexBuffer : public RenderObject {
    public:
        ~VertexBuffer() override = default;

        virtual const VertexBufferDesc &getDescription() = 0;

        Type getType() override {
            return Type::VERTEX_BUFFER;
        }

        /**
         * Upload the data from the specified buffers to the mesh buffer,
         * the layout of the passed buffers must match the layout specified in the mesh buffer description.
         *
         * @param vertexBuffer
         * @param instanceBuffer
         * @param indices
         */
        virtual std::unique_ptr<Fence> upload(const uint8_t *vertexBuffer,
                                              size_t vertexBufferSize,
                                              const uint8_t *instanceBuffer,
                                              size_t instanceBufferSize,
                                              const std::vector<uint> &indices) = 0;

        virtual std::unique_ptr<Fence> upload(const Mesh &mesh) {
            return upload(reinterpret_cast<const uint8_t *>(mesh.vertices.data()),
                          sizeof(Vertex) * mesh.vertices.size(),
                          nullptr,
                          0,
                          mesh.indices);
        }

        virtual std::unique_ptr<Fence> upload(const Mesh &mesh,
                                              const std::vector<Mat4f> &offsets) {
            return upload(reinterpret_cast<const uint8_t *>(mesh.vertices.data()),
                          sizeof(Vertex) * mesh.vertices.size(),
                          reinterpret_cast<const uint8_t *>(offsets.data()),
                          sizeof(Mat4f) * offsets.size(),
                          mesh.indices);
        }
    };
}

#endif //XENGINE_VERTEXBUFFER_HPP
