/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2021  Julian Zampiccoli
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

#include "ecs/entityscene.hpp"
#include "ecs/components.hpp"
#include "ecs/entity.hpp"

namespace xng {

#define DESERIALIZE_COMPONENT(NAME, TYPE) \
                { \
    NAME,\
    [](EntityScene &scene, EntityHandle ent, const Message &msg) {\
    TYPE comp;\
    comp << msg;\
    scene.createComponent(ent, comp);\
    }\
    },\

    static std::map<std::string, EntityScene::ComponentDeserializer> componentDeserializers = {
            DESERIALIZE_COMPONENT("audio_listener", AudioListenerComponent)
            DESERIALIZE_COMPONENT("audio_source", AudioSourceComponent)
            DESERIALIZE_COMPONENT("button", ButtonComponent)
            DESERIALIZE_COMPONENT("camera", CameraComponent)
            DESERIALIZE_COMPONENT("canvas", CanvasComponent)
            DESERIALIZE_COMPONENT("rigidbody", RigidBodyComponent)
            DESERIALIZE_COMPONENT("light", LightComponent)
            DESERIALIZE_COMPONENT("render", MeshRenderComponent)
            DESERIALIZE_COMPONENT("particle", ParticleComponent)
            DESERIALIZE_COMPONENT("canvas_transform", CanvasTransformComponent)
            DESERIALIZE_COMPONENT("skybox", SkyboxComponent)
            DESERIALIZE_COMPONENT("sprite_animation", SpriteAnimationComponent)
            DESERIALIZE_COMPONENT("sprite", SpriteComponent)
            DESERIALIZE_COMPONENT("text", TextComponent)
            DESERIALIZE_COMPONENT("transform", TransformComponent)
    };

    const std::map<std::string, EntityScene::ComponentDeserializer> &EntityScene::getComponentDeserializers() {
        return componentDeserializers;
    }

    void EntityScene::addComponentDeserializer(const std::string &componentName,
                                               EntityScene::ComponentDeserializer deserializer) {
        if (componentDeserializers.find(componentName) != componentDeserializers.end()) {
            throw std::runtime_error("Component deserializer for " + componentName + " already exists");
        }
        componentDeserializers[componentName] = deserializer;
    }

#define SERIALIZE_COMPONENT(NAME, TYPE) \
    if (components.check<TYPE>(entity)) {\
        Message component; \
        components.lookup<TYPE>(entity) >> component; \
        cmap[NAME] = component; \
    }

    void EntityScene::serializeEntity(const EntityHandle &entity, Message &message) const {
        message = Message(Message::DICTIONARY);
        auto it = entityNamesReverse.find(entity);
        if (it != entityNamesReverse.end()) {
            message["name"] = it->second;
        }
        auto cmap = std::map<std::string, Message>();

        if (components.check<AudioSourceComponent>(entity)) {
            Message component;
            components.lookup<AudioSourceComponent>(entity) >> component;
            cmap["test"] = component;
        }

        SERIALIZE_COMPONENT("audio_listener", AudioListenerComponent)
        SERIALIZE_COMPONENT("audio_source", AudioSourceComponent)
        SERIALIZE_COMPONENT("button", ButtonComponent)
        SERIALIZE_COMPONENT("camera", CameraComponent)
        SERIALIZE_COMPONENT("canvas", CanvasComponent)
        SERIALIZE_COMPONENT("rigidbody", RigidBodyComponent)
        SERIALIZE_COMPONENT("light", LightComponent)
        SERIALIZE_COMPONENT("render", MeshRenderComponent)
        SERIALIZE_COMPONENT("particle", ParticleComponent)
        SERIALIZE_COMPONENT("canvas_transform", CanvasTransformComponent)
        SERIALIZE_COMPONENT("skybox", SkyboxComponent)
        SERIALIZE_COMPONENT("sprite_animation", SpriteAnimationComponent)
        SERIALIZE_COMPONENT("sprite", SpriteComponent)
        SERIALIZE_COMPONENT("text", TextComponent)
        SERIALIZE_COMPONENT("transform", TransformComponent)

        message["components"] = cmap;
    }

    void EntityScene::deserializeEntity(const Message &message) {
        EntityHandle entity;
        if (message.has("name")) {
            entity = create(message.at("name"));
        } else {
            entity = create();
        }
        for (auto &c: message.value("components", std::map<std::string, Message>())) {
            auto &deser = getComponentDeserializers();
            auto it = deser.find(c.first);
            if (it == deser.end()) {
                throw std::runtime_error("Component deserializer for name " + c.first + " not found");
            } else {
                it->second(*this, entity, c.second);
            }
        }
    }

    Entity EntityScene::createEntity() {
        return {create(), *this};
    }

    Entity EntityScene::createEntity(const std::string &name) {
        return {create(name), *this};
    }

    void EntityScene::destroyEntity(const Entity &entity) {
        destroy(entity.getHandle());
    }

    Entity EntityScene::getEntity(const std::string &name) {
        return {getEntityByName(name), *this};
    }

    std::unique_ptr<Resource> EntityScene::clone() {
        return std::make_unique<EntityScene>(*this);
    }

    std::type_index EntityScene::getTypeIndex() {
        return typeid(EntityScene);
    }
}