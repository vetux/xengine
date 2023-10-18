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
#include "xng/render/scene.hpp"
#include "xng/render/meshallocator.hpp"

namespace xng {
    /**
     * The ConstructionPass creates and assigns the geometry buffer slot textures and creates and assigns the SLOT_DEFERRED_* and SLOT_FORWARD_* slot textures.
     */
    class XENGINE_EXPORT ConstructionPass : public FrameGraphPass {
    public:
        ConstructionPass();

        void setup(FrameGraphBuilder &builder) override;

        void execute(FrameGraphPassResources &resources,
                     const std::vector<std::reference_wrapper<CommandQueue>> &renderQueues,
                     const std::vector<std::reference_wrapper<CommandQueue>> &computeQueues,
                     const std::vector<std::reference_wrapper<CommandQueue>> &transferQueues) override;

        std::type_index getTypeIndex() const override;

    private:
        TextureAtlasHandle getTexture(const ResourceHandle<Texture> &texture,
                                      std::map<TextureAtlasResolution, std::reference_wrapper<TextureArrayBuffer>> &atlasBuffers);

        void deallocateTexture(const ResourceHandle<Texture> &texture);

        FrameGraphResource renderTargetRes;
        FrameGraphResource renderPipelineRes;
        FrameGraphResource renderPipelineSkinnedRes;
        FrameGraphResource renderPassRes;
        FrameGraphResource shaderBufferRes;
        FrameGraphResource boneBufferRes;

        FrameGraphResource clearTargetRes;
        FrameGraphResource clearPassRes;

        FrameGraphResource vertexBufferRes;
        FrameGraphResource indexBufferRes;
        FrameGraphResource vertexArrayObjectRes;

        FrameGraphResource staleVertexBuffer;
        FrameGraphResource staleIndexBuffer;

        FrameGraphTextureAtlas atlas;

        FrameGraphResource gBufferPosition;
        FrameGraphResource gBufferNormal;
        FrameGraphResource gBufferTangent;
        FrameGraphResource gBufferRoughnessMetallicAmbientOcclusion;
        FrameGraphResource gBufferAlbedo;
        FrameGraphResource gBufferAmbient;
        FrameGraphResource gBufferSpecular;
        FrameGraphResource gBufferModelObject;
        FrameGraphResource gBufferDepth;

        FrameGraphResource screenColor;
        FrameGraphResource screenDepth;

        FrameGraphResource deferredColor;
        FrameGraphResource deferredDepth;

        FrameGraphResource forwardColor;
        FrameGraphResource forwardDepth;

        FrameGraphResource commandBuffer;

        Camera camera;
        Transform cameraTransform;

        std::vector<Scene::Node> objects;

        size_t currentVertexBufferSize{};
        size_t currentIndexBufferSize{};

        MeshAllocator meshAllocator;

        std::map<Uri, TextureAtlasHandle> textures;

        Vec2i renderSize;

        std::set<Uri> usedTextures;
        std::set<Uri> usedMeshes;

        bool bindVao = true;
    };
}
#endif //XENGINE_CONSTRUCTIONPASS_HPP
