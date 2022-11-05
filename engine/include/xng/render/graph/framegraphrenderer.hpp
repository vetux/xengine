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

#include "xng/render/scenerenderer.hpp"

#include "framegraphpool.hpp"
#include "framegraphpass.hpp"
#include "framegraphlayout.hpp"
#include "framegraphallocator.hpp"
#include "xng/render/graph/allocators/framegraphpoolallocator.hpp"

#include "xng/gpu/renderdevice.hpp"

namespace xng {
    class XENGINE_EXPORT FrameGraphRenderer : public SceneRenderer {
    public:
        explicit FrameGraphRenderer(RenderTarget &target, std::unique_ptr<FrameGraphAllocator> allocator);

        void render(const Scene &scene) override;

        void setLayout(const FrameGraphLayout &v) {
            layout = v;
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

        std::type_index getType() override { return typeid(FrameGraphRenderer); }

    private:
        RenderTarget &target;
        std::unique_ptr<FrameGraphAllocator> allocator;

        FrameGraphLayout layout;

        GenericMapString properties;
        GenericMapString blackboard;
    };
}
#endif //XENGINE_FRAMEGRAPHRENDERER_HPP
