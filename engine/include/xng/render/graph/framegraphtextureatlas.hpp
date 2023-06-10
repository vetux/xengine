/**
 *  xEngine - C++ Game Engine Library
 *  Copyright (C) 2023  Julian Zampiccoli
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

#ifndef XENGINE_FRAMEGRAPHTEXTUREATLAS_HPP
#define XENGINE_FRAMEGRAPHTEXTUREATLAS_HPP

#include "xng/render/textureatlas.hpp"

#include "xng/render/graph/framegraphbuilder.hpp"

namespace xng {
    class FrameGraphTextureAtlas {
    public:
        FrameGraphTextureAtlas() = default;

        TextureAtlasHandle add(const ImageRGBA &texture) {
            auto level = TextureAtlas::getClosestMatchingResolutionLevel(texture.getSize());
            if (atlas.getFreeSlotCount(level) == 0) {
                auto &buf = atlas.getBufferOccupations()[level];
                buf.resize(buf.size() + 1);
            }
            auto ret = atlas.add(texture);
            pendingTextures[ret] = texture;
            return ret;
        }

        void remove(const TextureAtlasHandle &handle) {
            atlas.remove(handle);
        }

        void setup(FrameGraphBuilder &builder) {
            previousHandles = currentHandles;

            if (!currentHandles.tex8x8.assigned) {
                TextureArrayBufferDesc desc;
                desc.textureDesc.generateMipmap = true;
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_8x8].size();
                desc.textureDesc.size = {8, 8};
                currentHandles.tex8x8 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_16x16].size();
                desc.textureDesc.size = {16, 16};
                currentHandles.tex16x16 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_32x32].size();
                desc.textureDesc.size = {32, 32};
                currentHandles.tex32x32 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_64x64].size();
                desc.textureDesc.size = {64, 64};
                currentHandles.tex64x64 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_128x128].size();
                desc.textureDesc.size = {128, 128};
                currentHandles.tex128x128 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_256x256].size();
                desc.textureDesc.size = {256, 256};
                currentHandles.tex256x256 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_512x512].size();
                desc.textureDesc.size = {512, 512};
                currentHandles.tex512x512 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_1024x1024].size();
                desc.textureDesc.size = {1024, 1024};
                currentHandles.tex1024x1024 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_2048x2048].size();
                desc.textureDesc.size = {2048, 2048};
                currentHandles.tex2048x2048 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_4096x4096].size();
                desc.textureDesc.size = {4096, 4096};
                currentHandles.tex4096x4096 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_8192x8192].size();
                desc.textureDesc.size = {8192, 8192};
                currentHandles.tex8192x8192 = builder.createTextureArrayBuffer(desc);
                desc.textureCount = atlas.getBufferOccupations()[TEXTURE_ATLAS_16384x16384].size();
                desc.textureDesc.size = {16384, 16384};
                currentHandles.tex16384x16384 = builder.createTextureArrayBuffer(desc);

                builder.read(currentHandles.tex8x8);
                builder.read(currentHandles.tex16x16);
                builder.read(currentHandles.tex32x32);
                builder.read(currentHandles.tex64x64);
                builder.read(currentHandles.tex128x128);
                builder.read(currentHandles.tex256x256);
                builder.read(currentHandles.tex512x512);
                builder.read(currentHandles.tex1024x1024);
                builder.read(currentHandles.tex2048x2048);
                builder.read(currentHandles.tex4096x4096);
                builder.read(currentHandles.tex8192x8192);
                builder.read(currentHandles.tex16384x16384);

                builder.write(currentHandles.tex8x8);
                builder.write(currentHandles.tex16x16);
                builder.write(currentHandles.tex32x32);
                builder.write(currentHandles.tex64x64);
                builder.write(currentHandles.tex128x128);
                builder.write(currentHandles.tex256x256);
                builder.write(currentHandles.tex512x512);
                builder.write(currentHandles.tex1024x1024);
                builder.write(currentHandles.tex2048x2048);
                builder.write(currentHandles.tex4096x4096);
                builder.write(currentHandles.tex8192x8192);
                builder.write(currentHandles.tex16384x16384);

                builder.persist(currentHandles.tex8x8);
                builder.persist(currentHandles.tex16x16);
                builder.persist(currentHandles.tex32x32);
                builder.persist(currentHandles.tex64x64);
                builder.persist(currentHandles.tex128x128);
                builder.persist(currentHandles.tex256x256);
                builder.persist(currentHandles.tex512x512);
                builder.persist(currentHandles.tex1024x1024);
                builder.persist(currentHandles.tex2048x2048);
                builder.persist(currentHandles.tex4096x4096);
                builder.persist(currentHandles.tex8192x8192);
                builder.persist(currentHandles.tex16384x16384);
            } else {
                builder.read(currentHandles.tex8x8);
                builder.read(currentHandles.tex16x16);
                builder.read(currentHandles.tex32x32);
                builder.read(currentHandles.tex64x64);
                builder.read(currentHandles.tex128x128);
                builder.read(currentHandles.tex256x256);
                builder.read(currentHandles.tex512x512);
                builder.read(currentHandles.tex1024x1024);
                builder.read(currentHandles.tex2048x2048);
                builder.read(currentHandles.tex4096x4096);
                builder.read(currentHandles.tex8192x8192);
                builder.read(currentHandles.tex16384x16384);

                builder.write(currentHandles.tex8x8);
                builder.write(currentHandles.tex16x16);
                builder.write(currentHandles.tex32x32);
                builder.write(currentHandles.tex64x64);
                builder.write(currentHandles.tex128x128);
                builder.write(currentHandles.tex256x256);
                builder.write(currentHandles.tex512x512);
                builder.write(currentHandles.tex1024x1024);
                builder.write(currentHandles.tex2048x2048);
                builder.write(currentHandles.tex4096x4096);
                builder.write(currentHandles.tex8192x8192);
                builder.write(currentHandles.tex16384x16384);

                builder.persist(currentHandles.tex8x8);
                builder.persist(currentHandles.tex16x16);
                builder.persist(currentHandles.tex32x32);
                builder.persist(currentHandles.tex64x64);
                builder.persist(currentHandles.tex128x128);
                builder.persist(currentHandles.tex256x256);
                builder.persist(currentHandles.tex512x512);
                builder.persist(currentHandles.tex1024x1024);
                builder.persist(currentHandles.tex2048x2048);
                builder.persist(currentHandles.tex4096x4096);
                builder.persist(currentHandles.tex8192x8192);
                builder.persist(currentHandles.tex16384x16384);

                for (auto &pair: atlas.getBufferOccupations()) {
                    if (bufferSizes[pair.first] < pair.second.size()) {
                        bufferSizes[pair.first] = pair.second.size();
                        TextureArrayBufferDesc desc;
                        desc.textureCount = pair.second.size();
                        desc.textureDesc.generateMipmap = true;
                        switch (pair.first) {
                            case TEXTURE_ATLAS_8x8:
                                desc.textureDesc.size = {8, 8};
                                currentHandles.tex8x8 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex8x8);
                                builder.read(currentHandles.tex8x8);
                                builder.write(currentHandles.tex8x8);
                                break;
                            case TEXTURE_ATLAS_16x16:
                                desc.textureDesc.size = {16, 16};
                                currentHandles.tex16x16 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex16x16);
                                builder.read(currentHandles.tex16x16);
                                builder.write(currentHandles.tex16x16);
                                break;
                            case TEXTURE_ATLAS_32x32:
                                desc.textureDesc.size = {32, 32};
                                currentHandles.tex32x32 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex32x32);
                                builder.read(currentHandles.tex32x32);
                                builder.write(currentHandles.tex32x32);
                                break;
                            case TEXTURE_ATLAS_64x64:
                                desc.textureDesc.size = {64, 64};
                                currentHandles.tex64x64 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex64x64);
                                builder.read(currentHandles.tex64x64);
                                builder.write(currentHandles.tex64x64);
                                break;
                            case TEXTURE_ATLAS_128x128:
                                desc.textureDesc.size = {128, 128};
                                currentHandles.tex128x128 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex128x128);
                                builder.read(currentHandles.tex128x128);
                                builder.write(currentHandles.tex128x128);
                                break;
                            case TEXTURE_ATLAS_256x256:
                                desc.textureDesc.size = {256, 256};
                                currentHandles.tex256x256 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex256x256);
                                builder.read(currentHandles.tex256x256);
                                builder.write(currentHandles.tex256x256);
                                break;
                            case TEXTURE_ATLAS_512x512:
                                desc.textureDesc.size = {512, 512};
                                currentHandles.tex512x512 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex512x512);
                                builder.read(currentHandles.tex512x512);
                                builder.write(currentHandles.tex512x512);
                                break;
                            case TEXTURE_ATLAS_1024x1024:
                                desc.textureDesc.size = {1024, 1024};
                                currentHandles.tex1024x1024 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex1024x1024);
                                builder.read(currentHandles.tex1024x1024);
                                builder.write(currentHandles.tex1024x1024);
                                break;
                            case TEXTURE_ATLAS_2048x2048:
                                desc.textureDesc.size = {2048, 2048};
                                currentHandles.tex2048x2048 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex2048x2048);
                                builder.read(currentHandles.tex2048x2048);
                                builder.write(currentHandles.tex2048x2048);
                                break;
                            case TEXTURE_ATLAS_4096x4096:
                                desc.textureDesc.size = {4096, 4096};
                                currentHandles.tex4096x4096 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex4096x4096);
                                builder.read(currentHandles.tex4096x4096);
                                builder.write(currentHandles.tex4096x4096);
                                break;
                            case TEXTURE_ATLAS_8192x8192:
                                desc.textureDesc.size = {8192, 8192};
                                currentHandles.tex8192x8192 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex8192x8192);
                                builder.read(currentHandles.tex8192x8192);
                                builder.write(currentHandles.tex8192x8192);
                                break;
                            case TEXTURE_ATLAS_16384x16384:
                                desc.textureDesc.size = {16384, 16384};
                                currentHandles.tex16384x16384 = builder.createTextureArrayBuffer(desc);
                                builder.persist(currentHandles.tex16384x16384);
                                builder.read(currentHandles.tex16384x16384);
                                builder.write(currentHandles.tex16384x16384);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        std::map<TextureAtlasResolution, std::reference_wrapper<TextureArrayBuffer>>
        getAtlasBuffers(FrameGraphPassResources &resources,
                        CommandBuffer &commandBuffer,
                        CommandQueue &renderQueue) {
            commandBuffer.begin();
            if (currentHandles.tex8x8 != previousHandles.tex8x8 && previousHandles.tex8x8.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex8x8);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex8x8);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex16x16 != previousHandles.tex16x16 && previousHandles.tex16x16.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex16x16);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex16x16);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex32x32 != previousHandles.tex32x32 && previousHandles.tex32x32.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex32x32);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex32x32);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex64x64 != previousHandles.tex64x64 && previousHandles.tex64x64.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex64x64);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex64x64);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex128x128 != previousHandles.tex128x128 && previousHandles.tex128x128.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex128x128);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex128x128);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex256x256 != previousHandles.tex256x256 && previousHandles.tex256x256.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex256x256);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex256x256);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex512x512 != previousHandles.tex512x512 && previousHandles.tex512x512.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex512x512);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex512x512);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex1024x1024 != previousHandles.tex1024x1024 && previousHandles.tex1024x1024.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex1024x1024);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex1024x1024);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex2048x2048 != previousHandles.tex2048x2048 && previousHandles.tex2048x2048.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex2048x2048);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex2048x2048);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex4096x4096 != previousHandles.tex4096x4096 && previousHandles.tex4096x4096.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex4096x4096);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex4096x4096);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex8192x8192 != previousHandles.tex8192x8192 && previousHandles.tex8192x8192.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex8192x8192);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex8192x8192);
                commandBuffer.add(bufA.copy(bufB));
            }
            if (currentHandles.tex16384x16384 != previousHandles.tex16384x16384 &&
                previousHandles.tex16384x16384.assigned) {
                auto &bufA = resources.get<TextureArrayBuffer>(currentHandles.tex16384x16384);
                auto &bufB = resources.get<TextureArrayBuffer>(previousHandles.tex16384x16384);
                commandBuffer.add(bufA.copy(bufB));
            }

            commandBuffer.end();

            renderQueue.submit(commandBuffer);

            auto ret = std::map<TextureAtlasResolution, std::reference_wrapper<TextureArrayBuffer>>{
                    {TEXTURE_ATLAS_8x8,         resources.get<TextureArrayBuffer>(
                            currentHandles.tex8x8)},
                    {TEXTURE_ATLAS_16x16,       resources.get<TextureArrayBuffer>(
                            currentHandles.tex16x16)},
                    {TEXTURE_ATLAS_32x32,       resources.get<TextureArrayBuffer>(
                            currentHandles.tex32x32)},
                    {TEXTURE_ATLAS_64x64,       resources.get<TextureArrayBuffer>(
                            currentHandles.tex64x64)},
                    {TEXTURE_ATLAS_128x128,     resources.get<TextureArrayBuffer>(
                            currentHandles.tex128x128)},
                    {TEXTURE_ATLAS_256x256,     resources.get<TextureArrayBuffer>(
                            currentHandles.tex256x256)},
                    {TEXTURE_ATLAS_512x512,     resources.get<TextureArrayBuffer>(
                            currentHandles.tex512x512)},
                    {TEXTURE_ATLAS_1024x1024,   resources.get<TextureArrayBuffer>(
                            currentHandles.tex1024x1024)},
                    {TEXTURE_ATLAS_2048x2048,   resources.get<TextureArrayBuffer>(
                            currentHandles.tex2048x2048)},
                    {TEXTURE_ATLAS_4096x4096,   resources.get<TextureArrayBuffer>(
                            currentHandles.tex4096x4096)},
                    {TEXTURE_ATLAS_8192x8192,   resources.get<TextureArrayBuffer>(
                            currentHandles.tex8192x8192)},
                    {TEXTURE_ATLAS_16384x16384, resources.get<TextureArrayBuffer>(
                            currentHandles.tex16384x16384)},
            };

            for (auto &tex : pendingTextures){
                TextureAtlas::upload(tex.first, ret, tex.second);
            }
            pendingTextures.clear();

            return ret;
        }

        struct TextureHandles {
            FrameGraphResource tex8x8;
            FrameGraphResource tex16x16;
            FrameGraphResource tex32x32;
            FrameGraphResource tex64x64;
            FrameGraphResource tex128x128;
            FrameGraphResource tex256x256;
            FrameGraphResource tex512x512;
            FrameGraphResource tex1024x1024;
            FrameGraphResource tex2048x2048;
            FrameGraphResource tex4096x4096;
            FrameGraphResource tex8192x8192;
            FrameGraphResource tex16384x16384;
        };

        TextureHandles currentHandles;
        TextureHandles previousHandles;

        TextureAtlas atlas;

        std::map<TextureAtlasResolution, size_t> bufferSizes;

        std::unordered_map<TextureAtlasHandle, ImageRGBA> pendingTextures;
    };
}
#endif //XENGINE_FRAMEGRAPHTEXTUREATLAS_HPP
