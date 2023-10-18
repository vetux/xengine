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

#ifndef XENGINE_SCENE_HPP
#define XENGINE_SCENE_HPP

#include <utility>

#include "camera.hpp"
#include "directionallight.hpp"
#include "pointlight.hpp"
#include "spotlight.hpp"
#include "xng/asset/material.hpp"
#include "xng/asset/skybox.hpp"
#include "xng/asset/shader.hpp"
#include "xng/asset/mesh.hpp"
#include "xng/asset/skinnedmesh.hpp"

#include "xng/types/genericmap.hpp"

#include "xng/resource/resourcehandle.hpp"

namespace xng {
    /**
     * The runtime scene render data.
     */
    struct XENGINE_EXPORT Scene {
        struct XENGINE_EXPORT Property {
            virtual std::type_index getType() = 0;

            template<typename T>
            T &get() {
                return dynamic_cast<T &>(*this);
            }

            template<typename T>
            const T &get() const {
                return dynamic_cast<T &>(*this);
            }
        };

        struct Node;

        struct XENGINE_EXPORT Node {
            template<typename T>
            T &getProperty() {
                return dynamic_cast< T &>(*properties.at(typeid(T)));
            }

            template<typename T>
            const T &getProperty() const {
                return dynamic_cast<const T &>(*properties.at(typeid(T)));
            }

            template<typename T>
            void addProperty(const T &property) {
                properties[typeid(T)] = std::make_shared<T>(property);
            }

            std::vector<Node> findAll(std::vector<std::type_index> propertyTypes) const {
                std::vector<Node> ret;
                for (auto &c: childNodes) {
                    auto nodes = c.findAll(propertyTypes);
                    ret.insert(ret.end(), nodes.begin(), nodes.end());
                }
                for (auto &prop: propertyTypes) {
                    if (properties.find(prop) != properties.end()) {
                        ret.emplace_back(*this);
                        break;
                    }
                }
                return ret;
            }

            std::vector<Node> childNodes;
            std::map<std::type_index, std::shared_ptr<Property>> properties;
        };

        struct ShadowProperty : public Property {
            std::type_index getType() override {
                return typeid(ShadowProperty);
            }

            bool castShadows = false;
            bool receiveShadows = false;
        };

        struct MeshProperty : public Property {
            std::type_index getType() override {
                return typeid(MeshProperty);
            }

            Transform transform;
            ResourceHandle<Mesh> mesh;
        };

        struct SkinnedMeshProperty : public Property {
            std::type_index getType() override {
                return typeid(SkinnedMeshProperty);
            }

            Transform transform;
            ResourceHandle<SkinnedMesh> mesh;
        };

        struct MaterialProperty : public Property {
            std::type_index getType() override {
                return typeid(MaterialProperty);
            }

            std::map<size_t, ResourceHandle<Material>> materials;
        };

        struct BoneTransformsProperty : public Property {
            std::type_index getType() override {
                return typeid(BoneTransformsProperty);
            }

            std::map<std::string, Mat4f> boneTransforms; // Optional dynamic bone transform values which override the values in SkinnedMesh.rig
        };

        struct LightingProperty : public Property {
            std::type_index getType() override {
                return typeid(LightingProperty);
            }

            std::vector<DirectionalLight> directionalLights;
            std::vector<PointLight> pointLights;
            std::vector<SpotLight> spotLights;
        };

        struct SkyboxProperty : public Property {
            std::type_index getType() override {
                return typeid(SkyboxProperty);
            }

            Skybox skybox;
        };

        struct CameraProperty : public Property {
            std::type_index getType() override {
                return typeid(CameraProperty);
            }

            Transform cameraTransform;
            Camera camera;
        };

        Node rootNode;
    };
}

#endif //XENGINE_SCENE_HPP
