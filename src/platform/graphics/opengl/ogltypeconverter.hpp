/**
 *  Mana - 3D Game Engine
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

#ifndef MANA_OGLTYPECONVERTER_HPP
#define MANA_OGLTYPECONVERTER_HPP

#include "platform/graphics/rendercommand.hpp"

#include "openglinclude.hpp"

namespace engine {
    namespace opengl {
        namespace OGLTypeConverter {
            GLenum getColorAttachment(int index);

            GLint convertPrimitive(int value);

            GLuint convertPrimitive(uint value);

            GLenum convert(FaceCullingMode mode);

            GLenum convert(const DepthTestMode &mode);

            GLenum convert(BlendMode mode);

            GLenum convert(StencilMode mode);

            GLenum convert(StencilAction action);

            GLenum convert(TextureBuffer::TextureType type);

            GLenum convert(TextureBuffer::CubeMapFace face);

            GLenum convert(TextureBuffer::ColorFormat format);

            GLint convert(TextureBuffer::TextureWrapping wrapping);

            GLint convert(TextureBuffer::TextureFiltering filtering);

            GLint convert(TextureBuffer::MipMapFiltering filtering);
        }
    }
}

#endif //MANA_OGLTYPECONVERTER_HPP
