/**
 *  This file is part of xEngine, a C++ game engine library.
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

#ifndef XENGINE_TESTPASS_HPP
#define XENGINE_TESTPASS_HPP

#include "xng/xng.hpp"

using namespace xng;

static const char *SHADER_VERT_GEOMETRY = R"###(#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;

layout(location = 0) out vec4 fPos;
layout(location = 1) out vec2 fUv;

layout(binding = 0, std140) uniform ShaderUniformBuffer
{
    vec4 visualizeDepth_near_far;
} globs;

layout(binding = 1) uniform sampler2D tex;

void main()
{
    fPos = vec4(vPosition, 1);
    fUv = vUv;
    gl_Position = fPos;
}
)###";

static const char *SHADER_FRAG_GEOMETRY = R"###(#version 460

layout(location = 0) in vec4 fPos;
layout(location = 1) in vec2 fUv;

layout(location = 0) out vec4 oColor;

layout(binding = 0, std140) uniform ShaderUniformBuffer
{
    vec4 visualizeDepth_near_far;
} globs;

layout(binding = 1) uniform sampler2D tex;

void main() {
    oColor = texture(tex, fUv);
    if (globs.visualizeDepth_near_far.x != 0)
    {
        float near = globs.visualizeDepth_near_far.y;
        float far = globs.visualizeDepth_near_far.z;
        float ndc = oColor.r * 2.0 - 1.0;
        float linearDepth = 1 - ((2.0 * near * far) / (far + near - ndc * (far - near)) / far);
        oColor = vec4(linearDepth, linearDepth, linearDepth, 1);
    }
}
)###";

struct ShaderData {
    float visualizeDepth_near_far[4];
};

class TestPass : public FrameGraphPass {
public:
    SHARED_PROPERTY(TestPass, COLOR)

    void setup(FrameGraphBuilder &builder) override {
        if (!vertexBufferRes.assigned) {
            VertexBufferDesc desc;
            desc.size = mesh.vertices.size() * mesh.vertexLayout.getSize();
            vertexBufferRes = builder.createVertexBuffer(desc);

            VertexArrayObjectDesc oDesc;
            oDesc.vertexLayout = mesh.vertexLayout;
            vertexArrayObjectRes = builder.createVertexArrayObject(oDesc);

            builder.write(vertexBufferRes);
        }

        builder.persist(vertexBufferRes);
        builder.persist(vertexArrayObjectRes);
        builder.read(vertexBufferRes);
        builder.read(vertexArrayObjectRes);

        if (!pipelineRes.assigned) {
            auto vs = ShaderSource(SHADER_VERT_GEOMETRY, "main", xng::VERTEX, xng::GLSL_460, false);
            auto fs = ShaderSource(SHADER_FRAG_GEOMETRY, "main", xng::FRAGMENT, xng::GLSL_460, false);

            vs = vs.preprocess(builder.getShaderCompiler());
            fs = fs.preprocess(builder.getShaderCompiler());

            vsb = vs.compile(builder.getShaderCompiler());
            fsb = fs.compile(builder.getShaderCompiler());

            pipelineRes = builder.createPipeline(RenderPipelineDesc{
                    .shaders = {
                            {VERTEX,   vsb.getShader()},
                            {FRAGMENT, fsb.getShader()}
                    },
                    .bindings = {BIND_SHADER_UNIFORM_BUFFER,
                                 BIND_TEXTURE_BUFFER},
                    .primitive = TRIANGLES,
                    .vertexLayout = mesh.vertexLayout,
                    .clearColorValue = ColorRGBA::gray(),
                    .clearColor = true,
                    .clearDepth = true,
                    .enableBlending = true
            });
        }

        builder.persist(pipelineRes);
        builder.read(pipelineRes);

        passRes = builder.createRenderPass(
                RenderPassDesc{.numberOfColorAttachments = builder.getBackBufferDescription().numberOfColorAttachments,
                        .hasDepthStencilAttachment = builder.getBackBufferDescription().hasDepthStencilAttachment});

        builder.read(passRes);

        shaderBufferRes = builder.createShaderUniformBuffer(ShaderUniformBufferDesc{.size =  sizeof(ShaderData)});
        builder.read(shaderBufferRes);
        builder.write(shaderBufferRes);

        switch (tex) {
            default:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(
                        GBufferPass::GEOMETRY_BUFFER_POSITION);
                break;
            case 1:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(GBufferPass::GEOMETRY_BUFFER_NORMAL);
                break;
            case 2:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(GBufferPass::GEOMETRY_BUFFER_TANGENT);
                break;
            case 3:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(
                        GBufferPass::GEOMETRY_BUFFER_ROUGHNESS_METALLIC_AO);
                break;
            case 4:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(GBufferPass::GEOMETRY_BUFFER_ALBEDO);
                break;
            case 5:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(GBufferPass::GEOMETRY_BUFFER_AMBIENT);
                break;
            case 6:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(
                        GBufferPass::GEOMETRY_BUFFER_SPECULAR);
                break;
            case 7:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(
                        GBufferPass::GEOMETRY_BUFFER_MODEL_OBJECT);
                break;
            case 8:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(GBufferPass::GEOMETRY_BUFFER_DEPTH);
                break;
            case 9:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(PhongDeferredPass::COLOR);
                break;
            case 10:
                displayTextureRes = builder.getSharedData().get<FrameGraphResource>(PhongDeferredPass::DEPTH);
                break;
        }
        builder.read(displayTextureRes);

        screenRes = builder.getBackBuffer();
        builder.write(screenRes);

        camera = builder.getScene().camera;
    }

    void execute(FrameGraphPassResources &resources) override {
        auto &target = resources.get<RenderTarget>(screenRes);

        auto &pipeline = resources.get<RenderPipeline>(pipelineRes);
        auto &pass = resources.get<RenderPass>(passRes);

        auto &vertexBuffer = resources.get<VertexBuffer>(vertexBufferRes);
        auto &vertexArrayObject = resources.get<VertexArrayObject>(vertexArrayObjectRes);

        auto &shaderBuffer = resources.get<ShaderUniformBuffer>(shaderBufferRes);

        if (!quadAllocated) {
            quadAllocated = true;
            auto verts = VertexStream().addVertices(mesh.vertices).getVertexBuffer();
            vertexBuffer.upload(0,
                                verts.data(),
                                verts.size());
            vertexArrayObject.bindBuffers(vertexBuffer);
        }

        ShaderData buf;
        buf.visualizeDepth_near_far[0] = tex == 8 || tex == 10;
        buf.visualizeDepth_near_far[1] = camera.nearClip;
        buf.visualizeDepth_near_far[2] = camera.farClip;

        shaderBuffer.upload(buf);

        auto &texture = resources.get<TextureBuffer>(displayTextureRes);

        pass.beginRenderPass(target, {}, target.getDescription().size);

        pass.bindPipeline(pipeline);
        pass.bindVertexArrayObject(vertexArrayObject);
        pass.bindShaderData({
                                    shaderBuffer,
                                    texture
                            });
        pass.drawArray(RenderPass::DrawCall{.offset = 0, .count = mesh.vertices.size()});

        pass.endRenderPass();
    }

    std::type_index getTypeIndex() const override {
        return typeid(TestPass);
    }

    void setTex(int t) {
        tex = t;
    }

    int getTex() {
        return tex;
    }

    void incrementTex() {
        if (++tex > 10) {
            tex = 0;
        }
    }

    void decrementTex() {
        if (--tex < 0) {
            tex = 10;
        }
    }

private:
    Mesh mesh = Mesh::normalizedQuad();

    int tex = 0;

    FrameGraphResource screenRes;

    FrameGraphResource pipelineRes;
    FrameGraphResource passRes;

    FrameGraphResource shaderBufferRes;

    FrameGraphResource vertexBufferRes;
    FrameGraphResource vertexArrayObjectRes;

    SPIRVBundle vsb;
    SPIRVBundle fsb;

    bool quadAllocated = false;

    FrameGraphResource displayTextureRes;

    Camera camera;
};

#endif //XENGINE_TESTPASS_HPP
