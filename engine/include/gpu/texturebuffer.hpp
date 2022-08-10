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

#ifndef XENGINE_TEXTUREBUFFER_HPP
#define XENGINE_TEXTUREBUFFER_HPP

#include <memory>

#include "gpu/renderobject.hpp"
#include "gpu/textureproperties.hpp"
#include "gpu/texturebufferdesc.hpp"
#include "gpu/fence.hpp"

#include "asset/image.hpp"

namespace xng {
    /**
     * A texture buffer.
     */
    class XENGINE_EXPORT TextureBuffer : public RenderObject {
    public:
        ~TextureBuffer() override = default;

        virtual const TextureBufferDesc &getDescription() = 0;

        Type getType() override {
            return TEXTURE_BUFFER;
        }

        /**
         * Upload the image buffer.
         *
         * The size and format of the buffer has to match the texture description.
         *
         * @param buffer
         */
        virtual std::unique_ptr<Fence> upload(ColorFormat format, const uint8_t *buffer, size_t bufferSize) = 0;

        virtual Image<ColorRGBA> download() = 0;

        virtual std::unique_ptr<Fence> upload(CubeMapFace face, ColorFormat format, const uint8_t *buffer, size_t bufferSize) = 0;

        virtual Image<ColorRGBA> download(CubeMapFace face) = 0;

        std::unique_ptr<Fence> upload(const Image<ColorRGBA> &image) {
            return upload(RGBA, reinterpret_cast<const uint8_t *>(image.getData()), image.getDataSize() * sizeof(ColorRGBA));
        }
    };
}

#endif //XENGINE_TEXTUREBUFFER_HPP