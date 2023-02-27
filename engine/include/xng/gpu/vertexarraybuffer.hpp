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

#ifndef XENGINE_VERTEXARRAYBUFFER_HPP
#define XENGINE_VERTEXARRAYBUFFER_HPP

#include "xng/gpu/renderobject.hpp"

#include "xng/gpu/vertexbuffer.hpp"
#include "xng/gpu/indexbuffer.hpp"
#include "xng/gpu/vertexarraybufferdesc.hpp"

namespace xng {
    /**
     * A array buffer accesses vertex data from a set of buffers and uses a layout to specify
     * how the buffers should be used to generate the vertex attributes for the vertex shader.
     *
     * On OpenGL this would represent a vertex array (glBindVertexArray) with the corresponding vertex attribute pointer setup.
     */
    class VertexArrayBuffer : public RenderObject {
    public:
        Type getType() override {
            return VERTEX_ARRAY_BUFFER;
        }

        virtual const VertexArrayBufferDesc &getDescription() = 0;

        virtual VertexBuffer *getVertexBuffer() = 0;

        /**
         * Optional index buffer for specifying indices into the vertex buffer for elements instead of reading
         * the elements directly from the vertex buffer.
         * @return
         */
        virtual IndexBuffer *getIndexBuffer() = 0;

        /**
         * The instance buffer is vertex attribute data that is updated for each instance instead of each vertex
         * by the graphics driver and accessed using the instanceArrayLayout specified in the mesh buffer layout.
         *
         * Shader attribute bindings can access the instance attributes by using index vertexLayout.size
         *
         * @return
         */
        virtual VertexBuffer *getInstanceBuffer() = 0;

        virtual std::unique_ptr<GpuFence> setBuffers(VertexBuffer &vertexBuffer) = 0;

        virtual std::unique_ptr<GpuFence> setBuffers(VertexBuffer &vertexBuffer, IndexBuffer &indexBuffer) = 0;

        virtual std::unique_ptr<GpuFence> setBuffers(VertexBuffer &vertexBuffer,
                                                     IndexBuffer &indexBuffer,
                                                     VertexBuffer &instanceBuffer) = 0;
    };
}

#endif //XENGINE_VERTEXARRAYBUFFER_HPP
