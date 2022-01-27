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

#include "render/deferred/deferredpipeline.hpp"

#include <algorithm>
#include <utility>

namespace xengine {
    DeferredPipeline::DeferredPipeline(RenderDevice &device,
                                       AssetRenderManager &assetRenderManager,
                                       GBuffer &geometryBuffer,
                                       PassChain &chain,
                                       Compositor &compositor)
            : geometryBuffer(geometryBuffer),
              assetRenderManager(assetRenderManager),
              chain(chain),
              compositor(compositor) {}

    DeferredPipeline::~DeferredPipeline() = default;

    void DeferredPipeline::render(RenderTarget &target,
                                  Scene &scene) {
        geometryBuffer.update(scene, assetRenderManager);
        for (auto &pass: chain.passes) {
            pass->render(geometryBuffer, scene, assetRenderManager);
        }
        compositor.present(target, chain);
    }

    GBuffer &DeferredPipeline::getGeometryBuffer() {
        return geometryBuffer;
    }
}