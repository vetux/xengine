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

#ifndef XENGINE_RENDERPASS_HPP
#define XENGINE_RENDERPASS_HPP

#include "render/graph/renderpassresources.hpp"
#include "render/graph/framegraphbuilder.hpp"
#include "render/graph/framegraphblackboard.hpp"

#include "graphics/renderdevice.hpp"

namespace xengine {
    class XENGINE_EXPORT RenderPass {
    public:
        virtual ~RenderPass() = default;

        virtual void setup(FrameGraphBuilder &builder) = 0;

        virtual void execute(RenderPassResources &resources, RenderDevice &ren, FrameGraphBlackboard &board) = 0;
    };
}

#endif //XENGINE_RENDERPASS_HPP
