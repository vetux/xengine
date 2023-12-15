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

#include "xng/render/graph/passes/constructionpass.hpp"
#include "xng/render/graph/framegraphbuilder.hpp"
#include "xng/render/graph/framegraphsettings.hpp"
#include "xng/render/atlas/textureatlas.hpp"

#include "xng/render/geometry/vertexstream.hpp"

#include "graph/constructionpass_vs.hpp" // Generated by cmake
#include "graph/constructionpass_vs_skinned.hpp" // Generated by cmake
#include "graph/constructionpass_fs.hpp" // Generated by cmake

namespace xng {
#pragma pack(push, 1)
    struct ShaderAtlasTexture {
        int level_index_filtering_assigned[4]{0, 0, 0, 0};
        float atlasScale_texSize[4]{0, 0, 0, 0};
    };

    struct ShaderDrawData {
        Mat4f model;
        Mat4f mvp;

        int objectID_boneOffset_shadows[4]{0, 0, 0, 0};

        float metallic_roughness_ambientOcclusion[4]{0, 0, 0, 0};
        float albedoColor[4]{0, 0, 0, 0};

        float normalIntensity[4]{0, 0, 0, 0};

        ShaderAtlasTexture normal;

        ShaderAtlasTexture metallic;
        ShaderAtlasTexture roughness;
        ShaderAtlasTexture ambientOcclusion;
        ShaderAtlasTexture albedo;
    };
#pragma pack(pop)

    ConstructionPass::ConstructionPass() {}

    void ConstructionPass::setup(FrameGraphBuilder &builder) {
        auto renderSize = builder.getBackBufferDescription().size
                          * builder.getSettings().get<float>(FrameGraphSettings::SETTING_RENDER_SCALE);

        if (!renderPipelineRes.assigned) {
            renderPipelineRes = builder.createRenderPipeline(RenderPipelineDesc{
                    .shaders = {{VERTEX,   constructionpass_vs},
                                {FRAGMENT, constructionpass_fs}},
                    .bindings = {
                            BIND_SHADER_STORAGE_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER
                    },
                    .vertexLayout = Mesh::getDefaultVertexLayout(),
                    /*       .clearColorValue = ColorRGBA(0, 0, 0, 0),
                           .clearColor = false,
                           .clearDepth = false,
                           .clearStencil = false,*/
                    .enableDepthTest = true,
                    .depthTestWrite = true,
                    .depthTestMode = DEPTH_TEST_LESS,
                    .enableFaceCulling = true,
                    .enableBlending = false
            });
        }

        builder.persist(renderPipelineRes);

        if (!renderPipelineSkinnedRes.assigned) {
            renderPipelineSkinnedRes = builder.createRenderPipeline(RenderPipelineDesc{
                    .shaders = {{VERTEX,   constructionpass_vs_skinned},
                                {FRAGMENT, constructionpass_fs}},
                    .bindings = {
                            BIND_SHADER_STORAGE_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_TEXTURE_ARRAY_BUFFER,
                            BIND_SHADER_STORAGE_BUFFER,
                    },
                    .vertexLayout = SkinnedMesh::getDefaultVertexLayout(),
                    /*       .clearColorValue = ColorRGBA(0, 0, 0, 0),
                           .clearColor = false,
                           .clearDepth = false,
                           .clearStencil = false,*/
                    .enableDepthTest = true,
                    .depthTestWrite = true,
                    .depthTestMode = DEPTH_TEST_LESS,
                    .enableFaceCulling = true,
                    .enableBlending = false,
            });
        }

        builder.persist(renderPipelineSkinnedRes);

        if (!vertexArrayObjectRes.assigned) {
            vertexArrayObjectRes = builder.createVertexArrayObject(VertexArrayObjectDesc{
                    .vertexLayout = SkinnedMesh::getDefaultVertexLayout()
            });
        }

        builder.persist(vertexArrayObjectRes);

        auto desc = TextureBufferDesc();
        desc.size = renderSize;
        desc.format = RGBA32F;

        auto gBufferPosition = builder.createTextureBuffer(desc);
        auto gBufferNormal = builder.createTextureBuffer(desc);
        auto gBufferTangent = builder.createTextureBuffer(desc);
        auto gBufferRoughnessMetallicAmbientOcclusion = builder.createTextureBuffer(desc);

        desc.format = RGBA;

        auto gBufferAlbedo = builder.createTextureBuffer(desc);

        desc.format = RGBA32I;

        auto gBufferObjectShadows = builder.createTextureBuffer(desc);

        desc.format = DEPTH_STENCIL;
        auto gBufferDepth = builder.createTextureBuffer(desc);

        std::vector<Node> objects;
        size_t totalShaderBufferSize = 0;

        std::set<Uri> usedTextures;
        std::set<Uri> usedMeshes;

        size_t boneCount = 0;

        auto tmp = builder.getScene().rootNode.findAll({typeid(SkinnedMeshProperty)});
        for (auto id = 0; id < tmp.size(); id++) {
            auto &object = tmp.at(id);
            auto &meshProp = object.getProperty<SkinnedMeshProperty>();
            if (meshProp.mesh.assigned()) {
                auto it = object.properties.find(typeid(MaterialProperty));
                MaterialProperty matProp;
                if (it != object.properties.end()) {
                    matProp = it->second->get<MaterialProperty>();
                }

                meshAllocator.prepareMeshAllocation(meshProp.mesh);
                usedMeshes.insert(meshProp.mesh.getUri());

                for (auto i = 0; i < meshProp.mesh.get().subMeshes.size() + 1; i++) {
                    auto &mesh = i == 0 ? meshProp.mesh.get() : meshProp.mesh.get().subMeshes.at(i - 1);

                    Material mat = mesh.material.get();

                    auto mi = matProp.materials.find(i);
                    if (mi != matProp.materials.end()) {
                        mat = mi->second.get();
                    }

                    if (mat.transparent) {
                        continue;
                    }

                    boneCount += mesh.bones.size();

                    if (mat.normal.assigned()) {
                        if (textures.find(mat.normal.getUri()) == textures.end()) {
                            textures[mat.normal.getUri()] = atlas.add(mat.normal.get().image.get());
                        }
                        usedTextures.insert(mat.normal.getUri());
                    }
                    if (mat.metallicTexture.assigned()) {
                        if (textures.find(mat.metallicTexture.getUri()) == textures.end()) {
                            textures[mat.metallicTexture.getUri()] = atlas.add(mat.metallicTexture.get().image.get());
                        }
                        usedTextures.insert(mat.metallicTexture.getUri());
                    }
                    if (mat.roughnessTexture.assigned()) {
                        if (textures.find(mat.roughnessTexture.getUri()) == textures.end()) {
                            textures[mat.roughnessTexture.getUri()] = atlas.add(mat.roughnessTexture.get().image.get());
                        }
                        usedTextures.insert(mat.roughnessTexture.getUri());
                    }
                    if (mat.ambientOcclusionTexture.assigned()) {
                        if (textures.find(mat.ambientOcclusionTexture.getUri()) == textures.end()) {
                            textures[mat.ambientOcclusionTexture.getUri()] = atlas.add(
                                    mat.ambientOcclusionTexture.get().image.get());
                        }
                        usedTextures.insert(mat.ambientOcclusionTexture.getUri());
                    }
                    if (mat.albedoTexture.assigned()) {
                        if (textures.find(mat.albedoTexture.getUri()) == textures.end()) {
                            textures[mat.albedoTexture.getUri()] = atlas.add(mat.albedoTexture.get().image.get());
                        }
                        usedTextures.insert(mat.albedoTexture.getUri());
                    }

                    totalShaderBufferSize += sizeof(ShaderDrawData);
                }
                objects.emplace_back(object);
            }
        }

        auto shaderBufferRes = builder.createShaderStorageBuffer(ShaderStorageBufferDesc{
                .bufferType = RenderBufferType::HOST_VISIBLE,
                .size = totalShaderBufferSize
        });

        auto boneBufferRes = builder.createShaderStorageBuffer(ShaderStorageBufferDesc{
                .bufferType = RenderBufferType::HOST_VISIBLE,
                .size = sizeof(Mat4f) * boneCount
        });

        atlas.setup(builder);

        if (vertexBufferRes.assigned) {
            builder.persist(vertexBufferRes);
        }

        if (indexBufferRes.assigned) {
            builder.persist(indexBufferRes);
        }

        if (!vertexBufferRes.assigned || currentVertexBufferSize < meshAllocator.getRequestedVertexBufferSize()) {
            staleVertexBuffer = vertexBufferRes;
            auto d = VertexBufferDesc();
            d.size = meshAllocator.getRequestedVertexBufferSize();
            vertexBufferRes = builder.createVertexBuffer(d);
            currentVertexBufferSize = d.size;
            builder.persist(vertexBufferRes);
        }

        if (!indexBufferRes.assigned || currentIndexBufferSize < meshAllocator.getRequestedIndexBufferSize()) {
            staleIndexBuffer = indexBufferRes;
            auto d = IndexBufferDesc();
            d.size = meshAllocator.getRequestedIndexBufferSize();
            indexBufferRes = builder.createIndexBuffer(d);
            currentIndexBufferSize = d.size;
            builder.persist(indexBufferRes);
        }

        auto cameraNode = builder.getScene().rootNode.find<CameraProperty>();
        auto camera = cameraNode.getProperty<CameraProperty>().camera;
        auto cameraTransform = cameraNode.getProperty<TransformProperty>().transform;

        builder.assignSlot(SLOT_GBUFFER_POSITION, gBufferPosition);
        builder.assignSlot(SLOT_GBUFFER_NORMAL, gBufferNormal);
        builder.assignSlot(SLOT_GBUFFER_TANGENT, gBufferTangent);
        builder.assignSlot(SLOT_GBUFFER_ROUGHNESS_METALLIC_AO, gBufferRoughnessMetallicAmbientOcclusion);
        builder.assignSlot(SLOT_GBUFFER_ALBEDO, gBufferAlbedo);
        builder.assignSlot(SLOT_GBUFFER_OBJECT_SHADOWS, gBufferObjectShadows);
        builder.assignSlot(SLOT_GBUFFER_DEPTH, gBufferDepth);

        auto atlasBuffers = atlas.getAtlasBuffers(builder);

        bool updateVao = false;
        if (staleVertexBuffer.assigned) {
            builder.copy(staleVertexBuffer, vertexBufferRes);
            staleVertexBuffer = {};
            updateVao = true;
        }

        if (staleIndexBuffer.assigned) {
            builder.copy(staleIndexBuffer, indexBufferRes);
            staleIndexBuffer = {};
            updateVao = true;
        }

        if (updateVao || bindVao) {
            bindVao = false;
            builder.setVertexArrayObjectBuffers(vertexArrayObjectRes, vertexBufferRes, indexBufferRes, {});
        }

        meshAllocator.uploadMeshes(builder, vertexBufferRes, indexBufferRes);

        // Deallocate unused meshes
        std::set<Uri> dealloc;
        for (auto &pair: meshAllocator.getMeshAllocations()) {
            if (usedMeshes.find(pair.first) == usedMeshes.end()) {
                dealloc.insert(pair.first);
            }
        }
        for (auto &uri: dealloc) {
            meshAllocator.deallocateMesh(ResourceHandle<SkinnedMesh>(uri));
        }

        // Deallocate unused textures
        dealloc.clear();
        for (auto &pair: textures) {
            if (usedTextures.find(pair.first) == usedTextures.end()) {
                dealloc.insert(pair.first);
            }
        }
        for (auto &uri: dealloc) {
            deallocateTexture(ResourceHandle<Texture>(uri));
        }


        // Draw geometry buffer
        auto projection = camera.projection();
        auto view = Camera::view(cameraTransform);

        if (!objects.empty()) {
            std::vector<DrawCall> drawCalls;
            std::vector<size_t> baseVertices;
            std::vector<ShaderDrawData> shaderData;
            std::vector<Mat4f> boneMatrices;

            for (auto oi = 0; oi < objects.size(); oi++) {
                auto &node = objects.at(oi);
                auto &meshProp = node.getProperty<SkinnedMeshProperty>();

                auto rig = meshProp.mesh.get().rig;

                std::map<std::string, Mat4f> boneTransforms;
                auto it = node.properties.find(typeid(BoneTransformsProperty));
                if (it != node.properties.end()) {
                    boneTransforms = it->second->get<BoneTransformsProperty>().boneTransforms;
                }

                std::map<size_t, ResourceHandle<Material>> mats;
                it = node.properties.find(typeid(MaterialProperty));
                if (it != node.properties.end()) {
                    mats = it->second->get<MaterialProperty>().materials;
                }

                auto drawData = meshAllocator.getAllocatedMesh(meshProp.mesh);

                for (auto i = 0; i < meshProp.mesh.get().subMeshes.size() + 1; i++) {
                    auto model = node.getProperty<TransformProperty>().transform.model();

                    auto &mesh = i == 0 ? meshProp.mesh.get() : meshProp.mesh.get().subMeshes.at(i - 1);

                    auto material = mesh.material.get();

                    auto mIt = mats.find(i);
                    if (mIt != mats.end()) {
                        material = mIt->second.get();
                    }

                    if (material.transparent) {
                        continue;
                    }

                    auto boneOffset = boneMatrices.size();
                    if (mesh.bones.empty()) {
                        boneOffset = -1;
                    } else {
                        for (auto &bone: mesh.bones) {
                            Mat4f mat;
                            auto bt = boneTransforms.find(bone);
                            if (bt != boneTransforms.end()) {
                                boneMatrices.emplace_back(bt->second);
                            } else {
                                boneMatrices.emplace_back(MatrixMath::identity());
                            }
                        }
                    }

                    bool receiveShadows = true;
                    if (node.hasProperty<ShadowProperty>()) {
                        receiveShadows = node.getProperty<ShadowProperty>().receiveShadows;
                    }

                    auto data = ShaderDrawData();

                    data.model = model;
                    data.mvp = projection * view * model;
                    data.objectID_boneOffset_shadows[0] = static_cast<int>(oi);
                    data.objectID_boneOffset_shadows[1] = static_cast<int>(boneOffset);
                    data.objectID_boneOffset_shadows[2] = receiveShadows;

                    data.metallic_roughness_ambientOcclusion[0] = material.metallic;
                    data.metallic_roughness_ambientOcclusion[1] = material.roughness;
                    data.metallic_roughness_ambientOcclusion[2] = material.ambientOcclusion;

                    auto col = material.albedo.divide().getMemory();
                    data.albedoColor[0] = col[0];
                    data.albedoColor[1] = col[1];
                    data.albedoColor[2] = col[2];
                    data.albedoColor[3] = col[3];

                    data.normalIntensity[0] = material.normalIntensity;

                    if (material.metallicTexture.assigned()) {
                        auto tex = getTexture(material.metallicTexture, atlasBuffers);

                        data.metallic.level_index_filtering_assigned[0] = tex.level;
                        data.metallic.level_index_filtering_assigned[1] = static_cast<int>(tex.index);
                        data.metallic.level_index_filtering_assigned[2] = material.metallicTexture.get().description.filterMag;
                        data.metallic.level_index_filtering_assigned[3] = 1;

                        auto atlasScale = tex.size.convert<float>()
                                          / TextureAtlas::getResolutionLevelSize(tex.level).convert<float>();

                        data.metallic.atlasScale_texSize[0] = atlasScale.x;
                        data.metallic.atlasScale_texSize[1] = atlasScale.y;
                        data.metallic.atlasScale_texSize[2] = static_cast<float>(tex.size.x);
                        data.metallic.atlasScale_texSize[3] = static_cast<float>(tex.size.y);
                    }

                    if (material.roughnessTexture.assigned()) {
                        auto tex = getTexture(material.roughnessTexture, atlasBuffers);

                        data.roughness.level_index_filtering_assigned[0] = tex.level;
                        data.roughness.level_index_filtering_assigned[1] = static_cast<int>(tex.index);
                        data.roughness.level_index_filtering_assigned[2] = material.roughnessTexture.get().description.filterMag;
                        data.roughness.level_index_filtering_assigned[3] = 1;

                        auto atlasScale = tex.size.convert<float>()
                                          / TextureAtlas::getResolutionLevelSize(tex.level).convert<float>();

                        data.roughness.atlasScale_texSize[0] = atlasScale.x;
                        data.roughness.atlasScale_texSize[1] = atlasScale.y;
                        data.roughness.atlasScale_texSize[2] = static_cast<float>(tex.size.x);
                        data.roughness.atlasScale_texSize[3] = static_cast<float>(tex.size.y);
                    }

                    if (material.ambientOcclusionTexture.assigned()) {
                        auto tex = getTexture(material.ambientOcclusionTexture, atlasBuffers);

                        data.ambientOcclusion.level_index_filtering_assigned[0] = tex.level;
                        data.ambientOcclusion.level_index_filtering_assigned[1] = static_cast<int>(tex.index);
                        data.ambientOcclusion.level_index_filtering_assigned[2] = material.ambientOcclusionTexture.get().description.filterMag;
                        data.ambientOcclusion.level_index_filtering_assigned[3] = 1;

                        auto atlasScale = tex.size.convert<float>()
                                          / TextureAtlas::getResolutionLevelSize(tex.level).convert<float>();

                        data.ambientOcclusion.atlasScale_texSize[0] = atlasScale.x;
                        data.ambientOcclusion.atlasScale_texSize[1] = atlasScale.y;
                        data.ambientOcclusion.atlasScale_texSize[2] = static_cast<float>(tex.size.x);
                        data.ambientOcclusion.atlasScale_texSize[3] = static_cast<float>(tex.size.y);
                    }

                    if (material.albedoTexture.assigned()) {
                        auto tex = getTexture(material.albedoTexture, atlasBuffers);

                        data.albedo.level_index_filtering_assigned[0] = tex.level;
                        data.albedo.level_index_filtering_assigned[1] = static_cast<int>(tex.index);
                        data.albedo.level_index_filtering_assigned[2] = material.albedoTexture.get().description.filterMag;
                        data.albedo.level_index_filtering_assigned[3] = 1;

                        auto atlasScale = tex.size.convert<float>()
                                          / TextureAtlas::getResolutionLevelSize(tex.level).convert<float>();

                        data.albedo.atlasScale_texSize[0] = atlasScale.x;
                        data.albedo.atlasScale_texSize[1] = atlasScale.y;
                        data.albedo.atlasScale_texSize[2] = static_cast<float>(tex.size.x);
                        data.albedo.atlasScale_texSize[3] = static_cast<float>(tex.size.y);
                    }

                    if (material.normal.assigned()) {
                        auto tex = getTexture(material.normal, atlasBuffers);

                        data.normal.level_index_filtering_assigned[0] = tex.level;
                        data.normal.level_index_filtering_assigned[1] = static_cast<int>(tex.index);
                        data.normal.level_index_filtering_assigned[2] = material.normal.get().description.filterMag;
                        data.normal.level_index_filtering_assigned[3] = 1;

                        auto atlasScale = tex.size.convert<float>()
                                          / TextureAtlas::getResolutionLevelSize(tex.level).convert<float>();

                        data.normal.atlasScale_texSize[0] = atlasScale.x;
                        data.normal.atlasScale_texSize[1] = atlasScale.y;
                        data.normal.atlasScale_texSize[2] = static_cast<float>(tex.size.x);
                        data.normal.atlasScale_texSize[3] = static_cast<float>(tex.size.y);
                    }

                    shaderData.emplace_back(data);

                    auto &draw = drawData.data.at(i);
                    drawCalls.emplace_back(draw.drawCall);
                    baseVertices.emplace_back(draw.baseVertex);
                }
            }

            builder.upload(shaderBufferRes,
                           [shaderData]() {
                               return FrameGraphCommand::UploadBuffer{shaderData.size() * sizeof(Mat4f),
                                                                      reinterpret_cast<const uint8_t *>(shaderData.data())};
                           });

            builder.upload(boneBufferRes,
                           [boneMatrices]() {
                               return FrameGraphCommand::UploadBuffer{boneMatrices.size() * sizeof(Mat4f),
                                                                      reinterpret_cast<const uint8_t *>(boneMatrices.data())};
                           });

            builder.beginPass({
                                      FrameGraphCommand::Attachment::texture(gBufferPosition),
                                      FrameGraphCommand::Attachment::texture(gBufferNormal),
                                      FrameGraphCommand::Attachment::texture(gBufferTangent),
                                      FrameGraphCommand::Attachment::texture(gBufferRoughnessMetallicAmbientOcclusion),
                                      FrameGraphCommand::Attachment::texture(gBufferAlbedo),
                                      FrameGraphCommand::Attachment::texture(gBufferObjectShadows)
                              },
                              FrameGraphCommand::Attachment::texture(gBufferDepth));

            builder.setViewport({}, renderSize);

            builder.bindPipeline(renderPipelineSkinnedRes);
            builder.bindVertexArrayObject(vertexArrayObjectRes);
            builder.bindShaderResources(std::vector<FrameGraphCommand::ShaderData>{
                    {shaderBufferRes,                            {{VERTEX, ShaderResource::READ}, {FRAGMENT, ShaderResource::READ}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_8x8),         {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_16x16),       {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_32x32),       {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_64x64),       {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_128x128),     {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_256x256),     {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_512x512),     {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_1024x1024),   {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_2048x2048),   {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_4096x4096),   {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_8192x8192),   {{{FRAGMENT, ShaderResource::READ}}}},
                    {atlasBuffers.at(TEXTURE_ATLAS_16384x16384), {{{FRAGMENT, ShaderResource::READ}}}},
                    {boneBufferRes,                              {{VERTEX, ShaderResource::READ}}},
            });

            builder.multiDrawIndexed(drawCalls, baseVertices);

            builder.finishPass();
        }
    }

    std::type_index ConstructionPass::getTypeIndex() const {
        return typeid(ConstructionPass);
    }

    TextureAtlasHandle ConstructionPass::getTexture(const ResourceHandle<Texture> &texture,
                                                    std::map<TextureAtlasResolution, FrameGraphResource> &atlasBuffers) {
        if (textures.find(texture.getUri()) == textures.end()) {
            auto handle = atlas.add(texture.get().image.get());
            textures[texture.getUri()] = handle;
        }
        return textures.at(texture.getUri());
    }

    void ConstructionPass::deallocateTexture(const ResourceHandle<Texture> &texture) {
        atlas.remove(textures.at(texture.getUri()));
        textures.erase(texture.getUri());
    }
}
