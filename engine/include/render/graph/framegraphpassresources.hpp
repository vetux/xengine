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

#ifndef XENGINE_FRAMEGRAPHPASSRESOURCES_HPP
#define XENGINE_FRAMEGRAPHPASSRESOURCES_HPP

#include <map>
#include <memory>
#include <utility>

#include "render/graph/framegraphresource.hpp"
#include "render/graph/gbuffer.hpp"

#include "graphics/texturebuffer.hpp"
#include "graphics/vertexbuffer.hpp"
#include "graphics/rendertarget.hpp"
#include "graphics/shaderprogram.hpp"

namespace xng {
    class XENGINE_EXPORT FrameGraphPassResources {
    public:
        explicit FrameGraphPassResources(std::map<FrameGraphResource, RenderObject *> objects)
                : objects(std::move(objects)) {}

        VertexBuffer &getMeshBuffer(FrameGraphResource resource) {
            return dynamic_cast<VertexBuffer &>(*objects.at(resource));
        }

        TextureBuffer &getTextureBuffer(FrameGraphResource resource) {
            return dynamic_cast<TextureBuffer &>(*objects.at(resource));
        }

        RenderTarget &getRenderTarget(FrameGraphResource resource) {
            return dynamic_cast<RenderTarget &>(*objects.at(resource));
        }

        ShaderProgram &getShader(FrameGraphResource resource) {
            return dynamic_cast<ShaderProgram &>(*objects.at(resource));
        }

    private:
        std::map<FrameGraphResource, RenderObject *> objects;
    };
}
#endif //XENGINE_FRAMEGRAPHPASSRESOURCES_HPP
