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

#include "xng/render/textureatlas.hpp"

namespace xng {
    TextureAtlas::TextureAtlas(
            std::map<TextureAtlasResolution, std::reference_wrapper<TextureArrayBuffer>> atlasBuffers,
            std::map<TextureAtlasResolution, std::vector<bool>> bufferOccupations)
            : atlasBuffers(std::move(atlasBuffers)),
              bufferOccupations(std::move(bufferOccupations)) {}

    void TextureAtlas::upload(TextureAtlasResolution res, size_t index, const ImageRGBA &texture) {
        atlasBuffers.at(res).get().upload(index, getAlignedImage(texture, res));
    }

    TextureAtlasHandle TextureAtlas::add(const ImageRGBA &texture) {
        auto res = getClosestMatchingResolutionLevel(texture.getSize());
        for (size_t i = 0; i < bufferOccupations.at(res).size(); i++) {
            if (!bufferOccupations.at(res).at(i)) {
                bufferOccupations.at(res).at(i) = !bufferOccupations.at(res).at(i);
                upload(res, i, texture);
                TextureAtlasHandle ret;
                ret.index = i;
                ret.level = res;
                ret.size = texture.getSize();
                return ret;
            }
        }
        throw std::runtime_error("No free slot in atlas texture array buffers.");
    }

    void TextureAtlas::remove(const TextureAtlasHandle &handle) {
        bufferOccupations.at(handle.level).at(handle.index) = !bufferOccupations.at(handle.level).at(handle.index);
    }

    ImageRGBA TextureAtlas::getAlignedImage(const ImageRGBA &texture, TextureAtlasResolution res) {
        auto size = getResolutionLevelSize(res);
        ImageRGBA image(size);
        image.blit(Vec2i(0, 0), texture);
        return image;
    }
}