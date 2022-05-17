/**
 *  xEngine - C++ game engine library
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

#include "render/deferred/prepass.hpp"
#include "render/shader/shaderinclude.hpp"

#include "render/shader/shadercompiler.hpp"

static const char *SHADER_VERT_GEOMETRY = R"###(#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUv;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in vec4 vInstanceRow0;
layout (location = 6) in vec4 vInstanceRow1;
layout (location = 7) in vec4 vInstanceRow2;
layout (location = 8) in vec4 vInstanceRow3;

layout(location = 0) out vec3 fPos;
layout(location = 1) out vec3 fNorm;
layout(location = 2) out vec3 fTan;
layout(location = 3) out vec2 fUv;
layout(location = 4) out vec4 vPos;

layout(location = 0) uniform mat4 MANA_M;
layout(location = 1) uniform mat4 MANA_MVP;

layout(location = 2) uniform int hasTextureNormal;

layout(location = 3) uniform vec4 diffuseColor;
layout(location = 4) uniform vec4 ambientColor;
layout(location = 5) uniform vec4 specularColor;
layout(location = 6) uniform float shininessColor;

layout(location = 7) uniform vec4 emissiveColor;

layout(location = 8) uniform sampler2D diffuse;
layout(location = 9) uniform sampler2D ambient;
layout(location = 10) uniform sampler2D specular;
layout(location = 11) uniform sampler2D shininess;
layout(location = 12) uniform sampler2D emissive;

layout(location = 13) uniform sampler2D normal;

layout(location = 14) uniform mat4 TRANSFORM_ROTATION;

void main()
{
    mat4 instanceMatrix = mat4(vInstanceRow0, vInstanceRow1, vInstanceRow2, vInstanceRow3);

    vPos = (instanceMatrix * MANA_MVP) * vec4(vPosition, 1);
    fPos = ((instanceMatrix * MANA_M) * vec4(vPosition, 1)).xyz;
    fUv = vUv;

    fNorm = normalize(vNormal);
    fTan = normalize(vTangent);

    gl_Position = vPos;
}
)###";

static const char *SHADER_FRAG_GEOMETRY = R"###(#version 460

layout(location = 0) in vec3 fPos;
layout(location = 1) in vec3 fNorm;
layout(location = 2) in vec3 fTan;
layout(location = 3) in vec2 fUv;
layout(location = 4) in vec4 vPos;

layout(location = 0) out vec4 oPosition;
layout(location = 1) out vec4 oNormal;
layout(location = 2) out vec4 oTangent;
layout(location = 3) out vec4 oTexNormal;
layout(location = 4) out vec4 oDiffuse;
layout(location = 5) out vec4 oAmbient;
layout(location = 6) out vec4 oSpecular;
layout(location = 7) out vec4 oShininess;

layout(location = 0) uniform mat4 MANA_M;
layout(location = 1) uniform mat4 MANA_MVP;

layout(location = 2) uniform int hasTextureNormal;

layout(location = 3) uniform vec4 diffuseColor;
layout(location = 4) uniform vec4 ambientColor;
layout(location = 5) uniform vec4 specularColor;
layout(location = 6) uniform float shininessColor;

layout(location = 7) uniform vec4 emissiveColor;

layout(location = 8) uniform sampler2D diffuse;
layout(location = 9) uniform sampler2D ambient;
layout(location = 10) uniform sampler2D specular;
layout(location = 11) uniform sampler2D shininess;
layout(location = 12) uniform sampler2D emissive;

layout(location = 13) uniform sampler2D normal;

void main() {
    oPosition = vec4(fPos, 1);
    oDiffuse = texture(diffuse, fUv) + diffuseColor;
    oAmbient = texture(ambient, fUv) + ambientColor;
    oSpecular = texture(specular, fUv) + specularColor;
    oShininess.r = texture(shininess, fUv).r + shininessColor;

    mat3 normalMatrix = transpose(inverse(mat3(MANA_M)));
    oNormal = vec4(normalize(normalMatrix * fNorm), 1);
    oTangent = vec4(normalize(normalMatrix * fTan), 1);

    if (hasTextureNormal != 0)
    {
        vec3 texNormal = texture(normal, fUv).xyz;
        texNormal = normalize(texNormal * 2.0 - 1.0);
        oTexNormal = vec4(texNormal, 1);
    }
    else
    {
        oTexNormal = vec4(0, 0, 0, 0);
    }
}
)###";

namespace xengine {
    static inline Vec4f scaleColor(const ColorRGBA &color) {
        return {static_cast<float>(color.r()) / 255,
                static_cast<float>(color.g()) / 255,
                static_cast<float>(color.b()) / 255,
                static_cast<float>(color.a()) / 255};
    }

    PrePass::PrePass(RenderDevice &device)
            : ren(device.getRenderer()) {
        auto &allocator = device.getAllocator();

        vs = ShaderSource(SHADER_VERT_GEOMETRY, "main", VERTEX, GLSL_410);
        fs = ShaderSource(SHADER_FRAG_GEOMETRY, "main", FRAGMENT, GLSL_410);

        vs.preprocess(ShaderInclude::getShaderIncludeCallback(),
                      ShaderInclude::getShaderMacros(GLSL_410));
        fs.preprocess(ShaderInclude::getShaderIncludeCallback(),
                      ShaderInclude::getShaderMacros(GLSL_410));

        shader = allocator.createShaderProgram(vs, fs);

        shader->activate();
        shader->setTexture(8, 0);
        shader->setTexture(9, 1);
        shader->setTexture(10, 2);
        shader->setTexture(11, 3);
        shader->setTexture(12, 4);
        shader->setTexture(13, 5);

        TextureBuffer::Attributes attributes;
        attributes.size = {1, 1};
        defaultTexture = allocator.createTextureBuffer(attributes);
        defaultTexture->upload(ImageRGBA(1, 1, {{0, 0, 0, 0}}));
    }

    void PrePass::update(Scene &scene, GBuffer &buffer) {
      /*  Mat4f model, view, projection;
        view = scene.camera.view();
        projection = scene.camera.projection();

        shader->activate();

        auto &target = buffer.getRenderTarget();

        ren.renderBegin(target, RenderOptions({}, target.getSize(), true));

        std::vector<std::reference_wrapper<TextureBuffer>> textures;
        textures.reserve(6);

        bool firstCommand = true;
        Material shaderMaterial;
        for (auto &command: scene.nodes) {
            textures.clear();

            if (command.material.diffuseTexture) {
                if (firstCommand || shaderMaterial.diffuse != ColorRGBA()) {
                    shaderMaterial.diffuse = ColorRGBA();
                    shader->setVec4(3, Vec4f());
                }
                textures.emplace_back(*command.material.diffuseTexture);
            } else {
                if (firstCommand || shaderMaterial.diffuse != command.material.diffuse) {
                    shaderMaterial.diffuse = command.material.diffuse;
                    shader->setVec4(3, scaleColor(command.material.diffuse));
                }
                textures.emplace_back(*defaultTexture);
            }

            if (command.material.ambientTexture) {
                if (firstCommand || shaderMaterial.ambient != ColorRGBA()) {
                    shaderMaterial.ambient = ColorRGBA();
                    shader->setVec4(4, Vec4f());
                }
                textures.emplace_back(*command.material.ambientTexture);
            } else {
                if (firstCommand || shaderMaterial.ambient != command.material.ambient) {
                    shaderMaterial.ambient = command.material.ambient;
                    shader->setVec4(4, scaleColor(command.material.ambient));
                }
                textures.emplace_back(*defaultTexture);
            }

            if (command.material.specularTexture) {
                if (firstCommand || shaderMaterial.specular != ColorRGBA()) {
                    shaderMaterial.specular = ColorRGBA();
                    shader->setVec4(5, Vec4f());
                }
                textures.emplace_back(*command.material.specularTexture);
            } else {
                if (firstCommand || shaderMaterial.specular != command.material.specular) {
                    shaderMaterial.specular = command.material.specular;
                    shader->setVec4(5, scaleColor(command.material.specular));
                }
                textures.emplace_back(*defaultTexture);
            }

            if (command.material.shininessTexture) {
                if (firstCommand || shaderMaterial.shininess != 0) {
                    shaderMaterial.shininess = 0;
                    shader->setFloat(6, 0);
                }
                textures.emplace_back(*command.material.shininessTexture);
            } else {
                if (firstCommand || shaderMaterial.shininess != command.material.shininess) {
                    shaderMaterial.shininess = command.material.shininess;
                    shader->setFloat(6, command.material.shininess);
                }
                textures.emplace_back(*defaultTexture);
            }

            if (command.material.emissiveTexture) {
                if (firstCommand || shaderMaterial.emissive != ColorRGBA()) {
                    shaderMaterial.emissive = ColorRGBA();
                    shader->setVec4(7, Vec4f());
                }
                textures.emplace_back(*command.material.emissiveTexture);
            } else {
                if (firstCommand || shaderMaterial.emissive != command.material.emissive) {
                    shaderMaterial.emissive = command.material.emissive;
                    shader->setVec4(7, scaleColor(command.material.emissive));
                }
                textures.emplace_back(*defaultTexture);
            }

            if (firstCommand
                || shaderMaterial.normalTexture != command.material.normalTexture) {
                shaderMaterial.normalTexture = command.material.normalTexture;
                shader->setInt(2, (command.material.normalTexture != nullptr));
            }

            if (command.material.normalTexture) {
                textures.emplace_back(*command.material.normalTexture);
            }

            model = command.transform.model();

            shader->setMat4(0, model);
            shader->setMat4(1, projection * view * model);

            RenderCommand c(*shader, *command.mesh);
            c.textures = textures;
            c.properties.enableFaceCulling = true;
            ren.addCommand(c);

            firstCommand = false;
        }

        ren.renderFinish();*/
    }
}