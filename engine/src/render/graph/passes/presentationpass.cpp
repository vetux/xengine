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

#include "xng/render/graph/passes/presentationpass.hpp"

#include "xng/render/graph/framegraphbuilder.hpp"
#include "xng/render/graph/framegraphsettings.hpp"

namespace xng {
    PresentationPass::PresentationPass() = default;

    void PresentationPass::setup(FrameGraphBuilder &builder) {
        auto renderSize = builder.getBackBufferDescription().size
                          * builder.getSettings().get<float>(FrameGraphSettings::SETTING_RENDER_SCALE);

        auto backBufferDesc = builder.getBackBufferDescription();
        auto backBuffer = builder.getBackBuffer();
        auto screenColor = builder.getSlot(SLOT_SCREEN_COLOR);

        builder.blitColor(screenColor,
                          backBuffer,
                          {},
                          {},
                          renderSize,
                          backBufferDesc.size,
                          TextureFiltering::LINEAR,
                          0,
                          0);
    }
}