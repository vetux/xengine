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

#ifndef XENGINE_CANVASCOMPONENT_HPP
#define XENGINE_CANVASCOMPONENT_HPP

#include "xng/io/messageable.hpp"
#include "xng/ecs/component.hpp"

namespace xng {
    /*
     * A canvas renders CanvasTransformComponents to the screen using Renderer2D
     */
    struct XENGINE_EXPORT CanvasComponent : public Component {
        Vec2f cameraPosition;

        bool overrideViewport; // If true the specified viewport values are used, the values are generated based on screen size.
        Vec2i viewportSize;
        Vec2i viewportOffset;

        Vec2f projectionSize; // The size of the projection or zero to use screensize as projection size

        bool fitViewport; // If true the viewport is fitted to the next smallest screen size with aspect ratio matching projection size

        bool clear = false; // Wheter or not to clear the viewport when rendering this canvas
        ColorRGBA clearColor = ColorRGBA::black();
        int layer; // The sorting layer of this canvas relative to other canvases

        Messageable &operator<<(const Message &message) override {
            message.value("cameraPosition", cameraPosition);
            message.value("viewportSize", viewportSize);
            message.value("viewportOffset", viewportOffset);
            message.value("overrideViewport", overrideViewport);
            message.value("projectionSize", projectionSize);
            message.value("fitViewport", fitViewport);
            message.value("clear", clear);
            message.value("clearColor", clearColor);
            message.value("layer", layer);
            return Component::operator<<(message);
        }

        Message &operator>>(Message &message) const override {
            message = Message(Message::DICTIONARY);
            cameraPosition >> message["cameraPosition"];
            projectionSize >> message["projectionSize"];
            viewportSize >> message["viewportSize"];
            viewportOffset >> message["viewportOffset"];
            fitViewport >> message["fitViewport"];
            overrideViewport >> message["overrideViewport"];
            clear >> message["clear"];
            clearColor >> message["clearColor"];
            layer >> message["layer"];
            return Component::operator>>(message);
        }

        std::type_index getType() const override {
            return typeid(CanvasComponent);
        }

        float getViewportScale(const Vec2i &screenSize) const {
            if (projectionSize.magnitude() == 0)
                return 1;
            float scale;
            if (screenSize.x > screenSize.y) {
                scale = screenSize.y / projectionSize.y;
                if (scale > screenSize.x / projectionSize.x) {
                    scale = screenSize.x / projectionSize.x;
                }
            } else {
                scale = screenSize.x / projectionSize.x;
                if (scale > screenSize.y / projectionSize.y) {
                    scale = screenSize.y / projectionSize.y;
                }
            }
            return scale;
        }

        Vec2i getViewportSize(const Vec2i &screenSize) const {
            if (projectionSize.magnitude() > 0) {
                if (fitViewport) {
                    float scale = getViewportScale(screenSize);
                    Vec2f ret = projectionSize * scale;
                    if (ret.x <= 0) {
                        ret.x = 1;
                    }
                    if (ret.y <= 0) {
                        ret.y = 1;
                    }
                    return ret.convert<int>();
                } else {
                    return projectionSize.convert<int>();
                }
            } else {
                return screenSize;
            }
        }

        Vec2i getViewportOffset(const Vec2i &screenSize) const {
            return (screenSize - getViewportSize(screenSize)) / 2;
        }

        std::pair<Vec2i, Vec2i> getViewport(const Vec2i &screenSize) const {
            if (overrideViewport) {
                return {viewportSize, viewportOffset};
            } else {
                return {getViewportSize(screenSize), getViewportOffset(screenSize)};
            }
        }
    };
}

#endif //XENGINE_CANVASCOMPONENT_HPP
