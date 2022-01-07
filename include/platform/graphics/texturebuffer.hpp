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

#ifndef MANA_TEXTUREBUFFER_HPP
#define MANA_TEXTUREBUFFER_HPP

#include "asset/image.hpp"
#include "renderobject.hpp"

namespace engine {
    /**
     * A texture buffer.
     * The texture type, size and format is changed when calling the upload methods.
     */
    class MANA_EXPORT TextureBuffer : public RenderObject {
    public:
        enum TextureType {
            TEXTURE_2D,
            TEXTURE_2D_MULTISAMPLE,
            TEXTURE_CUBE_MAP
        };

        enum CubeMapFace {
            POSITIVE_X,
            NEGATIVE_X,
            POSITIVE_Y,
            NEGATIVE_Y,
            POSITIVE_Z,
            NEGATIVE_Z
        };

        enum ColorFormat {
            //Base Formats
            DEPTH,
            DEPTH_STENCIL,
            R,
            RG,
            RGB,
            RGBA,

            //Compressed Formats
            R_COMPRESSED,
            RG_COMPRESSED,
            RGB_COMPRESSED,
            RGBA_COMPRESSED,

            //Sized normalized float
            R8,
            RG8,
            RGB8,
            RGBA8,

            R16,
            RG16,
            RGB16,
            RGBA16,

            RGB12,
            RGBA12,
            RGB10,

            //Sized float
            R16F,
            RG16F,
            RGB16F,
            RGBA16F,

            R32F,
            RG32F,
            RGB32F,
            RGBA32F,

            //Sized integer
            R8I,
            RG8I,
            RGB8I,
            RGBA8I,

            R16I,
            RG16I,
            RGB16I,
            RGBA16I,

            R32I,
            RG32I,
            RGB32I,
            RGBA32I,

            //Sized unsigned integer
            R8UI,
            RG8UI,
            RGB8UI,
            RGBA8UI,

            R16UI,
            RG16UI,
            RGB16UI,
            RGBA16UI,

            R32UI,
            RG32UI,
            RGB32UI,
            RGBA32UI,
        };

        enum TextureWrapping {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER
        };

        enum TextureFiltering {
            NEAREST,
            LINEAR
        };

        enum MipMapFiltering {
            NEAREST_MIPMAP_NEAREST,
            LINEAR_MIPMAP_NEAREST,
            NEAREST_MIPMAP_LINEAR,
            LINEAR_MIPMAP_LINEAR
        };

        struct MANA_EXPORT Attributes {
            TextureType textureType = TEXTURE_2D;
            Vec2i size = {};
            ColorFormat format = RGBA;
            TextureWrapping wrapping = CLAMP_TO_EDGE;
            TextureFiltering filterMin = LINEAR;
            TextureFiltering filterMag = LINEAR;
            bool generateMipmap = true;
            MipMapFiltering mipmapFilter = NEAREST_MIPMAP_NEAREST;
            int samples = 1;
        };

        explicit TextureBuffer(Attributes attributes) : attributes(attributes) {}

        ~TextureBuffer() override = default;

        const Attributes &getAttributes() const { return attributes; }

        virtual void upload(const Image <ColorRGB> &buffer) = 0;

        virtual void upload(const Image <ColorRGBA> &buffer) = 0;

        virtual void upload(const Image<float> &buffer) = 0;

        virtual void upload(const Image<int> &buffer) = 0;

        virtual void upload(const Image<char> &buffer) = 0;

        virtual void upload(const Image<unsigned char> &buffer) = 0;

        virtual Image <ColorRGBA> download() = 0;

        virtual void upload(CubeMapFace face, const Image <ColorRGBA> &buffer) = 0;

        virtual Image <ColorRGBA> download(CubeMapFace face) = 0;

        virtual void uploadCubeMap(const Image <ColorRGBA> &buffer) = 0;

        virtual Image <ColorRGBA> downloadCubeMap() = 0;

    protected:
        Attributes attributes;
    };
}

#endif //MANA_TEXTUREBUFFER_HPP
