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

#ifndef XENGINE_CONSTRUCTIONPASS_HPP
#define XENGINE_CONSTRUCTIONPASS_HPP

#include <unordered_set>

#include "xng/resource/uri.hpp"

#include "xng/render/graph/framegraphpass.hpp"
#include "xng/render/graph/framegraphtextureatlas.hpp"
#include "xng/render/scene/scene.hpp"
#include "xng/render/graph/meshallocator.hpp"

namespace xng {
    /**
     * The ConstructionPass creates and assigns the geometry buffer slot textures and creates and assigns the SLOT_DEFERRED_* and SLOT_FORWARD_* slot textures.
     */
    class XENGINE_EXPORT ConstructionPass : public FrameGraphPass {
    public:
        ~ConstructionPass() override = default;

        void setup(FrameGraphBuilder &builder) override;

        std::type_index getTypeIndex() const override;

    private:
        TextureAtlasHandle getTexture(const ResourceHandle<Texture> &texture,
                                      std::map<TextureAtlasResolution, FrameGraphResource> &atlasBuffers);

        void deallocateTexture(const ResourceHandle<Texture> &texture);

        FrameGraphResource renderPipeline;
        FrameGraphResource renderPipelineSkinned;

        FrameGraphResource vertexBuffer;
        FrameGraphResource indexBuffer;

        FrameGraphTextureAtlas atlas;

        size_t currentVertexBufferSize{};
        size_t currentIndexBufferSize{};

        MeshAllocator meshAllocator;

        std::map<Uri, TextureAtlasHandle> textures;
    };
}

#endif //XENGINE_CONSTRUCTIONPASS_HPP
