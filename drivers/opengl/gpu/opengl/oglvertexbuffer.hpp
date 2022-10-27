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

#ifndef XENGINE_OGLVERTEXBUFFER_HPP
#define XENGINE_OGLVERTEXBUFFER_HPP

#include "gpu/vertexbuffer.hpp"

#include <utility>

#include "gpu/opengl/oglbuildmacro.hpp"
#include "gpu/opengl/oglfence.hpp"

#include "asset/mesh.hpp"

#include "math/matrixmath.hpp"

namespace xng::opengl {
    class OPENGL_TYPENAME(VertexBuffer) : public VertexBuffer OPENGL_INHERIT {
    public:
        VertexBufferDesc desc;

        GLuint VAO = 0;
        GLuint VBO = 0;
        GLuint EBO = 0;
        GLuint instanceVBO = 0;

        size_t elementCount = 0;
        GLuint elementType = 0;

        bool indexed = false;
        bool instanced = false;

        explicit OPENGL_TYPENAME(VertexBuffer)(VertexBufferDesc inputDescription) :
                desc(std::move(inputDescription)) {
            initialize();
            indexed = desc.numberOfIndices != 0;
            instanced = desc.numberOfInstances != 0;
            if (indexed) {
                elementCount = desc.numberOfIndices;
            } else {
                elementCount = desc.numberOfVertices;
            }
            switch (desc.primitive) {
                case POINT:
                    elementType = GL_POINTS;
                    break;
                case LINE:
                    elementType = GL_LINES;
                    break;
                case TRI:
                    elementType = GL_TRIANGLES;
                    break;
                case QUAD:
                    elementType = GL_QUADS;
                    break;
                default:
                    throw std::runtime_error("Invalid primitive");
            }
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            if (instanced) {
                glGenBuffers(1, &instanceVBO);
            }
        }

        OPENGL_TYPENAME(VertexBuffer)(const OPENGL_TYPENAME(VertexBuffer) &copy) = delete;

        OPENGL_TYPENAME(VertexBuffer) &operator=(const OPENGL_TYPENAME(VertexBuffer) &copy) = delete;

        ~OPENGL_TYPENAME(VertexBuffer)() override {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            if (instanced) {
                glDeleteBuffers(1, &instanceVBO);
            }
        }

        const VertexBufferDesc &getDescription() override {
            return desc;
        }

        std::unique_ptr<GpuFence>
        upload(const uint8_t *vertexBuffer, size_t vertexBufferSize, const std::vector<unsigned int> &indices,
               const uint8_t *instanceBuffer, size_t instanceBufferSize) override {
            int vertexSize = 0;
            for (auto &binding: desc.vertexLayout) {
                vertexSize += binding.stride();
            }

            int instanceSize = 0;
            for (auto &binding: desc.instanceLayout) {
                instanceSize += binding.stride();
            }

            int indexSize = sizeof(unsigned int);

            auto totalVerticesSize = numeric_cast<GLsizei>(vertexSize * desc.numberOfVertices);
            auto totalInstancesSize = numeric_cast<GLsizei>(instanceSize * desc.numberOfInstances);
            auto totalIndicesSize = numeric_cast<GLsizei>(indexSize * desc.numberOfIndices);

            if (vertexBufferSize != totalVerticesSize) {
                throw std::runtime_error("Invalid vertex buffer size");
            } else if (instanceBufferSize != totalInstancesSize) {
                throw std::runtime_error("Invalid instance buffer size");
            } else if (indices.size() != desc.numberOfIndices) {
                throw std::runtime_error("Invalid index buffer size");
            }

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER,
                         totalVerticesSize,
                         vertexBuffer,
                         GL_STATIC_DRAW);

            if (indexed) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             totalIndicesSize,
                             indices.data(),
                             GL_STATIC_DRAW);
            }

            for (int i = 0; i < desc.vertexLayout.size(); i++) {
                glEnableVertexAttribArray(i);
            }

            for (int i = 0; i < desc.instanceLayout.size(); i++) {
                glEnableVertexAttribArray(desc.vertexLayout.size() + i);
            }

            size_t currentOffset = 0;
            for (int i = 0; i < desc.vertexLayout.size(); i++) {
                auto &binding = desc.vertexLayout.at(i);
                glVertexAttribPointer(i,
                                      VertexAttribute::getCount(binding.type),
                                      getType(binding.component),
                                      GL_FALSE,
                                      vertexSize,
                                      (void *) (currentOffset));
                currentOffset += binding.stride();
            }

            if (instanced) {
                currentOffset = 0;

                glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
                glBufferData(GL_ARRAY_BUFFER,
                             totalInstancesSize,
                             instanceBuffer,
                             GL_STATIC_DRAW);

                for (int i = 0; i < desc.instanceLayout.size(); i++) {
                    auto &binding = desc.instanceLayout.at(i);
                    auto index = desc.vertexLayout.size() + i;
                    glVertexAttribPointer(index,
                                          VertexAttribute::getCount(binding.type),
                                          getType(binding.component),
                                          GL_FALSE,
                                          instanceSize,
                                          (void *) currentOffset);
                    currentOffset += binding.stride();
                }

                for (int i = 0; i < desc.instanceLayout.size(); i++) {
                    glVertexAttribDivisor(desc.vertexLayout.size() + i, 1);
                }
            }

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            if (indexed) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            checkGLError("OGLRenderAllocator::createMeshBuffer");

            return std::make_unique<OGLFence>();
        }

        static GLenum getType(VertexAttribute::Component c) {
            switch (c) {
                case VertexAttribute::UNSIGNED_BYTE:
                    return GL_UNSIGNED_BYTE;
                case VertexAttribute::SIGNED_BYTE:
                    return GL_BYTE;
                case VertexAttribute::UNSIGNED_INT:
                    return GL_UNSIGNED_INT;
                case VertexAttribute::SIGNED_INT:
                    return GL_INT;
                case VertexAttribute::FLOAT:
                    return GL_FLOAT;
                case VertexAttribute::DOUBLE:
                    return GL_DOUBLE;
                default:
                    throw std::runtime_error("Invalid component");
            }
        }

        std::unique_ptr<GpuFence> copy(RenderBuffer &other) override {
            throw std::runtime_error("Not Implemented");
        }

        OPENGL_MEMBERS

        OPENGL_CONVERSION_MEMBERS
    };
}

#endif //XENGINE_OGLVERTEXBUFFER_HPP
