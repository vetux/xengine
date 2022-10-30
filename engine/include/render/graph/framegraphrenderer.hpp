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

#ifndef XENGINE_FRAMEGRAPHRENDERER_HPP
#define XENGINE_FRAMEGRAPHRENDERER_HPP

#include "render/scenerenderer.hpp"

#include "render/graph/framegraphpool.hpp"
#include "render/graph/framegraphpass.hpp"
#include "render/graph/framegraphlayout.hpp"
#include "render/graph/framegraphallocator.hpp"
#include "render/graph/allocators/framegraphpoolallocator.hpp"

#include "gpu/renderdevice.hpp"

namespace xng {
    class XENGINE_EXPORT FrameGraphRenderer : public SceneRenderer {
    public:
        explicit FrameGraphRenderer(std::unique_ptr<FrameGraphAllocator> allocator);

        void render(RenderTarget &target, const Scene &scene) override;

        void setLayout(const FrameGraphLayout &v) {
            layout = v;
        }

        void setProperties(const GenericMapString &value) {
            properties = value;
        }

        GenericMapString &getProperties() {
            return properties;
        }

        const GenericMapString &getProperties() const {
            return properties;
        }

        void setRenderResolution(Vec2i res) override;

        void setRenderSamples(int samples) override;

        std::type_index getType() override { return typeid(FrameGraphRenderer); }

    private:
        std::unique_ptr<FrameGraphAllocator> allocator;

        FrameGraphLayout layout;

        GenericMapString properties;
        GenericMapString blackboard;
    };
}
#endif //XENGINE_FRAMEGRAPHRENDERER_HPP
