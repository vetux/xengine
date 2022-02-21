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

#ifndef XENGINE_FORWARDPIPELINE_HPP
#define XENGINE_FORWARDPIPELINE_HPP

#include "render/pipeline.hpp"

#include "render/forward/forwardrenderer.hpp"

namespace xengine {
    class XENGINE_EXPORT ForwardPipeline : public Pipeline {
    public:
        ForwardPipeline() = default;

        explicit ForwardPipeline(RenderDevice &device)
                : ren(&device.getRenderer()) {}

        void render(RenderTarget &target, Scene &scene) override {
            if (ren == nullptr)
                throw std::runtime_error("Forward pipeline not initialized");
            ForwardRenderer::renderScene(*ren, target, scene);
        }

    private:
        Renderer *ren = nullptr;
    };
}
#endif //XENGINE_FORWARDPIPELINE_HPP
