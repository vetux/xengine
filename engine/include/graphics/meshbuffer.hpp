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

#ifndef XENGINE_MESHBUFFER_HPP
#define XENGINE_MESHBUFFER_HPP

#include "graphics/renderobject.hpp"
#include "graphics/meshbufferdesc.hpp"
#include "graphics/meshbufferview.hpp"

#include "asset/mesh.hpp"

#include "math/transform.hpp"

namespace xengine {
    class XENGINE_EXPORT MeshBuffer : public RenderObject {
    public:
        ~MeshBuffer() override = default;

        virtual const MeshBufferDesc &getDescription() = 0;

        virtual std::unique_ptr<MeshBufferView> createView() = 0;

        /**
         * Upload the data from the specified buffers to the mesh buffer,
         * the layout of the passed buffers must match the layout specified in the mesh buffer description.
         *
         * @param buffer
         * @param instanceBuffer
         * @param indices
         */
        virtual void upload(const uint8_t *buffer,
                            size_t bufferSize,
                            const uint8_t *instanceBuffer,
                            size_t instanceBufferSize,
                            const std::vector<uint> &indices) = 0;

        virtual void upload(const Mesh &mesh) {
            upload(reinterpret_cast<const uint8_t *>(mesh.vertices.data()),
                   sizeof(Vertex) * mesh.vertices.size(),
                   nullptr,
                   0,
                   mesh.indices);
        }
    };
}

#endif //XENGINE_MESHBUFFER_HPP
