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
                                       AssetManager &assetManager,
                                       std::vector<std::unique_ptr<RenderPass>> passes)
            : prePass(std::make_unique<PrePass>(device)),
              gBuffer(std::make_unique<GBuffer>(device)),
              assetRenderManager(std::make_unique<AssetRenderManager>(assetManager, device.getAllocator())),
              compositor(std::make_unique<Compositor>(device)),
              passes(std::move(passes)) {}

    DeferredPipeline::DeferredPipeline(RenderDevice &device,
                                       std::unique_ptr<PrePass> prePass,
                                       std::unique_ptr<GBuffer> gBuffer,
                                       std::unique_ptr<Compositor> compositor,
                                       std::unique_ptr<AssetRenderManager> assetRenderManager,
                                       std::vector<std::unique_ptr<RenderPass>> passes)
            : prePass(std::move(prePass)),
              gBuffer(std::move(gBuffer)),
              compositor(std::move(compositor)),
              assetRenderManager(std::move(assetRenderManager)),
              passes(std::move(passes)) {}

    DeferredPipeline::~DeferredPipeline() = default;

    void DeferredPipeline::render(RenderTarget &target,
                                  Scene &scene) {
        prePass->update(scene, *gBuffer, *assetRenderManager);
        for (auto &pass: passes) {
            pass->render(*gBuffer, scene, *assetRenderManager);
        }
        compositor->present(target, passes);
    }
}