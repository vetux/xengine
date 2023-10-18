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

#include <algorithm>
#include <filesystem>

#include "xng/ecs/systems/meshrendersystem.hpp"
#include "xng/ecs/components.hpp"
#include "xng/types/time.hpp"

namespace xng {
    MeshRenderSystem::MeshRenderSystem(SceneRenderer &pipeline)
            : pipeline(pipeline) {
    }

    MeshRenderSystem::~MeshRenderSystem() = default;

    void MeshRenderSystem::start(EntityScene &entityManager, EventBus &eventBus) {}

    void MeshRenderSystem::stop(EntityScene &entityManager, EventBus &eventBus) {}

    void MeshRenderSystem::update(DeltaTime deltaTime, EntityScene &entScene, EventBus &eventBus) {
        Scene scene = {};

        polyCount = 0;

        // Get objects
        for (auto &pair: entScene.getPool<SkinnedMeshComponent>()) {
            // TODO: Culling
            // TODO: Z Sort transparent meshes based on distance of transform to camera
            //TODO: Change transform walking / scene creation to allow model matrix caching

            auto &transform = entScene.getComponent<TransformComponent>(pair.first);
            if (!transform.enabled)
                continue;

            auto &meshComponent = pair.second;
            if (!meshComponent.enabled
                || !meshComponent.mesh.isLoaded()
                || !meshComponent.mesh.get().isLoaded())
                continue;

            Scene::Node node;

            Scene::SkinnedMeshProperty meshProperty;
            meshProperty.mesh = pair.second.mesh;
            meshProperty.transform = TransformComponent::walkHierarchy(transform, entScene);
            node.addProperty(meshProperty);

            Scene::ShadowProperty shadowProperty;
            shadowProperty.castShadows = pair.second.castShadows;
            shadowProperty.receiveShadows = pair.second.receiveShadows;
            node.addProperty(shadowProperty);

            if (entScene.checkComponent<MaterialComponent>(pair.first)) {
                auto &comp = entScene.getComponent<MaterialComponent>(pair.first);
                bool skip = false;
                for (auto &mPair: comp.materials) {
                    if (!mPair.second.isLoaded() || !mPair.second.get().isLoaded()) {
                        skip = true;
                        break;
                    }
                }
                if (skip) {
                    continue;
                }
                Scene::MaterialProperty materialProperty;
                materialProperty.materials = comp.materials;
                node.addProperty(materialProperty);
            }

            if (entScene.checkComponent<RigAnimationComponent>(pair.first)) {
                Scene::BoneTransformsProperty boneTransformsProperty;
                boneTransformsProperty.boneTransforms = entScene.getComponent<RigAnimationComponent>(pair.first).boneTransforms;
                node.addProperty(boneTransformsProperty);
            }

            polyCount += meshComponent.mesh.get().polyCount();
            for (auto &mesh: meshComponent.mesh.get().subMeshes) {
                polyCount += mesh.polyCount();
            }

            scene.rootNode.childNodes.emplace_back(node);
        }

        // Get skybox
        for (auto &pair: entScene.getPool<SkyboxComponent>()) {
            auto &comp = pair.second;
            auto boxProp = Scene::SkyboxProperty();
            boxProp.skybox = comp.skybox;
            scene.rootNode.addProperty(boxProp);
        }

        // Get Camera
        for (auto &pair: entScene.getPool<CameraComponent>()) {
            auto &tcomp = entScene.getComponent<TransformComponent>(pair.first);

            if (!tcomp.enabled)
                continue;

            auto &comp = pair.second;

            auto camProp = Scene::CameraProperty();

            camProp.camera = comp.camera;
            camProp.cameraTransform = TransformComponent::walkHierarchy(tcomp, entScene);

            scene.rootNode.addProperty(camProp);

            break;
        }

        // Get lights

        Scene::LightingProperty lightProp;

        for (auto &pair: entScene.getPool<LightComponent>()) {
            auto lightComponent = pair.second;
            auto &tcomp = entScene.getPool<TransformComponent>().lookup(pair.first);

            if (!lightComponent.enabled)
                continue;

            if (!tcomp.enabled)
                continue;


            switch (lightComponent.type) {
                case LightComponent::LIGHT_DIRECTIONAL: {
                    auto tmp = std::get<DirectionalLight>(lightComponent.light);
                    tmp.transform = tcomp.transform;
                    lightProp.directionalLights.emplace_back(tmp);
                    break;
                }
                case LightComponent::LIGHT_POINT: {
                    auto tmp = std::get<PointLight>(lightComponent.light);
                    tmp.transform = tcomp.transform;
                    lightProp.pointLights.emplace_back(tmp);
                    break;
                }
                case LightComponent::LIGHT_SPOT: {
                    auto tmp = std::get<SpotLight>(lightComponent.light);
                    tmp.transform = tcomp.transform;
                    lightProp.spotLights.emplace_back(tmp);
                    break;
                }
            }
        }

        scene.rootNode.addProperty(lightProp);

        // Render
        pipeline.render(scene);
    }

    SceneRenderer &MeshRenderSystem::getPipeline() {
        return pipeline;
    }
}