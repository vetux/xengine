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

#ifndef XENGINE_FRAMEGRAPHRENDERER_HPP
#define XENGINE_FRAMEGRAPHRENDERER_HPP

#include "xng/render/scenerenderer.hpp"

#include "xng/render/graph/framegraphpass.hpp"
#include "xng/render/graph/framegraphallocator.hpp"
#include "xng/render/graph/framegraphpipeline.hpp"
#include "xng/render/graph/allocators/framegraphpoolallocator.hpp"

#include "xng/shader/shadercompiler.hpp"
#include "xng/shader/shaderdecompiler.hpp"

#include "xng/gpu/renderdevice.hpp"

namespace xng {
    class XENGINE_EXPORT FrameGraphRenderer : public SceneRenderer {
    public:
        explicit FrameGraphRenderer(RenderTarget &target,
                                    RenderDevice &device,
                                    std::unique_ptr<FrameGraphAllocator> allocator,
                                    ShaderCompiler &shaderCompiler,
                                    ShaderDecompiler &shaderDecompiler);

        void render(const Scene &scene) override;

        void setPipeline(const FrameGraphPipeline &v) {
            pipeline = v;
        }

        void setProperties(const GenericMapString &value) override {
            properties = value;
        }

        GenericMapString &getProperties() override {
            return properties;
        }

        const GenericMapString &getProperties() const override {
            return properties;
        }

    private:
        RenderTarget &target;
        RenderDevice &device;

        std::unique_ptr<FrameGraphAllocator> allocator;

        FrameGraphPipeline pipeline;

        GenericMapString properties;
        GenericMapString blackboard;

        FrameGraph frame;

        ShaderCompiler &shaderCompiler;
        ShaderDecompiler &shaderDecompiler;
    };
}
#endif //XENGINE_FRAMEGRAPHRENDERER_HPP
