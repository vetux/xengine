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

#ifndef XENGINE_MATERIAL_HPP
#define XENGINE_MATERIAL_HPP

#include <variant>

#include "xng/asset/texture.hpp"
#include "xng/asset/shader.hpp"

#include "xng/resource/resourcehandle.hpp"

#include "xng/io/messageable.hpp"

namespace xng {
    enum ShadingModel : int {
        SHADE_PBR, // PBR Shading
        SHADE_PHONG, // Per fragment phong shading
        SHADE_PHONG_GOURAUD, // Per vertex phong shading
        SHADE_PHONG_FLAT // Per polygon phong shading
    };

    struct XENGINE_EXPORT Material : public Resource, public Messageable {
        ~Material() override = default;

        std::unique_ptr<Resource> clone() override {
            return std::make_unique<Material>(*this);
        }

        std::type_index getTypeIndex() override {
            return typeid(Material);
        }

        Messageable &operator<<(const Message &message) override {
            message.value("model", (int &) model, (int) ShadingModel::SHADE_PBR);
            message.value("shader", shader);
            message.value("normal", normal);

            message.value("diffuse", diffuse);
            message.value("ambient", ambient);
            message.value("specular", specular);
            message.value("shininess", shininess);

            message.value("diffuseTexture", diffuseTexture);
            message.value("ambientTexture", ambientTexture);
            message.value("specularTexture", specularTexture);
            message.value("shininessTexture", shininessTexture);

            message.value("albedo", albedo);
            message.value("metallic", metallic);
            message.value("roughness", roughness, 1.0f);
            message.value("ambientOcclusion", ambientOcclusion, 1.0f);

            message.value("albedoTexture", albedoTexture);
            message.value("metallicTexture", metallicTexture);
            message.value("roughnessTexture", roughnessTexture);
            message.value("ambientOcclusionTexture", ambientOcclusionTexture);

            return *this;
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);

            model >> message["model"];
            shader >> message["shader"];
            normal >> message["normal"];

            diffuse >> message["diffuse"];
            ambient >> message["ambient"];
            specular >> message["specular"];
            shininess >> message["shininess"];

            diffuseTexture >> message["diffuseTexture"];
            ambientTexture >> message["ambientTexture"];
            specularTexture >> message["specularTexture"];
            shininessTexture >> message["shininessTexture"];

            albedo >> message["albedo"];
            metallic >> message["metallic"];
            roughness >> message["roughness"];
            ambientOcclusion >> message["ambientOcclusion"];

            albedoTexture >> message["albedoTexture"];
            metallicTexture >> message["metallicTexture"];
            roughnessTexture >> message["roughnessTexture"];
            ambientOcclusionTexture >> message["ambientOcclusionTexture"];

            return message;
        }

        ShadingModel model;

        /**
         * Optional user specified shader.
         *
         * Currently only glsl user shaders are supported.
         *
         * In a deferred pipeline the vertex/geometry shaders would replace the shaders in the gbuffer construction pass
         * and the fragment shader would replace the lighting pass.
         *
         * The vertex/geometry shaders might be run with platform specific fragment shaders and
         * the fragment shader might be run with a platform specific vertex/geometry shader (Deferred Shading).
         *
         * To make shader code compatible across pipelines the shaders must access external resources
         * through the stable shader interface described in xng/render/shaderinterface.hpp
         * by including either "fragment.glsl" or "vertex.glsl" respectively.
         */
        ResourceHandle<Shader> shader;

        /**
         * If assigned the contained normals are sampled otherwise vertex normals are used.
         */
        ResourceHandle<ImageRGBA> normal;

        /**
         * PBR Shading Data
         */
        ColorRGBA albedo{};
        float metallic{};
        float roughness{};
        float ambientOcclusion{};

        ResourceHandle<ImageRGBA> albedoTexture;
        ResourceHandle<ImageRGBA> metallicTexture;
        ResourceHandle<ImageRGBA> roughnessTexture;
        ResourceHandle<ImageRGBA> ambientOcclusionTexture;

        /**
         * Phong Shading Data
         */
        ColorRGBA diffuse{};
        ColorRGBA ambient{};
        ColorRGBA specular{};
        float shininess{};

        ResourceHandle<ImageRGBA> diffuseTexture;
        ResourceHandle<ImageRGBA> ambientTexture;
        ResourceHandle<ImageRGBA> specularTexture;
        ResourceHandle<ImageRGBA> shininessTexture;
    };
}

#endif //XENGINE_MATERIAL_HPP
