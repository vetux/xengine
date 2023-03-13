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

#include <sstream>

#include "xng/ecs/systems/canvasrendersystem.hpp"

#include "xng/ecs/components/transformcomponent.hpp"
#include "xng/ecs/components/recttransformcomponent.hpp"
#include "xng/ecs/components/canvascomponent.hpp"
#include "xng/ecs/components/textcomponent.hpp"

#include "xng/gui/canvasscaler.hpp"
#include "xng/gui/layoutengine.hpp"

namespace xng {
    CanvasRenderSystem::CanvasRenderSystem(Renderer2D &renderer2D,
                                           RenderTarget &target,
                                           FontDriver &fontDriver,
                                           bool drawDebugGeometry)
            : ren2d(renderer2D), target(target), fontDriver(fontDriver), drawDebugGeometry(drawDebugGeometry) {}

    void CanvasRenderSystem::start(EntityScene &scene, EventBus &eventBus) {
        scene.addListener(*this);
        for (auto &pair: scene.getPool<SpriteComponent>()) {
            if (!pair.second.sprite.assigned())
                continue;
            if (spriteTextureHandles.find(pair.first) == spriteTextureHandles.end()) {
                createTexture(pair.first, pair.second);
            }
        }

        for (auto &pair : scene.getPool<TextComponent>()){
            if (textTextureHandles.find(pair.first) == textTextureHandles.end()){
                createText(pair.first, pair.second);
            }
        }
    }

    void CanvasRenderSystem::stop(EntityScene &scene, EventBus &eventBus) {
        scene.removeListener(*this);
        for (auto &pair: spriteTextureHandles) {
            ren2d.destroyTexture(pair.second);
        }
        spriteTextureHandles.clear();
        textRenderers.clear();
        renderedTexts.clear();
        fonts.clear();
    }

    static std::vector<EntityHandle> getDrawingOrderRecursive(EntityHandle rectComp,
                                                              std::map<EntityHandle, std::set<int>> rects) {
        std::vector<EntityHandle> ret;
        ret.emplace_back(rectComp);
        for (auto &id: rects[rectComp]) {
            auto order = getDrawingOrderRecursive(EntityHandle(id), rects);
            ret.insert(ret.end(), order.begin(), order.end());
        }
        return ret;
    }

    static Vec2f clampSize(const Vec2f &size, const Vec2f &limit) {
        if (size.x > limit.x && size.y > limit.y) {
            auto diffX = size.x - limit.x;
            auto diffY = size.y - limit.y;
            if (diffX > diffY) {
                return {limit.x, size.y * (limit.x / size.x)};
            } else {
                return {size.x * (limit.y / size.y), limit.y};
            }
        } else if (size.x > limit.x) {
            return {limit.x, size.y * (limit.x / size.x)};
        } else if (size.y > limit.y) {
            return {size.x * (limit.y / size.y), limit.y};
        } else {
            return size;
        }
    }

    void CanvasRenderSystem::update(DeltaTime deltaTime, EntityScene &scene, EventBus &eventBus) {
        //TODO: Design a better text scaling method which doesnt produce pixelated characters
        std::map<int, std::set<EntityHandle>> canvases;

        for (auto &p: scene.getPool<CanvasComponent>()) {
            if (!p.second.enabled)
                continue;
            canvases[p.second.layer].insert(p.first);
        }

        std::map<int, RectTransform> transforms;
        std::map<int, int> transformParents;
        std::map<EntityHandle, std::set<int>> parentsChildrenMapping;

        for (auto &comp: scene.getPool<RectTransformComponent>()) {
            if (scene.checkComponent<CanvasComponent>(comp.first)) {
                auto &canvas = scene.getComponent<CanvasComponent>(comp.first);
                transforms[comp.first.id] = comp.second.rectTransform;
                transforms[comp.first.id].size = target.getDescription().size.convert<float>();
            } else if (!comp.second.parent.empty()
                       && scene.entityNameExists(comp.second.parent)) {
                transforms[comp.first.id] = comp.second.rectTransform;

                auto parentEnt = scene.getEntityByName(comp.second.parent);

                transformParents[comp.first.id] = parentEnt.id;

                parentsChildrenMapping[parentEnt].insert(comp.first.id);
            }
        }

        for (auto &pair: canvases) {
            for (auto &canvasHandle: pair.second) {
                auto &canvasComponent = scene.getComponent<CanvasComponent>(canvasHandle);

                std::map<int, RectTransform> absoluteTransforms;
                switch (canvasComponent.scaleMode) {
                    case SCALE_REFERENCE_RESOLUTION:
                        absoluteTransforms = LayoutEngine::getAbsoluteReferenceScaled(transforms,
                                                                                      transformParents,
                                                                                      canvasComponent.referenceResolution,
                                                                                      target.getDescription().size.convert<float>(),
                                                                                      canvasComponent.referenceFitWidth);
                        break;
                    default:
                        absoluteTransforms = LayoutEngine::getAbsolute(transforms, transformParents);
                        break;
                }

                auto &transformIds = parentsChildrenMapping[canvasHandle];

                ren2d.renderBegin(target,
                                  canvasComponent.clear,
                                  canvasComponent.clearColor,
                                  {},
                                  target.getDescription().size,
                                  canvasComponent.cameraPosition);

                for (auto &id: transformIds) {
                    auto order = getDrawingOrderRecursive(EntityHandle(EntityHandle(id)), parentsChildrenMapping);
                    for (auto &handle: order) {
                        auto transform = absoluteTransforms.at(handle.id);

                        if (scene.checkComponent<SpriteComponent>(handle)) {
                            auto &comp = scene.getComponent<SpriteComponent>(handle);
                            if (comp.enabled
                                && comp.sprite.assigned()) {
                                ren2d.draw(Rectf(comp.sprite.get().offset),
                                           Rectf(transform.position, transform.size),
                                           spriteTextureHandles.at(handle),
                                           transform.center,
                                           transform.rotation,
                                           comp.mix,
                                           comp.mixAlpha,
                                           comp.mixColor);
                                if (drawDebugGeometry) {
                                    ren2d.draw(Rectf(transform.position, transform.size),
                                               ColorRGBA::yellow(),
                                               false,
                                               transform.center,
                                               transform.rotation);
                                }
                            }
                        } else if (scene.checkComponent<TextComponent>(handle)) {
                            auto debugTransform = transform;
                            auto &comp = scene.getComponent<TextComponent>(handle);

                            auto &text = renderedTexts.at(handle);

                            transform.position += transform.size / 2;

                            auto imgSize = text.getImage().getSize().convert<float>();

                            auto textSize = imgSize * transform.size.x / transforms.at(handle.id).size.x;

                            Rectf srcRect({}, imgSize);

                            auto diffX = transform.size.x - textSize.x;
                            if (diffX > 0) {
                                transform.size.x = textSize.x;
                            } else {
                                float diffSrcX = diffX * (imgSize.x / textSize.x);
                                srcRect.dimensions.x += diffSrcX;
                                srcRect.position.x += -diffSrcX / 2;
                            }

                            auto diffY = transform.size.y - textSize.y;
                            if (diffY > 0) {
                                transform.size.y = textSize.y;
                            } else {
                                float diffSrcY = diffY * (imgSize.y / textSize.y);
                                srcRect.dimensions.y += diffSrcY;
                                srcRect.position.y += -diffSrcY / 2;
                            }

                            transform.position -= transform.size / 2;

                            ren2d.draw(srcRect,
                                       Rectf(transform.position, transform.size),
                                       textTextureHandles.at(handle),
                                       imgSize / 2,
                                       transform.rotation,
                                       comp.textColor);
                            if (drawDebugGeometry) {
                                ren2d.draw(Rectf(debugTransform.position, debugTransform.size),
                                           ColorRGBA::yellow(),
                                           false,
                                           debugTransform.center,
                                           debugTransform.rotation);
                                ren2d.draw(Rectf(transform.position, transform.size),
                                           ColorRGBA::red(),
                                           false,
                                           transform.center,
                                           transform.rotation);
                            }
                        } else {
                            if (drawDebugGeometry) {
                                ren2d.draw(Rectf(transform.position, transform.size),
                                           ColorRGBA::fuchsia(),
                                           false,
                                           transform.center,
                                           transform.rotation);
                            }
                        }
                    }
                }

                ren2d.renderPresent();
            }
        }
    }

    void CanvasRenderSystem::onComponentCreate(const EntityHandle &entity, const Component &component) {
        if (component.getType() == typeid(SpriteComponent)) {
            const auto &t = dynamic_cast<const SpriteComponent &>(component);
            createTexture(entity, t);
        } else if (component.getType() == typeid(TextComponent)){
            const auto &t = dynamic_cast<const TextComponent &>(component);
            createText(entity, t);
        }
    }

    void CanvasRenderSystem::onComponentDestroy(const EntityHandle &entity, const Component &component) {
        if (component.getType() == typeid(SpriteComponent)) {
            if (spriteTextureHandles.find(entity) != spriteTextureHandles.end()) {
                ren2d.destroyTexture(spriteTextureHandles.at(entity));
                spriteTextureHandles.erase(entity);
            }
        } else if (component.getType() == typeid(TextComponent)) {
            renderedTexts.erase(entity);
            if (textTextureHandles.find(entity) != textTextureHandles.end()) {
                ren2d.destroyTexture(textTextureHandles.at(entity));
                textTextureHandles.erase(entity);
            }
        }
    }

    void CanvasRenderSystem::onComponentUpdate(const EntityHandle &entity,
                                               const Component &oldComponent,
                                               const Component &newComponent) {
        if (oldComponent.getType() == typeid(SpriteComponent)) {
            auto &os = dynamic_cast<const SpriteComponent &>(oldComponent);
            auto &ns = dynamic_cast<const SpriteComponent &>(newComponent);
            if (os.sprite != ns.sprite) {
                if (spriteTextureHandles.find(entity) != spriteTextureHandles.end()) {
                    ren2d.destroyTexture(spriteTextureHandles.at(entity));
                    spriteTextureHandles.erase(entity);
                }
                createTexture(entity, ns);
            }
        } else if (oldComponent.getType() == typeid(TextComponent)) {
            auto &os = dynamic_cast<const TextComponent &>(oldComponent);
            auto &ns = dynamic_cast<const TextComponent &>(newComponent);
            if (os != ns) {
                renderedTexts.erase(entity);
                if (textTextureHandles.find(entity) != textTextureHandles.end()) {
                    ren2d.destroyTexture(textTextureHandles.at(entity));
                    textTextureHandles.erase(entity);
                }
                createText(entity, ns);
            }
        }
    }

    void CanvasRenderSystem::createTexture(const EntityHandle &ent, const SpriteComponent &t) {
        if (t.sprite.assigned()) {
            Vec2i dimensions = t.sprite.get().offset.dimensions;
            if (dimensions.x * dimensions.y == 0) {
                dimensions = t.sprite.get().image.get().getSize();
            }
            TextureBufferDesc desc;
            desc.size = dimensions;
            auto &img = t.sprite.get().image.get();
            if (img.getSize() != dimensions) {
                // Upload a slice of an image
                auto slice = img.slice(t.sprite.get().offset);
                spriteTextureHandles[ent] = ren2d.createTexture(slice);
            } else {
                // Upload the whole image
                spriteTextureHandles[ent] = ren2d.createTexture(img);
            }
        }
    }

    void CanvasRenderSystem::createText(const EntityHandle &ent, const TextComponent &comp) {
        if (!comp.text.empty()
            && comp.font.assigned()) {
            auto pointSize = comp.pixelSize;

            auto it = fonts.find(comp.font.getUri());
            if (it == fonts.end()) {
                std::string str = std::string(comp.font.get().bytes.begin(), comp.font.get().bytes.end());
                auto stream = std::stringstream(str);
                fonts[comp.font.getUri()] = fontDriver.createFont(stream);
            }
            auto rIt = textRenderers.find(ent);
            if (rIt == textRenderers.end()) {
                fonts.at(comp.font.getUri())->setPixelSize(pointSize);
                textRenderers.insert(std::move(std::make_pair(ent, TextRenderer(*fonts[comp.font.getUri()], ren2d))));
            }

            auto text = textRenderers.at(ent).render(comp.text, TextLayout{
                    comp.lineHeight == 0 ? pointSize.y : static_cast<int>(static_cast<float>(comp.lineHeight)),
                    static_cast<int>(static_cast<float>(comp.lineWidth)),
                    static_cast<int>(static_cast<float>(comp.lineSpacing)),
                    comp.alignment});
            renderedTexts.insert(std::make_pair(ent, text));
            textTextureHandles[ent] = ren2d.createTexture(text.getImage());
        }
    }

}